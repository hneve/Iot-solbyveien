/*###############################################################################
## SETUP : 
#################################################################################
## NETWORK SETUP
###############################################################################*/
    const char* ssid =			"";
    const char* password =		"";
    const char* mqtt_server =	"192.168.20.200";

/*###############################################################################
## Devices SETUP 
###############################################################################*/
    //#define DEBUG 1
    //#define SWITCH 0
    //#define DS18B20 12
    //#define DHT22_PIN 12
    //#define DEV_BMP180 1



/*###############################################################################
## MQTT SETUP
## client name MUST be unike for broker
###############################################################################*/

    const char*	clientName =				"";							//## Client name. unike

    //## MQTT topics:
    const char* Topic_DS18B =				"";			//## Topic to post DS18B temperature
    const char* Topic_DHT22_humidity =		"";			//## Topic to post Humidity
    const char* Topic_DHT22_temperature =	"";			//## Topic to post Humidity
    const char* Topic_button =				"";				//## Topic to post buttonstate
    const char* Topic_BMP180_pressure =		"";
    const char* Topic_BMP180_temperature =	"";

    long		UpdateInterval =			60000;      //Update every 60000ms = 60s
