#!/usr/bin/env python
import Tkinter as tkinter
#import tkinter
import matplotlib.pyplot as plt
import PIL.Image
import PIL
import PIL.ImageTk
import time
import threading
import serial
from socket import *
import numpy as np
#graph new
import matplotlib
matplotlib.use("TkAgg")
#from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg,NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.animation as animation
from matplotlib import style
import matplotlib.cm as cm
from matplotlib.colors import LogNorm

import servidorUdp1 as servidorUdp

#GUI Root Window

top = tkinter.Tk()
top.title("Projetobo")
top.resizable(width = 0, height = 0)

#GUIvariables

decoded_bytes = 'a'

#Limpar Variaves Globais

robotStatus = ''
robotStatusVerif = ''
speedV = 0.0
speedO = 0.0
#speedT = 0.0
recievedSpeedV = 0.0
#recievedSpeedY = 0.0
recievedSpeedO = 0.0
poseX = 0.0
poseY = 0.0
poseT = 0.0
temp1 = 0.0
temp2 = 0.0
temp3 = 0.0
temp4 = 0.0
linec = 0
startSeconds = time.time()
currentSeconds = time.time()
timeB = np.arange(50)
#timeB = [4,7,5,2,0,0,0,0,0,0]
tempS1 = np.arange (50)
#tempS1 = [20,24,38,39,30,32,37,21,22,29]
#tempS2 = [0,2,4,9,0,0,0,0,0,0]
#tempS3 = [0,2,4,9,0,0,0,0,0,0]
#tempS4 = [0,2,4,9,0,0,0,0,0,0]
speedSV = np.arange(50)
speedSO = np.arange(50)
poseSX = np.arange(50)
poseSY = np.arange(50)
poseST = np.arange(50)

photo = PIL.ImageTk.PhotoImage(PIL.Image.open('rob.jpg'))
manualOn = (False)

#Serial COM Variables

receivingStatusF = (False)
receivingSpeedF = (False)
receivingPositionF = (False)


#Begin GUI

###########################################################################################


#updater
starttime = 0
for i in range (-50,0):
    timeB[starttime] = i
    starttime = starttime+1
print (timeB)

