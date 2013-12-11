import socket

UDP_IP = "192.168.1.4"
UDP_PORT = 5683
bind_udp_ip = "192.168.1.2"
bind_udp_port = 8042
MESSAGE = "message: "
COUNT = 1

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT

sock = socket.socket(socket.AF_INET, # Internet
              socket.SOCK_DGRAM) # UDP
              
sock1 = socket.socket(socket.AF_INET, # Internet
              socket.SOCK_DGRAM) # UDP
sock1.bind((bind_udp_ip, bind_udp_port))
                        
while True:
	con = raw_input("Press Entet to continue, q to quit : ")
	if(con == 'q'):
		break
	sock.sendto(MESSAGE + str(COUNT) , (UDP_IP, UDP_PORT))
	print "Message Sent:", MESSAGE + str(COUNT) + '\n'
	COUNT = COUNT + 1
	data, addr = sock1.recvfrom(1024) # buffer size is 1024 bytes
	print "Received message:", data
	print "Received addr:", addr