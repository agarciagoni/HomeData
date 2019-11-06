# -*- coding: utf-8 -*-
"""
Created on Fri Nov  1 10:55:58 2019

@author: Alejandro
"""
import time
import serial
from datetime import datetime
datetime.now().strftime('%H:%M:%S%S')
##import numpy as np
import pandas as pd
ser = serial.Serial(
    port='COM4',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        )

kitchen_status=pd.DataFrame(columns=['Time','Position','Sound 1','Sound 2','Position Back','N Mov Back','N Back pos','N Back total','Position Front','N Mov Front','N Front pos','N Front total']);
##time.sleep(5.0)
while True:
    data=str(ser.readline().decode('utf-8'))
    ##print(data)
    data=data.split('/')
    #print(data[0])
  ##  data=pd.DataFrame(data)
 ##  print(data)
    try: 
        kitchen_status=kitchen_status.append({'Time':datetime.now().strftime('%H:%M:%f'),
                                              'Position': data[0],
                                              'Sound 1': data[1],'Sound 2': data[2],
                                              'Position Back': data[3],'N Mov Back':data[4],
                                              'N Back pos': data[5],'N Back total':data[6],
                                              'Position Front': data[7],'N Mov Front':data[8] ,
                                              'N Front pos': data[9],'N Front total':data[10].strip("\r\n")}
        ,ignore_index=True)
    except:
        print(data)

    kitchen_status.to_csv('kitchen_status_T0.csv',index=False)


ser.close()

    