def updater():

    global decoded_bytes

    robotStatus = ''
    #speedY = 0.0
    #robotStatus = ''
    speedV = 0.0
    #speedY = 0.0
    speedO = 0.0
    #poseX = 0.0
    #poseY = 0.0
    #poseT = 0.0
    temp1 = 0.0
    temp2 = 0.0
    temp3 = 0.0
    temp4 = 0.0
    while(1):

        currentSeconds = time.time()
        time.sleep(1)
        for i in range(49):
            timeB[i]=timeB[i+1]
        timeB[49] = int (currentSeconds-startSeconds)
        print(timeB)
        #speedX = timeB[9]+990
        temp1 = timeB[9]+990


        #Begin Temp

        ############################################################
        '''
        serverSocket = socket(AF_INET, SOCK_DGRAM)
        portAdress = ('192.168.043.033', 5555)
        serverSocket.bind(portAdress)
        print('waiting')
        tempvalues = serverSocket.recvfrom(1024)
        print(tempvalues)
        '''
        ############################################################

        #End Temp


        #Beguin Serial COM

        ###########################################################################################


        #UNCOMENT?

        #ser = serial.Serial('/dev/ttyACM0') #set correct port
         #UNCOMENT
        ser = serial.Serial()
        #ser.port = 'COM12'
        ser.port = '/dev/ttyUSB0'
        ser.boudrate = 9600
        ser.open()
        #ser.flushInput()
        print("open?")
        '''
        ser2 = serial.Serial()
        ser.port = 'COM11'
        ser.boudrate = 9600
        ser.open()
        #ser.flushInput()
        print("open?")
        '''
        #UNCOMENT ABOVE
        print ('chegou?')

        ser.write(b'0-000.100-000.100-000.500\n')
        ser.read()
        linec =0
        try:
            while(linec <6):
                if (linec == 0):  #IF MUST READ ON SAME CICLE include while linec <6 and set linec to 0 after that
                    robotStatusVerif = ser.readline()#UNCOMENT
                    #robotStatusVerif = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8")) #UNCOMENT?
                    linec = linec + 1
                elif (linec == 1):
                    poseX = ser.readline()#UNCOMENT
                    linec = linec + 1
                elif (linec == 2):
                    poseY = ser.readline()#UNCOMENT
                    linec = linec + 1
                elif (linec == 3):
                    poseT = ser.readline()#UNCOMENT
                    linec = linec + 1
                elif (linec == 4):
                    speedV = ser.readline()#UNCOMENT
                    linec = linec + 1
                elif (linec == 5):
                    speedO = ser.readline()#UNCOMENT
                    linec = linec+1
            linec = 0
            print("open?")
            #decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))  #UNCOMENT
            #print(decoded_bytes)
            #print(decoded_bytes)
            #decoded_bytes = str('1-684.125-688.111-879.548-215.358-511.010')


            #UNCOMENT BELOW
            #if robotStatusVerif == 0:
            #    robotStatus = 'Aguardando'
            #elif robotStatusVerif == 1:
            #    robotStatus = 'Movendo'
            #elif robotStatusVerif == 2:
            #    robotStatus = 'Girando'
            #UNCOMENT ABOVE


            '''
            if decoded_bytes[0] == '0':
                robotStatus = 'Aguardando'
            elif decoded_bytes[0] == '1':
                print(decoded_bytes)
                robotStatus = 'Movimentando'
                poseX = float(decoded_bytes[2:9])
                poseY = float(decoded_bytes[10:17])
                poseT = float(decoded_bytes[18:25])
                speedV = float(decoded_bytes[26:33])
                #speedO = float(decoded_bytes[34:41])
            elif decoded_bytes[0] == '2':
                robotStatus = 'Rotacionando'
                poseX = float(decoded_bytes[2:9])
                poseY = float(decoded_bytes[10:17])
                poseT = float(decoded_bytes[18:25])
                speedO = float(decoded_bytes[34:41])
            else:
                print('Invalid Command')
            '''
            '''
                #FOR FREEEEEEEEEEE CONTROOOOOOOOOL! =DDDD

                poseX = float(decoded_bytes[2:9])
                poseY = float(decoded_bytes[10:17])
                poseT = float(decoded_bytes[18:25])
                speedV = float(decoded_bytes[26:33])
                speedO = float(decoded_bytes[34:41])
                #speedT = float(decoded_bytes[42:48])
            '''

        except:
            print('No Serial')

    ###########################################################################################

    #End Serial COM
        temp1 = servidorUdp.flys[0].temps[49]
        temp2 = servidorUdp.flys[1].temps[49]
        temp3 = servidorUdp.flys[2].temps[49]
        temp4 = servidorUdp.flys[3].temps[49]

        #Update Robot Data

        robotStatusL = tkinter.Label (box1, anchor = 'w', text = "Status de Rob:"+'%s' %(robotStatus), padx = 2)
        #decoded_bytes = ('1-325.548')
        #speedX = float(decoded_bytes[2]+decoded_bytes[3]+decoded_bytes[4]+decoded_bytes[5]+decoded_bytes[6]+decoded_bytes[7]+decoded_bytes[8])
        #print(speedX)
        speedVValue = tkinter.Label(robotDataBox, text = '%s' % (speedV))
        speedOValue = tkinter.Label(robotDataBox, text = '%s' % (speedO))
        poseXValue = tkinter.Label(robotDataBox, text = '%s' % (poseX))
        poseYValue = tkinter.Label(robotDataBox, text = '%s' % (poseY))
        poseTValue = tkinter.Label(robotDataBox, text = '%s' % (poseT))
        #speedTValue = tkinter.Label(robotDataBox, text = '%s' % (speedT))

        #Update Temp Data

        temp1Value = tkinter.Label(tempDataBox, text = '%s' % (temp1))
        temp2Value = tkinter.Label(tempDataBox, text = '%s' % (temp2))
        temp3Value = tkinter.Label(tempDataBox, text = '%s' % (temp3))
        temp4Value = tkinter.Label(tempDataBox, text = '%s' % (temp4))

        #Speed Updated Robot Data

        robotStatusL.grid(row = 1, column = 1, sticky = 'W', pady = (4,0))
        speedVValue.grid(row = 1, column = 2, sticky ='E',padx=(0,0))
        speedOValue.grid(row = 2, column = 2, sticky ='E')
        poseXValue.grid(row = 4, column = 2, sticky ='E')
        poseYValue.grid(row = 5, column = 2, sticky ='E')
        poseTValue.grid(row = 6, column = 2, sticky ='E')
        #speedTValue.grid(row = 3, column = 2, sticky ='E')

        #Speed Updated Temp Data

        temp1Value.grid(row = 1, column = 2, sticky = 'E',padx = (0,0))
        temp2Value.grid(row = 2, column = 2, sticky = 'E')
        temp3Value.grid(row = 3, column = 2, sticky = 'E')
        temp4Value.grid(row = 4, column = 2, sticky = 'E')

        #write coordenates to serials here

