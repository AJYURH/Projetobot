import serial
import time

ser = serial.Serial('/dev/ttyUSB0')
ser.boudrate = 9600
print(ser.name)



#buffer [FF-XXX.XXX-YYY.YYY-ZZZ.ZZZ] (26 posicoes)
#      68-010.000-010.000-000.000
#Envia a coordenada






#Le a mensagem
while(True):
    print("Enviando mensagem\n")
    ser.write(b'68-111.111-777.777-888.888')
    time.sleep(3)
    print("Mensagem enviada, aquardando retorno\n")
    s = ser.readline()
    print("Retorno recebido\n")

    print(s)



ser.close()