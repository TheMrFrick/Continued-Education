import socket
import time
import math
import array
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
    for y in range (window_size):
        packet = "Hello:" + str(sequence_number)
        sequence_number = sequence_number + 1
        window = window + packet
        if y != 4:
            window = window + " | "
        
    clientsocket.send(window.encode())


total_packets = 10
acks = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
window_size = 5
segments = math.ceil(total_packets/window_size)
window = []
last_sent = 0
last_ack_received = time.time()
# window(window_size)

sequence_number = 0
fin = finished(acks)
while (not fin):
    
    # Send Packets
    sendPackets()

    # now check the ack's to see what to resend
    # always receiving one ack back at least

    try:
        ack_packet = clientsocket.recv(1024)
        # print("SENDER: " + str(ack_packet.decode()))
        ack_packet = ack_packet.replace("Ack:", "")
        ack_packet = ack_packet.replace("|", "")
        nums = ack_packet.split()
        # print(nums)
        for i in nums:
            i = int(i)
            acks[i] = 1
            print("Ack:" + str(i))

        # num_recv = int(ack_packet.replace("Ack:",""))
        
    except:
        if(time.time() - last_ack_received > 0.01):
            sendPackets()
    
    fin = finished(acks)

clientsocket.close()
