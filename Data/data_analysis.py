# -*- coding: utf-8 -*-
"""
Created on Thu Nov 14 10:59:19 2019

@author: Alejandro

This script plots the data gathered from the sensors in the Piccolo Kitchen and
starts running the first analysis. At the moment just:
    -Plots.
    
    
Plot info using seaborn: https://seaborn.pydata.org/api.html
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_name='C:/Users/Alejandro/Desktop/MIT Media Lab/codes/full_kitchen_sensor/Data/kitchen_status.csv'
data=pd.read_csv(file_name)
x=np.arange(data.shape[0])
sns.pairplot(data)
#g=sns.FacetGrid(data)

sns.relplot(x='Time',y='Temperature',  
            sizes=(40, 400), alpha=.5, palette="muted",
            height=6, data=data,kind='line')
sns.lineplot(data=data['Temperature'])
time=data.iloc[:,0].tolist()
#Categorical variables
x_cat=np.arange(data.shape[0])
y_cat='Position'
hue_cat='Position Back' #Color to classify
col_cat='Position Back' #Columns to classify
sns.catplot(x=x_cat,y=y_cat,data=data)
sns.catplot(x=x_cat,y=y_cat,data=data,kind='violin')
sns.catplot(x=x_cat,y=y_cat,data=data,hue=hue_cat)
sns.catplot(x=x_cat,y=y_cat,data=data,col=col_cat)
