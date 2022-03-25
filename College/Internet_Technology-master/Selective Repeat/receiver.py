import socket
import re

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host ="localhost"
port =8000
s.connect((host,port))

# Do not share and copy code from each other or from Internet
# Please start your code here for receiver

data = s.recv(1024)
acks = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

while data != b'':
    
    data = data.decode()
    # print("RECEIVER: " + data)
    data = data.replace("Hello:", "")
    data = data.replace("|", "")
    nums = data.split()
    # print(nums)

    window = ""
    for i in nums:
        i = int(i)
        if acks[i] == 1:
            continue # disregard packet
        else:
            window = window + "Ack:" + str(i)
            window = window + " | "
            acks[i] = 1
            print("Rcv:" + str(i))

    # Send the window back
    s.send(window.encode())

    # get next packet
    data = s.recv(1024)