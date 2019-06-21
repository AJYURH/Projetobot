#!/usr/bin/env python
import Tkinter as tkinter
#from Tkinter import *
import matplotlib.pyplot as plt
import PIL.Image
import PIL
import PIL.ImageTk
import time
import threading
import serial
from socket import *




#GUI Root Window

top = tkinter.Tk()
top.title("Projetobo")
top.resizable(width = 0, height = 0)

#GUIvariables

decoded_bytes = 'a'

#LIMPAR VARIAVEIS GLOBAIS

robotStatus = ''
speedX = 0.0
speedY = 0.0
speedT = 0.0
recievedSpeedX = 0.0
recievedSpeedY = 0.0
recievedSpeedT = 0.0
poseX = 0.0
poseY = 0.0
poseT = 0.0
temp1 = 0.0
temp2 = 0.0
temp3 = 0.0
temp4 = 0.0
startSeconds = time.time()
currentSeconds = time.time()
timeB = [4,7,5,2,0,0,0,0,0,0]
tempS1 = [20,24,38,39,30,32,37,21,22,29]
tempS2 = [0,2,4,9,0,0,0,0,0,0]
tempS3 = [0,2,4,9,0,0,0,0,0,0]
tempS4 = [0,2,4,9,0,0,0,0,0,0]
photo = PIL.ImageTk.PhotoImage(PIL.Image.open('rob.jpg'))
manualOn = (False)

#Serial COM Variables

receivingStatusF = (False)
receivingSpeedF = (False)
receivingPositionF = (False)


#Begin GUI

##################################################################################################################################


#updater


def updater():

    global decoded_bytes

    #speedY = 0.0
    robotStatus = ''
    speedX = 0.0
    speedY = 0.0
    speedT = 0.0
    poseX = 0.0
    poseY = 0.0
    poseT = 0.0
    temp1 = 0.0
    temp2 = 0.0
    temp3 = 0.0
    temp4 = 0.0
    while(1):
        currentSeconds = time.time()
        time.sleep(1)
        for i in range(9):
            timeB[i]=timeB[i+1]
        timeB[9] = int (currentSeconds-startSeconds)
        print(timeB)
        #speedX = timeB[9]+990
        temp1 = timeB[9]+990


        #Begin Temp

        ############################################################

        serverSocket = socket(AF_INET, SOCK_DGRAM)
        portAdress = ('192.168.043.033', 5555)
        serverSocket.bind(portAdress)
        print('waiting')
        tempvalues = serverSocket.recvfrom(1024)
        print(tempvalues)
        ############################################################

        #End Temp


        #Beguin Serial COM

        #################################################################################################################################


        #UNCOMENT

        #ser = serial.Serial('/dev/ttyUSB0') #set correct port
        #ser.boudrate = 9600
        #ser.flushInput()

        #UNCOMENT ABOVE


        try:
            #ser_bytes = ser.readline() #UNCOMENT]
            #decoded_bytes = str(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))  #UNCOMENT
            print(decoded_bytes)
            #decoded_bytes = str('1-684.125-688.111-879.548-215.358-511.010-152.548')

            if decoded_bytes[0] == '0':
                robotStatus = 'Aguardando'
            elif decoded_bytes[0] == '1':
                print(decoded_bytes)
                robotStatus = 'Movimentando'
                poseX = float(decoded_bytes[2:9])
                poseY = float(decoded_bytes[10:17])
                poseT = float(decoded_bytes[18:25])
                speedX = float(decoded_bytes[26:33])
                speedY = float(decoded_bytes[34:41])
            elif decoded_bytes[0] == '2':
                robotStatus = 'Rotacionando'
                poseX = float(decoded_bytes[2:9])
                poseY = float(decoded_bytes[10:17])
                poseT = float(decoded_bytes[18:25])
                speedT = float(decoded_bytes[42:49])
            else:
                print('Invalid Command')

                '''
                #FOR FREEEEEEEEEEE CONTROOOOOOOOOL! =DDDD

                poseX = float(decoded_bytes[2:8])
                poseY = float(decoded_bytes[10:16])
                poseT = float(decoded_bytes[18:24])
                speedX = float(decoded_bytes[26:32])
                speedY = float(decoded_bytes[34:40])
                speedT = float(decoded_bytes[42:48])
                '''

        except:
            print('No Serial')

    ###########################################################################################

    #End Serial COM


        #Update Robot Data

        #decoded_bytes = ('1-325.548')
        #speedX = float(decoded_bytes[2]+decoded_bytes[3]+decoded_bytes[4]+decoded_bytes[5]+decoded_bytes[6]+decoded_bytes[7]+decoded_bytes[8])
        #print(speedX)
        speedXValue = tkinter.Label(robotDataBox, text = '%s' % (speedX))
        speedYValue = tkinter.Label(robotDataBox, text = '%s' % (speedY))
        speedTValue = tkinter.Label(robotDataBox, text = '%s' % (speedT))

        #Update Temp Data

        temp1Value = tkinter.Label(tempDataBox, text = '%s' % (temp1))
        temp2Value = tkinter.Label(tempDataBox, text = '%s' % (temp2))
        temp3Value = tkinter.Label(tempDataBox, text = '%s' % (temp3))
        temp4Value = tkinter.Label(tempDataBox, text = '%s' % (temp4))

        #Speed Updated Robot Data

        speedXValue.grid(row = 1, column = 2, sticky ='E',padx=(0,0))
        speedYValue.grid(row = 2, column = 2, sticky ='E')
        speedTValue.grid(row = 3, column = 2, sticky ='E')

        #Speed Updated Temp Data

        temp1Value.grid(row = 1, column = 2, sticky = 'E',padx = (0,0))
        temp2Value.grid(row = 2, column = 2, sticky = 'E')
        temp3Value.grid(row = 3, column = 2, sticky = 'E')
        temp4Value.grid(row = 4, column = 2, sticky = 'E')


