import socket
import select
import time
import numpy


NUM_FLYPORTS = 4

#HOST = '192.168.1.2'
HOST2 = 'localhost'
#HOST = '192.168.43.47'
PORT2 = 5556
#PORT = 5555
DATASIZE = 50



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



#Listas de objetos flY e IPS
flys = []
clientesIPs = []

#um socket para cada clienteFlyport
'''
server_addr = (HOST, PORT)
server_socket  = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server_socket.bind(server_addr)
'''

server_addr2 = (HOST2, PORT2)
server_socket2  = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
server_socket2.bind(server_addr2)

def atualizaTemperatura(cliente,msg_temp):
    mostraTemps()

    #temp  = float(msg_temp[3:10])
    temp = float(msg_temp)
    for fly in flys:
        if(fly.statusRun ==True and fly.ip ==  cliente):
            fly.temps = numpy.append(fly.temps[1:DATASIZE],temp) 
            #print("print input")
            #print (fly.temps)
#            fly.temps.append(temp)
#            if(fly.temps.__len__()>10):
#                fly.temps.pop(0)
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
    print("Inicando Leitura Temperaturas")
    for i in range(NUM_FLYPORTS):
        flys.append(Flyport(i))


    while True:
        '''
        msg, cliente = server_socket2.recvfrom(1024)
        print (msg)
        if cliente[0] not in clientesIPs:
            clientesIPs.append(cliente[0])
        atualizaTemperatura(cliente[0], msg)
        print (clientesIPs)
        '''
        msg2, cliente = server_socket2.recvfrom(1024)
        print (msg2)
        if cliente[0] not in clientesIPs:
            clientesIPs.append(cliente[0])
        atualizaTemperatura(cliente[0], msg2)
        print (clientesIPs)

        #print (cliente[0], msg)


    server_socket2.close()


if __name__ == "__main__":
    iniciaLeitura()