#Updater End

#Serial Updater

########################################################################



########################################################################

#Serial Updater End

#Graph

def graph():
    fig = plt.figure()
    fig.suptitle ('Grafico de Temperaturas')
    ax1 = fig.add_subplot(221)
    ax2 = fig.add_subplot(222)
    ax3 = fig.add_subplot(223)
    ax4 = fig.add_subplot(224)

    fig2 = plt.figure()
    fig2.suptitle ('Grafico Robo Lider')
    ax5 = fig2.add_subplot(231)
    ax6 = fig2.add_subplot(232)
    ax7 = fig2.add_subplot(233)
    ax8 = fig2.add_subplot(234)
    ax9 = fig2.add_subplot(235)

    fig3 = plt.figure()
    fig3.suptitle ('Grafico Robo Escravo')
    ax10 = fig3.add_subplot(231)
    ax11 = fig3.add_subplot(232)
    ax12 = fig3.add_subplot(233)
    ax13 = fig3.add_subplot(234)
    ax14 = fig3.add_subplot(235)
    def animate1(i):
        #pullData = open("sampleText.txt","r").read()
        #dataArray = pullData.split('\n')
        #xar = []
        #yar = []
        #for eachLine in dataArray:
            #if len(eachLine)>1:
        #x,y = eachLine.split(',')
        #xar.append(int(x))
        #yar.append(int(y))
        ax1.clear()
        ax1.plot(timeB,servidorUdp.flys[0].temps)
        ax1.set_title('Sensor 1')
        ax1.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        print('tempf0')
        print(servidorUdp.flys[0].temps)
        ax2.clear()
        ax2.plot(timeB,servidorUdp.flys[1].temps)
        ax2.set_title('Sensor 2')
        ax2.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        print('tempf1')
        print(servidorUdp.flys[1].temps)
        ax3.clear()
        ax3.plot(timeB,servidorUdp.flys[0].temps)
        ax3.set_title('Sensor 3')
        ax3.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax4.clear()
        ax4.plot(timeB,servidorUdp.flys[1].temps)
        ax4.set_title('Sensor 4')
        ax4.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        '''
        ax5.clear()
        ax5.plot(timeB,servidorUdp.flys[0].temps)
        ax5.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax6.clear()
        ax6.plot(timeB,servidorUdp.flys[1].temps)
        ax6.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax7.clear()
        ax7.plot(timeB,servidorUdp.flys[0].temps)
        ax7.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax8.clear()
        ax8.plot(timeB,servidorUdp.flys[1].temps)
        ax8.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax9.clear()
        ax9.plot(timeB,servidorUdp.flys[0].temps)
        ax9.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax10.clear()
        ax10.plot(timeB,servidorUdp.flys[0].temps)
        ax10.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax11.clear()
        ax11.plot(timeB,servidorUdp.flys[0].temps)
        ax11.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax12.clear()
        ax12.plot(timeB,servidorUdp.flys[0].temps)
        ax12.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax13.clear()
        ax13.plot(timeB,servidorUdp.flys[0].temps)
        ax13.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        ax14.clear()
        ax14.plot(timeB,servidorUdp.flys[0].temps)
        ax14.set(xlabel='Tempo (s)',ylabel = 'Temperatura (C)')
        '''
    def animate2(i):
        #pullData = open("sampleText.txt","r").read()
        #dataArray = pullData.split('\n')
        #xar = []
        #yar = []
        #for eachLine in dataArray:
            #if len(eachLine)>1:
        #x,y = eachLine.split(',')
        #xar.append(int(x))
        #yar.append(int(y))
        ax5.clear()
        ax5.plot(timeB,servidorUdp.flys[0].temps)
        ax5.set_title('Posicao X')
        ax5.set(xlabel='Tempo (s)',ylabel = 'Posicao X')
        ax6.clear()
        ax6.plot(timeB,servidorUdp.flys[1].temps)
        ax6.set_title('Posicao Y')
        ax6.set(xlabel='Tempo (s)',ylabel = 'Posicao Y')
        ax7.clear()
        ax7.plot(timeB,servidorUdp.flys[0].temps)
        ax7.set_title('Posicao Z')
        ax7.set(xlabel='Tempo (s)',ylabel = 'Posicao Z')
        ax8.clear()
        ax8.plot(timeB,servidorUdp.flys[1].temps)
        ax8.set_title('Velocidade V')
        ax8.set(xlabel='Tempo (s)',ylabel = 'Velocidade V')
        ax9.clear()
        ax9.plot(timeB,servidorUdp.flys[0].temps)
        ax9.set_title('Velocidade T')
        ax9.set(xlabel='Tempo (s)',ylabel = 'Velocidade T')
    def animate3(i):
        ax10.clear()
        ax10.plot(timeB,servidorUdp.flys[0].temps)
        ax10.set_title('Posicao X')
        ax10.set(xlabel='Tempo (s)',ylabel = 'Posicao X')
        ax11.clear()
        ax11.plot(timeB,servidorUdp.flys[1].temps)
        ax11.set_title('Posicao Y')
        ax11.set(xlabel='Tempo (s)',ylabel = 'Posicao Y')
        ax12.clear()
        ax12.plot(timeB,servidorUdp.flys[0].temps)
        ax12.set_title('Posicao Z')
        ax12.set(xlabel='Tempo (s)',ylabel = 'Posicao Z')
        ax13.clear()
        ax13.plot(timeB,servidorUdp.flys[1].temps)
        ax13.set_title('Velocidade V')
        ax13.set(xlabel='Tempo (s)',ylabel = 'Velocidade V')
        ax14.clear()
        ax14.plot(timeB,servidorUdp.flys[0].temps)
        ax14.set_title('Velocidade T')
        ax14.set(xlabel='Tempo (s)',ylabel = 'Velocidade T')
    ani1 = animation.FuncAnimation(fig, animate1, interval=1000)

    ani2 = animation.FuncAnimation(fig2, animate2, interval=1000)

    ani3 = animation.FuncAnimation(fig3, animate3, interval=1000)
    plt.show()

    '''
    #data = numpy.random.rand(100)
    #plt.show()
    fig = plt.gcf()
    #plt.interactive(True)
    #plt.clf()
    ax1 = fig.add_subplot(331)
    plt.title('Sensor de Temperatura 1')
    #ax1.plot(timeB,tempS1)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    #ax1.axis([timeB[0], timeB[9], 20, 40])
    #plt.figure(2)
    ax2 = fig.add_subplot(332)
    plt.title('Sensor de Temperatura 2')
    #ax2.plot(timeB,tempS2)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(3)
    ax3 = fig.add_subplot(333)
    plt.title('Sensor de Temperatura 3')
    #ax3.plot(timeB,tempS3)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(4)
    ax4 = fig.add_subplot(334)
    plt.title('Sensor de Temperatura 4')
    #ax4.plot(timeB,tempS4)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    print ('asd')
    plt.axis([timeB[0], timeB[49], 20, 40])
    print ('sfd')
    #plt.figure(5)
    ax5 = fig.add_subplot(335)
    plt.title('Velocidade V/t')
    #ax5.plot(timeB,speedSV)
    plt.xlabel('tempo(s)')
    plt.ylabel('VelV')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(6)
    print ('zas')
    ax6 = fig.add_subplot(336)
    plt.title('Velocidade T/t')
    #ax6.plot(timeB,speedSO)
    plt.xlabel('tempo(s)')
    plt.ylabel('VelT')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(7)
    ax7 = fig.add_subplot(337)
    plt.title('PoseX/t')
    #ax7.plot(timeB,poseSX)
    plt.xlabel('tempo(s)')
    plt.ylabel('PoseX')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(8)
    ax8 = fig.add_subplot(338)
    plt.title('PoseY/t')
    #ax8.plot(timeB,poseSY)
    plt.xlabel('tempo(s)')
    plt.ylabel('PoseY')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    #plt.figure(9)
    ax9 = fig.add_subplot(339)
    plt.title('PoseT/t')
    #ax9.plot(timeB,poseST)
    plt.xlabel('tempo(s)')
    plt.ylabel('PoseT')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[49], 20, 40])
    plt.tight_layout()
    #fig.axis.clear()

    while(1):
        print ('entratemps')
        servidorUdp.mostraTemps()

        print ('entratemps')
        ax1.plot(timeB,servidorUdp.flys[1].temps)
        ax1.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax2.plot(timeB,servidorUdp.flys[1].temps)
        ax2.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax3.plot(timeB,servidorUdp.flys[0].temps)
        ax3.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax4.plot(timeB,servidorUdp.flys[0].temps)
        ax4.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax5.plot(timeB,servidorUdp.flys[0].temps)
        ax5.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax6.plot(timeB,servidorUdp.flys[0].temps)
        ax6.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax7.plot(timeB,servidorUdp.flys[0].temps)
        ax7.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax8.plot(timeB,servidorUdp.flys[0].temps)
        ax8.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        #plt.pause(1)

        ax9.plot(timeB,servidorUdp.flys[0].temps)
        ax9.axis([timeB[0], timeB[49], 20, 40])
        fig.canvas.draw()
        plt.pause(1)
        #ax1.axes.lines[0].remove()
        #ax2.axes.lines[0].remove()
        '''
    print('end')


