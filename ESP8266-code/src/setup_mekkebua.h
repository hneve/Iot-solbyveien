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
    //#define SWITCH 0
    //#define DS18B20 12
    #define DHT22_PIN 12
    //#define DEV_BMP180 1



/*###############################################################################
## MQTT SETUP
## client name MUST be unike for broker
###############################################################################*/

    const char*	clientName =				"ESP_garasje";							//## Client name. unike

    //## MQTT topics:
    const char* Topic_DS18B =				"x";			//## Topic to post DS18B temperature
    const char* Topic_DHT22_humidity =		"sbv9/mekkepult2/fuktighet";			//## Topic to post Humidity
    const char* Topic_DHT22_temperature =	"sbv9/mekkepult/temperatur";			//## Topic to post Humidity
    const char* Topic_button =				"x";				//## Topic to post buttonstate
    const char* Topic_BMP180_pressure =		"x";
    const char* Topic_BMP180_temperature =	"x";

    long		UpdateInterval =			60000;
