/**
 *
 */
//Uncomment for DEBUG in Serial Port
//#define DEBUG
#define Wifly

#ifdef Wifly

//#undef PROGMEM 
//#define PROGMEM __attribute__(( section(".progmem.data") )) 
//#undef PSTR 
//#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(2,3);

WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "ssid";
const char myPassword[] = "password";

#else
//Ethernet Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
byte mac[] = {
  0xAA, 0xAD, 0xBA, 0xED, 0xa3, 0xED};
IPAddress ip(150, 140, 5, 67);
unsigned int localPort = 5683;      // local port to listen on
EthernetUDP EthUDP;
#endif 

//Coap Library
#include <coap.h>
//#include "GetSensor.h"
#include "PostSensor.h"
//#include "AnalogSensor.h"
Coap coap;
//#define ALL

//Runs only once
void setup()
{
	pinMode(13, OUTPUT);
	char buf[32];
    
	Serial.begin(115200);
	Serial.println("Starting");
	Serial.print("Free Memory: ");
	Serial.println(wifly.getFreeMemory(), DEC);  // DECimal representation of free memory.
    
	wifiSerial.begin(9600);
    
	if(!wifly.begin(&wifiSerial, &Serial)) {
		Serial.println("Failed to start wifly!");
	}
    
	if(wifly.getFlushTimeout() != 10) {
		Serial.println("Restoring flush timeout to 10 msecs.");
		wifly.setFlushTimeout(10);
		if(!wifly.save()) {
			Serial.println("Failed to save configuration!");
		}
		if(!wifly.reboot()) {
			Serial.println("Failed to reboot!");
		}
		delay(3000);
	}
    
	/* Join wifi network if not already associated */
	if(!wifly.isAssociated()) {
		/* Set up the WiFly to connect to a wifi network */
		Serial.println("Joining network.");
		wifly.setSSID(mySSID);
		wifly.setPassphrase(myPassword);
		wifly.enableDHCP();

		if(wifly.join()) {
			Serial.println("Joined wifi network.");
		}
		else {
			Serial.println("Failed to join wifi network.");
		}
	}
	else {
		Serial.println("Already joined network.");
	}

	wifly.setBroadcastInterval(0);	// Turn off UPD broadcast

	wifly.setDeviceID("Wifly-CoAP");
	Serial.print("DeviceID: ");
	Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

	wifly.enableUdpAutoPair();

	/* Setup for UDP packets, sent automatically */
	wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
	if(wifly.getPort() != 5683) {
		wifly.setPort(5683);
		Serial.print("Set localport to : ");
		Serial.println(wifly.getPort());
		wifly.save();
		wifly.reboot();
		delay(3000);
	}
	
	Serial.print("MAC: ");
	Serial.println(wifly.getMAC(buf, sizeof(buf)));
	Serial.print("IP: ");
	Serial.println(wifly.getIP(buf, sizeof(buf)));
	Serial.print("Netmask: ");
	Serial.println(wifly.getNetmask(buf, sizeof(buf)));
	Serial.print("Gateway: ");
	Serial.println(wifly.getGateway(buf, sizeof(buf)));
	Serial.println("WiFly Ready.");

	//Init Coap
	coap.init(&wifly);

	char name[3];  
	name[1]='S';     
	name[2]='\0';
	//for (int i=0;i<CONF_MAX_RESOURCES;i++){
	name[0]=0x30;
	CoapSensor * example_post_resource = new postSensor(name);
	coap.add_resource(example_post_resource);
	//} 


/*
  DBG(
  Serial.println("Setup...Done!");
  )*/
}

void loop()
{
	// nothing else should be done here. CoAP service is running
	// if there is a request for your resource, your callback function will be triggered
	coap.handler();
// 	int mbyte=-1;
// 	do{
// 		mbyte = wifly.read();
// 		if (mbyte!=-1){
// 			digitalWrite(13,HIGH);
// 			delay(100);
// 			digitalWrite(13,LOW);
// 			delay(100);
// 		}
// 	}while(mbyte!=-1);
}
