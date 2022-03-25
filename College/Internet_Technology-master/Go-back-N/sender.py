import socket
import time
import math
import array

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
def finished(acks):
    for i in range(10):
        if acks[i] == 0:
            return False
    return True

def getIndex(acks):
    for i in range(10):
        if acks[i] == 0:
            return i
    return -1

def sendPackets():
    window = ""
    # send # of packets = window size without waiting for ack
    # create the window
    sequence_number = getIndex(acks)
    for y in range (5):
        if sequence_number > 9 :
            break
        packet = "Hello:" + str(sequence_number)
        sequence_number = sequence_number + 1
        window = window + packet
        if y != 4:
            window = window + " | "
    print(len(window))
    clientsocket.send(window.encode())

total_packets = 10
acks = [0,0,0,0,0,0,0,0,0,0]
# window(window_size)
last_ack_received = time.time()
fin = finished(acks)
while (not fin):
    
    # send # of packets = window size without waiting for ack
    sendPackets()

    # now check the ack's to see what to resend
    try:
        for y in range(5):
            ack_packet = clientsocket.recv(5)
            print("SENDER: " + str(ack_packet.decode()))
            num_recv = int(ack_packet.replace("Ack:",""))
            last_ack_received = time.time()
            acks[num_recv] = 1
    except:
        if(time.time() - last_ack_received > 0.05):
            sendPackets()

    
    fin = finished(acks)
    
    

clientsocket.close()