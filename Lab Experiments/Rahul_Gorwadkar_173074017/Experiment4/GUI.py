import serial
import matplotlib.pyplot as plt
# import time 
from drawnow import *

ser = serial.Serial('COM7',115200)

plt.ion() #Tell matplotlib you want interactive mode to plot live data
cnt=0
C=3
D=3

def makeFig():
    plt.xlim((0,6))
    plt.ylim((0,6))
    plt.grid(True)
    plt.plot(C,D, 'ro') 

flag=0

drawnow(makeFig)

while(1):
    data = list(ser.read(5))

    if(data[4]==83):
        flag=1

    if(flag==1):
        X= data[1]
        Y= data[3]
        print(X)
        print(Y)

        if((X==67)and(Y==67)):
            C=float(3)
            D=float(3)
            drawnow(makeFig)            
            #plt.show()
        elif((X==67) and (Y==84)):
            C=float(3)
            D=float(6)
            drawnow(makeFig) 
            #plt.show()
        elif((X==67) and (Y==66)):
            C=float(3)
            D=float(0)
            drawnow(makeFig) 
            #plt.show()
        elif((X==76) and (Y==67)):
            C=float(0)
            D=float(3)
            drawnow(makeFig) 
            #plt.show()
        elif((X==82) and (Y==67)):
            C=float(6)
            D=float(3)
            drawnow(makeFig) 
            #plt.show()

        flag=0
        #plt.pause(0.001)