#Graph

def graph():

    #data = numpy.random.rand(100)
    #plt.show()
    fig = plt.gcf()
    #plt.interactive(True)
    #plt.clf()
    ax1 = fig.add_subplot(221)
    plt.title('Sensor de Temperatura 1')
    #ax1.plot(timeB,tempS1)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    #ax1.axis([timeB[0], timeB[9], 20, 40])
    #plt.figure(2)
    ax2 = fig.add_subplot(222)
    plt.title('Sensor de Temperatura 2')
    ax2.plot(timeB,tempS2)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[9], 20, 40])
    #plt.figure(3)
    ax3 = fig.add_subplot(223)
    plt.title('Sensor de Temperatura 3')
    ax3.plot(timeB,tempS3)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[9], 20, 40])
    #plt.figure(4)
    ax4 = fig.add_subplot(224)
    plt.title('Sensor de Temperatura 4')
    ax4.plot(timeB,tempS4)
    plt.xlabel('tempo(s)')
    plt.ylabel('Temperatura (C)')
    #plt.grid(True)
    plt.axis([timeB[0], timeB[9], 20, 40])
    plt.tight_layout()
    #fig.axis.clear()

    while(1):

        ax1.plot(timeB,tempS1)
        ax1.axis([timeB[0], timeB[9], 20, 40])
        fig.canvas.draw()
        plt.pause(1)
        ax1.axes.lines[0].remove()
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
    manualOn=(True)

    def close():
        manualOn=(False)
        top.deiconify()
        manualW.destroy()

    #Send info to open robot for info

    #Send inputs function

    def input():
        global decoded_bytes
        recievedSpeedX = float(e1.get())
        #recievedSpeedY = float(e2.get())
        recievedSpeedT = float(e3.get())

        speedX = float(e1.get()) #DEBUGGING
        decoded_bytes = str(e2.get())

        print('adsdasd %s' % decoded_bytes)
        #check if float, and size, if True and acceptable send

        print('value a: %s \nvalue b: %s \nvalue c: %s' % (recievedSpeedX, recievedSpeedY, recievedSpeedT))

    #Manual Window

        #Labels

    iControlL = tkinter.Label (manualW, anchor = 'w', text = "Inserir TODOS os valores de controle:" , padx = 2)
    iSpeedX = tkinter.Label (manualW, anchor = 'w', text = "Speed X", padx = 2)
    iSpeedY = tkinter.Label (manualW, anchor = 'w', text = "Speed Y", padx = 2)
    iSpeedT = tkinter.Label (manualW, anchor = 'w', text = "Speed T", padx = 2)

        #input boxes

    e1 = tkinter.Entry(manualW)
    e2 = tkinter.Entry(manualW)
    e3 = tkinter.Entry(manualW)

        #Button Manual

    send = tkinter.Button(manualW, borderwidth = 3, width = 15, height = 1, text = 'Send', pady = 2 , padx = 3, command = input)


        #Positioning Manual

    iControlL.grid(row = 1, column = 1, pady = 2, padx = 3, sticky = 'W')
    iSpeedX.grid(row = 2, column = 1, pady = 2, padx = 3, sticky = 'W')
    iSpeedY.grid(row = 3, column = 1, pady = 2, padx = 3, sticky = 'W')
    iSpeedT.grid(row = 4, column = 1, pady = 2, padx = 3, sticky = 'W')
    e1.grid(row = 2, column = 2, pady = 2, padx = 3, sticky = 'E')
    e2.grid(row = 3, column = 2, pady = 2, padx = 3, sticky = 'E')
    e3.grid(row = 4, column = 2, pady = 2, padx = 3, sticky = 'E')
    send.grid(row = 5, column = 2, pady = 4, padx = 5, sticky = 'E')


    manualW.protocol("WM_DELETE_WINDOW", close)



