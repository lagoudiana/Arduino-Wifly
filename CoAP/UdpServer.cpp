#include "UdpServer.h"

boolean receivePacket(UDPpacket& packet, WiFly& wifly)
{
	boolean ret;
	char buf[100];
	ret = wifly.gets(buf, sizeof(buf));
	packet.data += buf;
// 	do {
// 		rByte = wifly.read();
// 		//Serial.println(rByte);
// 		packet.data = packet.data + (char) rByte;
// 		if((char)rByte == '\n') {
// 			break;
// 		}
// 	}while(rByte != -1);
	packet.dataLength = packet.data.length();
	wifly.getHostIP(packet.ip, sizeof(packet.ip));
	packet.port = wifly.getHostPort();

	return true;
	
// 	if(!packet.data.compareTo("-1"))
// 		return false;
// 	else
// 		return true;
}

boolean sendPacket(UDPpacket& packet, WiFly& wifly)
{
	boolean ret;
	char buf[100];
	
	packet.data.toCharArray(buf, sizeof(buf));

	ret = wifly.sendto((const char *)buf, packet.ip, packet.port);
	wifly.flush();
	return ret;
}

void printRecPacket(UDPpacket packet)
{
	Serial.print("Message of ");
	Serial.print(packet.dataLength);
	Serial.print(" bytes received from : ");
	Serial.print(packet.ip);
	Serial.print(":");
	Serial.println(packet.port);
	Serial.print("MESSAGE : ");
	Serial.println(packet.data);
}

void printSendPacket(UDPpacket packet)
{
	Serial.print("Message of ");
	Serial.print(packet.dataLength);
	Serial.print(" bytes sent to : ");
	Serial.print(packet.ip);
	Serial.print(":");
	Serial.println(packet.port);
	Serial.print("MESSAGE : ");
	Serial.println(packet.data);
}

UDPpacket::UDPpacket()
{

}