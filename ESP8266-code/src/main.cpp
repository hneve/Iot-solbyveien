

#include "setup_garasjen.h"
//#include "setup_mekkebua.h"

/*###############################################################################
## Nifty DEBUG MACRO
###############################################################################*/
    #ifdef DEBUG
        #define DEBUG_PRINT(x)		Serial.print (x)
        #define DEBUG_PRINTDEC(x)   Serial.print (x, DEC)
        #define DEBUG_PRINTLN(x)	Serial.println (x)
        #define DEBUG_PRINTHEX(x)	Serial.print (x, HEX)
    #else
        #define DEBUG_PRINT(x)
        #define DEBUG_PRINTDEC(x)
        #define DEBUG_PRINTLN(x)
        #define DEBUG_PRINTHEX(x)
    #endif

/*###############################################################################
## Includes and basic variable initiation
###############################################################################*/
    #include <ESP8266WiFi.h>
    #include <PubSubClient.h>
    #include <DallasTemperature.h>
    #include <DHTesp.h>
    #include <Wire.h>
    #include <Adafruit_BMP085.h>

    #define CLK_PIN 14
    #define SDA_PIN 13

    char msg[50];
    bool old_button = false;
    long lastMsg = 0;
    float lastValue = 0;

/*###############################################################################
## Device initiations
###############################################################################*/
    WiFiClient espClient;
    PubSubClient client(espClient);

    #ifdef DS18B20
        OneWire oneWire(DS18B20);
        DallasTemperature ds18b20(&oneWire);
        DeviceAddress insideThermometer;
    #endif

    #ifdef DHT22_PIN
        DHTesp dht;
    #endif

    #ifdef DEV_BMP180
        Adafruit_BMP085 bmp;
    #endif

/*###############################################################################
## WIFI setup sub-function
###############################################################################*/
void setup_wifi() {
	// WIFI setup
	delay(10);
	// We start by connecting to a WiFi network
	DEBUG_PRINT("\nConnecting to ");
	DEBUG_PRINTLN(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		DEBUG_PRINT(".");
	}

	DEBUG_PRINTLN("\nWiFi connected");
	DEBUG_PRINTLN("IP address: ");
	DEBUG_PRINTLN(WiFi.localIP());
} //## END OF setup_wifi()

/*###############################################################################
## callback function for MQTT : when subscribed message arrives
###############################################################################*/
void callback(char* topic, byte* payload, unsigned int length) { 
    DEBUG_PRINT("Message arrived [");
    DEBUG_PRINT(topic);
    DEBUG_PRINTLN("]");
    switch (payload[0]) {
        case 'r':
            snprintf(msg, 50,"%s is alive", clientName);
            client.publish("DEBUG", msg);
            break;
        case 'h':
            snprintf(msg, 50,"%i is alive", ESP.getFreeHeap());
            client.publish("DEBUG", msg);
            break;	
    }
}

/*###############################################################################
## DS18B20 helper functions
###############################################################################*/
#ifdef DS18B20	
    void printAddress(DeviceAddress deviceAddress){
        for (uint8_t i = 0; i < 8; i++){
            if (deviceAddress[i] < 16) Serial.print("0");
            DEBUG_PRINTHEX(deviceAddress[i]);
        }
    }

    float GetTemperature(DeviceAddress deviceAddress){
        float tempC = ds18b20.getTempCByIndex(0);
        DEBUG_PRINT("\nTemp C: ");
        DEBUG_PRINT(tempC);
        DEBUG_PRINT("\t");
        return tempC;
    }
#endif

/*###############################################################################
## Nifty float to char function
###############################################################################*/
void FtoChar2(float fVal, char* cF, int nDigs) {
	long dSep = pow(10, nDigs);
	signed long slVal = fVal * dSep;
	sprintf(cF, "%d.%d", int(slVal / dSep), abs(int(slVal % int(dSep))));
}

