/*###############################################################################
## SETUP : 
#################################################################################
## NETWORK SETUP
###############################################################################*/
    const char* ssid =			"MB";
    const char* password =		"K9Wt25qc4U";
    const char* mqtt_server =	"192.168.20.200";

/*###############################################################################
## Devices SETUP 
###############################################################################*/
    #define DEBUG 1
    #define SWITCH 0    // switch with pullup on pin 0 
    #define DS18B20 12  // 1-wire bus for DS18B20 on pin 12
    //#define DHT22_PIN 12  // DHT22 data on pin 12
    #define DEV_BMP180 1    // I2C device.



/*###############################################################################
## MQTT SETUP
## client name MUST be unike for broker
###############################################################################*/

    const char*	clientName =				"ESP_garasje";							//## Unique client name

    //## MQTT topics:
    const char* Topic_DS18B =				"sbv9/garasje/DS1/temperatur";			//## Topic to post DS18B temperature
    const char* Topic_DHT22_humidity =		"";			//## Topic to post Humidity
    const char* Topic_DHT22_temperature =	"";			//## Topic to post Humidity
    const char* Topic_button =				"sbv9/garasje/port_bryter";				//## Topic to post buttonstate
    const char* Topic_BMP180_pressure =		"sbv9/garasje/BMP/trykk";
    const char* Topic_BMP180_temperature =	"sbv9/garasje/BMP/temperatur";

    long		UpdateInterval =			60000;  //ms interval
