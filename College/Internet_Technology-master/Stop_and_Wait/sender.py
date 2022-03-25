import socket
import time

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "localhost"
port = 8000

serversocket.bind((host, port))
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

serversocket.listen(5)
print ('Sender ready and is listening for receiver to connect')

#to accept all incoming connections
clientsocket, address = serversocket.accept()
print("Receiver "+str(address)+" connected")

# Do not share and copy code from each other or from Internet
# Please start your code here for sender

sequence_number = -1

for x in range(10):
    sequence_number = sequence_number + 1

    # make packet
    packet = "Hello: " + str(sequence_number)

    # send packet to receiver over socket
    clientsocket.send(packet.encode())

    # receive ack from receiver
    ack_packet = clientsocket.recv(1024)

    #acknowledge ack received from receiver
    print("SENDER: " + str(ack_packet.decode()))

clientsocket.close()