#Receiving Data

    #def receivingStatus():

    #def receivingPosition():

    #def receivingSpeed():



# Code to add widgets will go here...

#Button Commands

def openManual():
    manualW = tkinter.Toplevel()
    manualW.title('Operacao Manual')
    top.resizable(width = 0, height = 0)
    top.withdraw()
    #manualOn=(True)

    def close():
        #manualOn=(False)
        top.deiconify()
        manualW.destroy()

    #Send info to open robot for info

    #Send inputs function

    def input():
        global decoded_bytes
        recievedPoseX = float(e1.get())
        #recievedPoseY = float(e2.get())
        recievedPoseT = float(e3.get())

        #speedV = float(e1.get()) #DEBUGGING
        decoded_bytes = str(e2.get())

        print('adsdasd %s' % decoded_bytes)
        #check if float, and size, if True and acceptable send

        print('value a: %s \nvalue b: %s' % (recievedPoseX, recievedPoseT))

    #Manual Window

        #Labels

    iControlL = tkinter.Label (manualW, anchor = 'w', text = "Inserir TODOS os valores de controle:" , padx = 2)
    iPoseX = tkinter.Label (manualW, anchor = 'w', text = "Pose X", padx = 2)
    iPoseY = tkinter.Label (manualW, anchor = 'w', text = "Pose Y", padx = 2)
    iPoseT = tkinter.Label (manualW, anchor = 'w', text = "Pose T", padx = 2)

        #input boxes

    e1 = tkinter.Entry(manualW)
    e2 = tkinter.Entry(manualW)
    e3 = tkinter.Entry(manualW)

        #Button Manual

    send = tkinter.Button(manualW, borderwidth = 3, width = 15, height = 1, text = 'Send', pady = 2 , padx = 3, command = input)


        #Positioning Manual

    iControlL.grid(row = 1, column = 1, pady = 2, padx = 3, sticky = 'W')
    iPoseX.grid(row = 2, column = 1, pady = 2, padx = 3, sticky = 'W')
    iPoseY.grid(row = 3, column = 1, pady = 2, padx = 3, sticky = 'W')
    iPoseT.grid(row = 4, column = 1, pady = 2, padx = 3, sticky = 'W')
    e1.grid(row = 2, column = 2, pady = 2, padx = 3, sticky = 'E')
    e2.grid(row = 3, column = 2, pady = 2, padx = 3, sticky = 'E')
    e3.grid(row = 4, column = 2, pady = 2, padx = 3, sticky = 'E')
    send.grid(row = 5, column = 2, pady = 4, padx = 5, sticky = 'E')


    manualW.protocol("WM_DELETE_WINDOW", close)

