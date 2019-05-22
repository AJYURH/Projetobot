/* PROJETO ENGG68 - TOPICOS ESPECIAIS EM ENGENHARIA DE COMPUTACAO
   Foi adotado como valor máximo para controle PWM dos motores 245 ao invés de 255 na tentativa de preservar seu funcionamento
   A velocidade máxima teórica em cm para essa restrição é de 12 cm/s ao invés de 12.5 cm/s como no manual do robô
   O controle de velocidade está sendo feito nos motores através dos encoders INDIVIDUALMENTE, isto é, ainda não há correção para a rotação
       - acredito precisamos da equação do robô pra isso
*/

int SP1 = 5; //M1 Speed Control
int SP2 = 6; //M2 Speed Control
int M1 = 7; //M1 Direction Control
int M2 = 8; //M2 Direction Control

int rawsensorValueR = 0; // variable to store the value coming from the sensor
int sensorcount0R = 0;
int sensorcount1R = 0;
long countR = 0;
int rawsensorValueL = 0; // variable to store the value coming from the sensor
int sensorcount0L = 0;
int sensorcount1L = 0;
long countL = 0;


  //setar os pinos como saída
void setup()
{
int i;
for(i=5;i<=8;i++)
pinMode(i, OUTPUT);
Serial.begin(9600);

 int leftspeed = 0; //255 is maximum speed
 int rightspeed = 0;
 analogWrite (SP1,leftspeed);
 digitalWrite(M1,LOW);
 analogWrite (SP2,rightspeed);
 digitalWrite(M2,LOW);
 delay(100);
}

float controlP(float currentPosition, float setpoint) 
{       
 float currentPositionR = currentPosition;
 float currentPositionL = currentPosition;
 float erro = setpoint - currentPosition;
 if (erro<0)
   erro = -erro;
 float erroR = erro;
 float erroL = erro;

 int kp = 2;
 
  while(erro>0.2)
  {
    //************************************************Leitura do encoder - MOTOR 2 - RIGHT************************************************
  rawsensorValueR = analogRead(1);
  if (rawsensorValueR < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
    sensorcount1R = 1;
  else 
  sensorcount1R = 0;
  if (sensorcount1R != sensorcount0R)
    countR ++;
  sensorcount0R = sensorcount1R;
    //************************************************Leitura do encoder - MOTOR 1 - LEFT************************************************
  rawsensorValueL = analogRead(0);
  if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
    sensorcount1L = 1;
  else 
  sensorcount1L = 0;
  if (sensorcount1L != sensorcount0L)
    countL ++;
  sensorcount0L = sensorcount1L;
  //*************************************Controle Proporcional de velocidade*****************************************************
  currentPositionR = countR*3.14159*0.32;
  currentPositionL = countL*3.14159*0.32;

  erroR = setpoint - currentPositionR;
  erroL = setpoint - currentPositionL;
  erro = erroR + erroL;
  if (erroR<0)
   erroR = -erroR;
  if (erroL<0)
   erroL = -erroL;
  float rightspeed = erroR * kp;
  if(rightspeed>12)
    rightspeed = 12;
   float leftspeed = erroL * kp;
  if(leftspeed>12)
    leftspeed = 12;

   float speedFoward = (leftspeed+rightspeed)/2;
   float w = (leftspeed-rightspeed)/9.2; //9.2 é a distância entre as rodas
    
  int rightspeedPWM = 245*rightspeed/12;
  int leftspeedPWM = 245*leftspeed/12;
  analogWrite (SP2,rightspeedPWM);
  digitalWrite(M2,LOW);
  delay(5); // ALTERAR
  analogWrite (SP1,leftspeedPWM);
  digitalWrite(M1,LOW);
  Serial.print(countR); Serial.print(" ");Serial.print(countL); Serial.print(" "); Serial.print(currentPosition); Serial.print(" "); Serial.print(erro); Serial.print(" "); Serial.println(rightspeedPWM);
  }
        analogWrite (SP1,0);
        digitalWrite(M1,LOW);
        analogWrite (SP2,0);
        digitalWrite(M2,LOW);
        currentPosition = (currentPositionR+currentPositionL)/2;
        countR = 0;
        countL = 0;
        return currentPosition;
}

void loop()
{
 int fowardspeed = 0;
 int comando = 0;
 bool flag = 0; //flag de movimentacao
 float currentPosition = 0;
      
  if(Serial.available() > 0 )
   { flag=1;
     comando = Serial.read();
     if(comando=='1'){
      Serial.print("Comando 1 \n");
      float setpoint = 30; //50 cm a frente
      currentPosition = 0; //RETIRAR ISSO DAQUI, SÓ PARA DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
      currentPosition = controlP(currentPosition,setpoint);
      Serial.print("Chegamos ao Ponto 1! CurrentPosition = "); Serial.println(currentPosition);
      delay(50);

     }
     
      if(comando=='2'){
        Serial.print("Comando 2 \n");
        int leftspeed = 100;
        int rightspeed = 100;
        analogWrite (SP1,leftspeed);
        digitalWrite(M1,LOW);
        analogWrite (SP2,rightspeed);
        digitalWrite(M2,LOW);
        delay(100);
     }
     
      if(comando=='0'){
        Serial.print("Comando 0 \n");
        int leftspeed = 0;
        int rightspeed = 0;
        analogWrite (SP1,leftspeed);
        digitalWrite(M1,LOW);
        analogWrite (SP2,rightspeed);
        digitalWrite(M2,LOW);
        delay(100);
      }
     
     flag = 0;
   }
}