/*###############################################################################
## MAIN SETUP
###############################################################################*/
void setup(){
	Serial.begin(115200);
	
	#ifdef SWITCH  	
		pinMode(SWITCH,INPUT);
	#endif
	#ifdef DS18B20		
		ds18b20.begin();
	#endif
	#ifdef DHT22_PIN
		dht.setup(12);
	#endif
	#ifdef DEV_BMP180
		Wire.begin(SDA_PIN, CLK_PIN);
		if (!bmp.begin()) {
			Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	  }
	#endif
	
	
	//#### WIFI begin
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	

	#ifdef DS18B20
		DEBUG_PRINT("Locating devices...");
		DEBUG_PRINT("Found ");
		DEBUG_PRINTDEC(ds18b20.getDeviceCount());
		DEBUG_PRINTLN(" devices.");
		
		if (!ds18b20.getAddress(insideThermometer, 0)) DEBUG_PRINTLN("Unable to find address for Device 0"); 
		DEBUG_PRINT("Device 0 Address: ");
		printAddress(insideThermometer);
		DEBUG_PRINTLN();

		ds18b20.setResolution(insideThermometer, 11);	// (Sensor_adress, bits)
 
		DEBUG_PRINT("Device 0 Resolution: ");
		DEBUG_PRINTDEC(ds18b20.getResolution(insideThermometer)); 
	#endif
}	//## END OF setup()

/*###############################################################################
## Connect and reconnect to MQTT server
###############################################################################*/
void reconnect() {	
	while (!client.connected()) {		// Loop until we're reconnected
		DEBUG_PRINTLN("\nAttempting MQTT connection...");
		if (client.connect(clientName)) {	// Attempt to connect
			DEBUG_PRINT(clientName);
			DEBUG_PRINTLN(" connected");
			snprintf(msg, 50,"Device %s connected", clientName);	
			client.publish("DEBUG", msg);	// Send connected rapport 
			
			client.subscribe("CTRL");	// Subscribe to control topic
		} 
		else {
			DEBUG_PRINT("failed, rc=");
			DEBUG_PRINT(client.state());
			DEBUG_PRINTLN(" try again in 5 seconds");
			delay(5000);	// Wait 5 seconds before retrying
		}
	}
}

/*###############################################################################
## MAIN LOOP 
###############################################################################*/
void loop() {
	if (!client.connected()) { reconnect();}
	client.loop();
	
/*###############################################################################
## Loop for switch sensor
###############################################################################*/
	#ifdef SWITCH 
		bool button = !digitalRead(SWITCH);  //GPIO 0 is normal high, low when button pressed
		if(button != old_button) { // Dont send same message more than once.
			old_button = button;
			if(button)  client.publish(Topic_button,"1");
			else		client.publish(Topic_button,"0");
		}
	#endif
	
/*###############################################################################
## Loop Timekeeping 
###############################################################################*/
	long now = millis();
	if (now - lastMsg > UpdateInterval) {
		lastMsg = now;
		
/*############################################################################### 
## Loop for DS18B20 sensor
###############################################################################*/		
	#ifdef DS18B20
		ds18b20.requestTemperatures();
		float tempC = ds18b20.getTempCByIndex(0);
		FtoChar2(tempC, msg,1);	
		DEBUG_PRINT("\nDS18B20: ");
		DEBUG_PRINT(msg);
		client.publish(Topic_DS18B, msg);
	#endif	
	
/*###############################################################################
 ## Loop for DHT22 sensor
###############################################################################*/
	#ifdef DHT22_PIN
		FtoChar2(dht.getTemperature(), msg, 1);
		DEBUG_PRINT("\nDHT22 T:");
		DEBUG_PRINT(msg);
		client.publish(Topic_DHT22_temperature, msg);
		
		FtoChar2(dht.getHumidity(), msg, 1);
		DEBUG_PRINT("\tH:");
		DEBUG_PRINT(msg);
		client.publish(Topic_DHT22_humidity, msg);
	
	#endif
	
/*###############################################################################
## Loop for BMP180 sensor
###############################################################################*/
	#ifdef DEV_BMP180
		FtoChar2(bmp.readTemperature(), msg,1);
	    DEBUG_PRINT("\nBMP180 T: ");
	    DEBUG_PRINT(msg);
		client.publish(Topic_BMP180_temperature, msg);
		
	    DEBUG_PRINT("\tP: ");
		FtoChar2(bmp.readPressure()/100.0, msg,1);
	    DEBUG_PRINT(msg);
	    DEBUG_PRINT(" hPa");
		client.publish(Topic_BMP180_pressure, msg);
	#endif
	
	} //############################ END of Timekeeping
}//################################# END of MAIN LOOP