#Map

def map():

    #mapW = tkinter.Toplevel()
    #mapW.title('Mapa Temp')
    #top.resizable(width = 0, height = 0)
    #top.withdraw()
    #manualOn=(True)

    #def close():
        #mapOn=(False)
        #top.deiconify()
        #mapW.destroy()

    a = np.random.random((16, 16))
    #a = np.indices(16,16)
    #a[0][0] = 0.30
    a[0][0] = servidorUdp.flys[0].temps[49]
    a[0][15] = 0.50
    #a[0][15] = servidorUdp.flys[1].temps[49]
    a[15][0] = 0.44
    #a[15][0] = servidorUdp.flys[2].temps[49]
    a[15][15] = 0.24
    #a[15][15] = servidorUdp.flys[3].temps[49]
    z=0
    #for z in range (1,14):
    print(a)
    c = a[0][0] - a[15][0]
    c = c/15
    print (c)
    d = a[0][15] - a[15][15]
    d = d/15
    print(d)
    for z in range (1,17):
        b = a[z-1][0] - a[z-1][15]
        b = b/15

        for i in range(1,15):
            #b = a[0][0] - a[0][15]
            #b = b/invz
            a[z-1][i] = a[z-1][i-1]-b
        print('bvalor')
        print(b)
        if z!=16:
            a[z][0] = a[z-1][0]-c
            a[z][15] = a[z-1][15]-d
        print (a)
        print (z)

    plt.imshow(a, cmap='hot', interpolation='catrom')
    plt.show()
    '''
    xvalues = np.array([0, 1, 2, 3, 4, 5, 6, 7])
    yvalues = np.array([0, 1, 2, 3, 4, 5, 6, 7])
    xx, yy = np.meshgrid(xvalues, yvalues)
    #plt.plot(xx, yy, marker='.', color='k', linestyle='none')
    plt.plot(xx, yy)
    plt.show()
    '''
    '''
    distances = distance_2d(1, 2, xs, ys)
    plt.figure()
    plt.title('distance to point (1, 2)')
    plt.imshow(distances, origin='lower', interpolation="none")
    plt.xticks(np.arange(xs.shape[1]), xs.ravel())  # need to set the ticks manually
    plt.yticks(np.arange(ys.shape[0]), ys.ravel())
    plt.colorbar()
    '''
    '''
    side = np.linspace(-2,2,15)
    X,Y = np.meshgrid(side,side)
    Z = np.exp(-((X-1)**2+Y**2))

    # Plot the density map using nearest-neighbor interpolation
    plt.pcolormesh(X,Y,Z)
    plt.show()
    '''


