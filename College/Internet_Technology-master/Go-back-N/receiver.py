import socket
import re

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host ="localhost"
port =8000
s.connect((host,port))

# Do not share and copy code from each other or from Internet
# Please start your code here for receiver

data = s.recv(47)
acks = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
curr_index = 0

def getLatest(acks):
    for i in range(len(acks)):
        if acks[i] == 0:
            if i == 0:
                return 0
            else:
                return i - 1

while data != b'':
    
    data = data.decode()
    print("RECEIVER: " + data)
    data = data.replace("Hello:", "")
    data = data.replace("|", "")
    nums = data.split() 
    # print(nums)

    for i in nums:
        i = int(i)
        if acks[i] == 1:
            latest = getLatest(acks)
            ack_packet = "Ack:" +str(latest)
            s.send(ack_packet.encode())
            continue # disregard number
        elif i > curr_index:
            latest = getLatest(acks)
            ack_packet = "Ack:" +str(latest)
            s.send(ack_packet.encode())
            continue #disregard since out of order
        else:
            ack_packet = "Ack:" + str(i)
            acks[i] = 1
            print("Rcv:" + str(i))
            s.send(ack_packet.encode())
            curr_index = curr_index + 1

    # get next packet
    data = s.recv(47)