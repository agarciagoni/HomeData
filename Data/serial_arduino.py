# -*- coding: utf-8 -*-
"""
Created on Fri Nov  1 10:55:58 2019

@author: Alejandro
"""
import serial
import time
import numpy as np
import pandas as pd
ser = serial.Serial(
    port='COM4',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        )


while True:
    data=str(ser.readline().decode('utf-8'))
    print(data)
    data=data.split('/')
   ## print(data[0])
    data=pd.DataFrame(data)
 ##  print(data)
    data.to_csv("sensors_arduino.csv")
##    data=data.split(',')
##    x(float(data[0]))
##
##    y = float(data[1])
##    z = float(data[2])
#    t=float(data[3])
#    l=float(data[4])
#    h=float(data[5])
#
#    data_ter=data_ter.append({'Temperature':t, 'Light':l,'Humidity':h},ignore_index=True)
#    data_ter.to_csv('termites.csv')
##    data_ter['Light'].append()
##    data_ter['Humidity'].append(float(data[5]))
##    proximity.append(float(data[6]))
##    pressure.append(float(data[7]))
##    altitude = float(data[8])
##    dewpoint = (float(data[9]))

ser.close()

    