#########################//#########################//####################//#####################//########################//###############
#Root Commands

thread = threading.Thread(target=updater)
thread.daemon = True                            # Daemonize thread
thread.start()
'''
thread = threading.Thread(target=serialUpdater)
thread.daemon = True                            # Daemonize thread
thread.start()
'''
    #Buttons

automaticB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, relief = 'sunken', text = 'AUTO', pady = 2 , padx = 3, )
manualB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, text = 'MANUAL', pady = 2 , padx = 3, command = openManual)
graphB = tkinter.Button(top, borderwidth = 3, width = 25, height = 5, text = 'Graphs', pady = 2 , padx = 3, command = graph)

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
speedXL = tkinter.Label(robotDataBox, text = "Robot speed x:")
speedYL = tkinter.Label(robotDataBox, text = "Robot speed y:")
speedTL = tkinter.Label(robotDataBox, text = "Robot speed T:" )
speedXValue = tkinter.Label(robotDataBox, text = '%s' % (speedX))
speedYValue = tkinter.Label(robotDataBox, text = '%s' % (speedY))
speedTValue = tkinter.Label(robotDataBox, text = '%s' % (speedT))
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

            #box1

robotStatusL.grid(row = 1, column = 1, sticky = 'W', pady = (4,0))
robotData.grid(row = 2, column = 1, sticky = 'W', pady = (4,0))
robotDataBox.grid(row= 3, column = 1, sticky ='W')
tempData.grid(row = 4, column = 1, sticky = 'W', pady = (2,0))
tempDataBox.grid(row = 5, column =1,sticky = 'W')

                #RoboData

speedXL.grid(row = 1, column = 1, sticky ='W')
speedYL.grid(row = 2, column = 1, sticky ='W')
speedTL.grid(row = 3, column = 1, sticky ='W')
speedXValue.grid(row = 1, column = 2, sticky ='E',padx=(100,0))
speedYValue.grid(row = 2, column = 2, sticky ='E')
speedTValue.grid(row = 3, column = 2, sticky ='E')
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