#########################//#########################//####################//#####################//########################//###############
#Root Commands

thread = threading.Thread(target=updater)
thread.daemon = True                            # Daemonize thread
thread.start()

threadTemp =threading.Thread(target=servidorUdp.iniciaLeitura,args=())
#thread.daemon=False
threadTemp.start()
'''
thread = threading.Thread(target=serialUpdater)
thread.daemon = True                            # Daemonize thread
thread.start()
'''
    #Buttons

automaticB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, relief = 'sunken', text = 'AUTO', pady = 2 , padx = 3, )
manualB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, text = 'MANUAL', pady = 2 , padx = 3, command = openManual)
graphB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, text = 'Graphs', pady = 2 , padx = 3, command = graph)
mapB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, text = 'Map', pady = 2 , padx = 3, command = map)

    #Frames

        #Main

box1 = tkinter.Frame(top, borderwidth = 2, width = 300, height = 400, relief = 'sunken', pady = 2, padx = 3)
box2 = tkinter.Frame(top, borderwidth = 2, width = 250, height = 300, relief = 'sunken', pady = 2, padx = 3)

            #Box1

robotDataBox = tkinter.Frame(box1, borderwidth = 2 , width = 250, height = 140 , relief = 'ridge', pady = 2, padx =3)
robotDataBox.grid_propagate(1)
tempDataBox = tkinter.Frame(box1, borderwidth = 2 , width = 250, height = 95 , relief = 'ridge', pady = 2, padx =3)
tempDataBox.grid_propagate(1)

            #Box2

photoF = tkinter.Label(box2, width = 300, height = 300, image = photo )


    #Labels

        #Main

            #Box1
robotStatusL = tkinter.Label (box1, anchor = 'w', text = "Status de Rob:"+'%s' %(robotStatus), padx = 2)
robotData = tkinter.Label (box1,  text = "Dados de Rob", anchor = 'w', padx = 2)
tempData = tkinter.Label(box1, anchor = 'w', text = "Dados de Temperatura", padx = 2)

                #RoboData
