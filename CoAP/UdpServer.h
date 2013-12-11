/* Simple UDP packet  */
#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_

#include <Arduino.h>
#include "WiFlyHQ.h"

class UDPpacket {
public:
	UDPpacket();
	char ip[16];
	uint16_t port;
	String data;
	uint16_t dataLength;
};

boolean receivePacket(UDPpacket& packet, WiFly& wifly);
boolean sendPacket(UDPpacket& packet, WiFly& wifly);
void printRecPacket(UDPpacket packet);
void printSendPacket(UDPpacket packet);

#endif