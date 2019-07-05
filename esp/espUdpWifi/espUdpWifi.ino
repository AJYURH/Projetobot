#include <WiFi.h>
#include <WiFiUdp.h>

//pino analogico de leitura
const int pinoAnalog= 34;
int valorADClido = 0;
float valorConvertido = 0;




const char * nomeRede = "juliete";
const char * passRede = "gameofthrones";
char msg[60];

const char *addr = "192.168.43.33";

int   porta = 5556;

boolean conectado = false;

WiFiUDP udp;


float teste = 1455.22855;
char msgTest[50];



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  connectarWIFI();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(conectado){
    //envia o pacote
    udp.beginPacket(addr,porta);
    sprintf(msg,"F2-%f",valorConvertido);
    //msg = String("Temperatura lida "+valorConvertido)
    udp.printf(msg);
    udp.endPacket();


    //teste do conversão de string:

    sprintf(msgTest,"conversão de string %07.3f",teste);
    
    Serial.println(msgTest);
   



    
  }


  valorADClido = analogRead(pinoAnalog);

  //convert para Celcius:
  
  valorConvertido = ( ((float)valorADClido )* (3.3/2023)/0.01 );

  Serial.print("valor lido e convertido convertido: ");
  Serial.println(valorADClido);
  Serial.println(valorConvertido);
  
  delay(1000);

}

void connectarWIFI(){
  WiFi.disconnect(true);
  WiFi.begin(nomeRede,passRede);
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print("Wifi Conectando...");
  }
  Serial.print("Wifi Conectado IP: ");
  Serial.println(WiFi.localIP());
  udp.begin(WiFi.localIP(),porta);
  conectado = true;

  
}