speedVL = tkinter.Label(robotDataBox, text = "Robot speed v:")
speedOL = tkinter.Label(robotDataBox, text = "Robot speed o:")
#speedTL = tkinter.Label(robotDataBox, text = "Robot speed T:" )
speedVValue = tkinter.Label(robotDataBox, text = '%s' % (speedV))
speedOValue = tkinter.Label(robotDataBox, text = '%s' % (speedO))
#speedTValue = tkinter.Label(robotDataBox, text = '%s' % (speedT))
poseXL = tkinter.Label(robotDataBox, text = "Robot position x:")
poseYL = tkinter.Label(robotDataBox, text = "Robot position y:")
poseTL = tkinter.Label(robotDataBox, text = "Robot position T:")
poseXValue = tkinter.Label(robotDataBox, text = '%s' % (poseX))
poseYValue = tkinter.Label(robotDataBox, text = '%s' % (poseY))
poseTValue = tkinter.Label(robotDataBox, text = '%s' % (poseT))

                #TempData

temp1L = tkinter.Label(tempDataBox, text = "Temperature 1:")
temp2L = tkinter.Label(tempDataBox, text = "Temperature 2:")
temp3L = tkinter.Label(tempDataBox, text = "Temperature 3:")
temp4L = tkinter.Label(tempDataBox, text = "Temperature 4:")
temp1Value = tkinter.Label(tempDataBox, text = '%s' % (temp1))
temp2Value = tkinter.Label(tempDataBox, text = '%s' % (temp2))
temp3Value = tkinter.Label(tempDataBox, text = '%s' % (temp3))
temp4Value = tkinter.Label(tempDataBox, text = '%s' % (temp4))


    #Positioning

        #Main

box1.grid(row =1, column =1, sticky ='NW', padx = 8, pady = 7, rowspan = 3)
box2.grid(row =1, column =2, sticky ='NE', padx = 8, pady = 7, rowspan = 3)
automaticB.grid(row = 1, column = 3, sticky ='NE', padx = 8, pady = 7, rowspan = 1)
manualB.grid(row = 2, column = 3, sticky ='NE', padx = 8, pady = 7, rowspan = 1)
graphB.grid(row = 3, column = 3, sticky ='NE', padx = 8, pady = 7, rowspan = 1)
mapB.grid(row = 4, column = 3, sticky ='NE', padx = 8, pady = 7, rowspan = 1)

            #box1

robotStatusL.grid(row = 1, column = 1, sticky = 'W', pady = (4,0))
robotData.grid(row = 2, column = 1, sticky = 'W', pady = (4,0))
robotDataBox.grid(row= 3, column = 1, sticky ='W')
tempData.grid(row = 4, column = 1, sticky = 'W', pady = (2,0))
tempDataBox.grid(row = 5, column =1,sticky = 'W')

                #RoboData

speedVL.grid(row = 1, column = 1, sticky ='W')
speedOL.grid(row = 2, column = 1, sticky ='W')
#speedTL.grid(row = 3, column = 1, sticky ='W')
speedVValue.grid(row = 1, column = 2, sticky ='E',padx=(100,0))
speedOValue.grid(row = 2, column = 2, sticky ='E')
#speedTValue.grid(row = 3, column = 2, sticky ='E')
poseXL.grid(row = 4, column = 1, sticky ='W')
poseYL.grid(row = 5, column = 1, sticky ='W')
poseTL.grid(row = 6, column = 1, sticky ='W')
poseXValue.grid(row = 4, column = 2, sticky ='E')
poseYValue.grid(row = 5, column = 2, sticky ='E')
poseTValue.grid(row = 6, column = 2, sticky ='E')

                #TempData

temp1L.grid(row =1, column = 1, sticky = 'W')
temp2L.grid(row =2, column = 1, sticky = 'W')
temp3L.grid(row =3, column = 1, sticky = 'W')
temp4L.grid(row =4, column = 1, sticky = 'W')
temp1Value.grid(row = 1, column = 2, sticky = 'E',padx = (112,0))
temp2Value.grid(row = 2, column = 2, sticky = 'E')
temp3Value.grid(row = 3, column = 2, sticky = 'E')
temp4Value.grid(row = 4, column = 2, sticky = 'E')

            #box2

photoF.grid(row = 1, column = 2, sticky = 'E')





# End of Code to add widgets will go here...




top.mainloop()

###########################################################################################

#End GUI
