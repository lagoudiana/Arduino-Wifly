 /*
  * WiFlyHQ Example udpserver.ino
  *
  *This sketch implements a simple UDP server
  *
  */

#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(2,3);

#include <UdpServer.h>

const char SSID[] = "SSID";
const char Password[] = "password";

void terminal();

WiFly wifly;
uint32_t packetsRecNum = 0;
uint32_t packetsSendNum = 0;

void setup()
{
	char buf[32];
    
	Serial.begin(115200);
	Serial.println("Starting");
	Serial.print("Free Memory: ");
	Serial.println(wifly.getFreeMemory(), DEC);  // DECimal representation of free memory.
    
	wifiSerial.begin(9600);
    
	if(!wifly.begin(&wifiSerial, &Serial)) {
		Serial.println("Failed to start wifly!");
		terminal();
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
		wifly.setSSID(SSID);
		wifly.setPassphrase(Password);
		wifly.enableDHCP();

		if(wifly.join()) {
			Serial.println("Joined wifi network.");
		}
		else {
			Serial.println("Failed to join wifi network.");
			terminal();
		}
	}
	else {
		Serial.println("Already joined network.");
	}
    
	wifly.setBroadcastInterval(0);	// Turn off UPD broadcast
    
	wifly.setDeviceID("Wifly-UDPserver");
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
	Serial.println("Waiting for UDP packets...");
}

void loop()
{
	UDPpacket sendPac;
	UDPpacket recPac;

	sendPac.data = "Hello World";
	sendPac.dataLength = sendPac.data.length();

	if(wifly.available() > 0) {
		if(receivePacket(recPac, wifly)) {
			Serial.println("\n\nPacket received!!");
			printRecPacket(recPac);
			packetsRecNum++;
			Serial.print("Number of received packets : ");
			Serial.println(packetsRecNum);
		}
		else
			Serial.println("Packet failed to be received!!");
	}

	if(packetsRecNum > packetsSendNum) {
		wifly.getHostIP(sendPac.ip, sizeof(sendPac.ip));
		sendPac.port = wifly.getHostPort();
		if(sendPacket(sendPac, wifly)) {
			Serial.println("Packet sent!!");
			printSendPacket(sendPac);
			packetsSendNum++;
			Serial.print("Number of send packets : "); 
			Serial.println(packetsSendNum);
			Serial.println("\nWaiting for UDP packets...");
		}
		else {
			Serial.println("Packet failed to be sent!!");
		}
	}
}

void terminal()
{
	Serial.println("Terminal ready");
	while (1) {
	if (wifly.available() > 0) {
		Serial.write(wifly.read());
	}

	if (Serial.available()) {
		wifly.write(Serial.read());
	}
    }
}