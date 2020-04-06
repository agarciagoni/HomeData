# -*- coding: utf-8 -*-
"""

@author: Alejandro Garcia
"""

import socket
import pandas as pd
#import csv
import time
from datetime import datetime

data_kit=pd.DataFrame(columns=['Time',
                               'Sound1','Sound 1 Level', 'Sound2','Sound 2 Level',
                               'Distance sonic','Distance Laser','User position', #Add 'Distance Sonar' if sensor included.
                               'Back Cab Position','Back Movements','Back Interactions in Position','Back Interactions in Total',
                               'Right Cab Position','Right Movements','Right Interactions in Position','Right Interactions in Total',
                               'Left Cab Position','Left Movements','Left Interactions in Position','Left Interactions in Total'])

file_name='KitchenData_'+datetime.now().strftime('%m_%d_%H_%M_%S')+'.csv'

####### MODIFY THE FOLLOWING TWO LINES ##########################################################
UDP_IP = "" #Use the same address that was specified on the UDP Settings.
UDP_PORT = 8888 #Use the same port that was specified on the UDP Settings.
#################################################################################################

print("Connecting...")
time.sleep(2)  # Wait for the NodeMCU to connect to the internet.

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
        data=data.decode().split(',')
        try:
            data_kit=data_kit.append({'Time':datetime.now().strftime('%H:%M:%S'),
                                   'Sound1':data[0],'Sound 1 Level':data[1], 'Sound2':data[2],'Sound 2 Level':data[3],
                                   'Distance sonic':data[4],'Distance Laser':data[5],'User position':data[6], #Add 'Distance Sonar':data[5] and reorder number if Distance Sonar included.
                                   'Back Cab Position':data[7],'Back Movements':data[8],'Back Interactions in Position':data[9],'Back Interactions in Total':data[10],
                                   'Right Cab Position':data[11],'Right Movements':data[12],'Right Interactions in Position':data[13],'Right Interactions in Total':data[14],
                                   'Left Cab Position':data[15],'Left Movements':data[16],'Left Interactions in Position':data[17],'Left Interactions in Total':data[18]},ignore_index=True)
            data_kit.to_csv(file_name)
        except:
            print('bad row')
            
    except:
        print('connection problem')
    time.sleep(0.1)
    

