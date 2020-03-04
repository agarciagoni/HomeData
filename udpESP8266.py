# -*- coding: utf- -*-
"""

@author: Alejandro Garcia
"""

import socket
import pandas as pd
import csv
import time

from datetime import datetime
datetime.now().strftime('%d/%m/%Y')
####### MODIFY THE FOLLOWING TWO LINES ##########################################################
UDP_IP = "" #Use the same address that was specified on the UDP Settings.
UDP_PORT = 8888 #Use the same port that was specified on the UDP Settings.
#################################################################################################
print("Connecting...")
time.sleep(5)  # Wait for the NodeMCU to connect to the internet.
try:
    sock = socket.socket(socket.AF_INET, # Internet
                         socket.SOCK_DGRAM) # UDP
except:
    print('Not able to connect over UDP')

while True:
    message='Hello Node'
    try:
        sock.sendto(message.encode(),(UDP_IP, UDP_PORT))
        #    sock.bind((UDP_IP, UDP_PORT))
        data,addr= sock.recvfrom(2024)
        print(data.decode())
    except:
        print('problem')
    time.sleep(0.1)
    

