# -*- coding: utf-8 -*-
"""
Created on Sat Oct 19 13:27:33 2019

@author: Alejandro
"""

import socket
import pandas as pd
import csv



#######MODIFY THE FOLLOWING TWO LINES ##########################################################
UDP_IP = "192.168.43.156" #Use the same address that was specified on the UDP Settings.
UDP_PORT = 19909 #Use the same port that was specified on the UDP Settings.
################################################################################################

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

with open('csv_udp.csv', mode='w',newline='') as csv_file:
#       fieldnames = ['Temperature', 'Humidity', 'Light']
       writer = csv.writer(csv_file)
##        writer.writeheader()

       while True:
            data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
            x=data.decode().replace(' = ',':').split(' ')[1].split(':')[1]
            y=data.decode().replace(' = ',':').split(' ')[2].split(':')[1]
            z=data.decode().replace(' = ',':').split(' ')[3].split(':')[1]
            temp=data.decode().replace(' = ',':').split(' ')[4].split(':')[1]
            hum=data.decode().replace(' = ',':').split(' ')[5].split(':')[1]
            lit=data.decode().replace(' = ',':').split(' ')[6].split(':')[1]
            prox=data.decode().replace(' = ',':').split(' ')[7].split(':')[1]
            pre=data.decode().replace(' = ',':').split(' ')[8].split(':')[1]
            alt=data.decode().replace(' = ',':').split(' ')[9].split(':')[1]
            dew=data.decode().replace(' = ',':').split(' ')[10].split(':')[1]
 

            writer.writerow([x,y,z,temp,hum,lit,prox,pre,alt,dew])             
#            writer.writerow([temp,humidity,light]) 
    #    envir=envir.append({'Temperature':temp,'Humidity':humidity,'Light':light},ignore_index=True)
#    envir.to_csv('udp_receive.csv')
#    with open('csv_udp2.csv', mode='w') as csv_file:
#       fieldnames = ['Temperature', 'Humidity', 'Light']
#       writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
##        writer.writeheader() 
#.replace(' = ',':').split(' ')[:].split(':')
#    .split('Temp')[1].split(' ')

