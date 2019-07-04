import socket
import select
import time
import numpy


NUM_FLYPORTS = 4

HOST = '192.168.43.47'
PORT = 5555
DATASIZE = 50

STATUS_FILTER = True
WINDOW_LEN = 11
w = numpy.hamming(WINDOW_LEN)





#Listas de objetos flY e IPS
flys = []
clientesIPs = []

#um socket para cada clienteFlyport
server_addr = (HOST, PORT)
server_socket  = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server_socket.bind(server_addr)

class Flyport:
    def __init__(self,id):
        self.statusRun = False
        self.id = id
        self.temps = numpy.zeros(DATASIZE)
        #self.temps = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] 
        
        #self.temps.append(temp)
    def setIpTemp(self,ip,temp):
        self.statusRun = True
        self.ip = ip
        self.temps = numpy.append(self.temps[1:DATASIZE],temp) 
        #self.temps.append(temp)

    def filtraTemps(self):
        s=numpy.r_[self.temps[WINDOW_LEN-1:0:-1],self.temps,self.temps[-2:-WINDOW_LEN-1:-1]]
        j = numpy.convolve(w/w.sum(),s,mode='valid')
        self.temps = j[5:len(j)-5]
        




def atualizaTemperatura(cliente,msg_temp):
    #mostraTemps()

    temp  = float(msg_temp[3:10])
    for fly in flys:
        if(fly.statusRun ==True and fly.ip ==  cliente):
            fly.temps = numpy.append(fly.temps[1:DATASIZE],temp)
            #filtras
            if(STATUS_FILTER):
                fly.filtraTemps()
            

#            
            return
        elif fly.statusRun == False :
            fly.setIpTemp(cliente,temp)
            return

##FF-XXXX

def mostraTemps():
    for fly in flys:
        if fly.statusRun:
            print('FlyPort temp: {} , IP: {}\n'.format(fly.temps,fly.ip))
    print("##################")

def iniciaLeitura():
    #print("Inicando Leitura Temperaturas")
    for i in range(NUM_FLYPORTS):
        flys.append(Flyport(i))


    while True:
        msg, cliente = server_socket.recvfrom(1024)
        if cliente[0] not in clientesIPs:
            clientesIPs.append(cliente[0])
        atualizaTemperatura(cliente[0], msg)

        #print (cliente[0], msg)


    server_socket.close()


if __name__ == "__main__":
    iniciaLeitura()