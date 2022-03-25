import socket
import re

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host ="localhost"
port =8000
s.connect((host,port))

# Do not share and copy code from each other or from Internet
# Please start your code here for receiver

data = s.recv(1024)

while data != b'':
    data = data.decode()
    print("RECEIVER: " + data)
    sequence_number = data.replace("Hello: ", "")
    
    ack_packet = "Ack:" + str(sequence_number)
    s.send(ack_packet.encode())

    # get next packet
    data = s.recv(1024)