/* PROJETO ENGG68 - TOPICOS ESPECIAIS EM ENGENHARIA DE COMPUTACAO
   Foi adotado como valor máximo para controle PWM dos motores 245 ao invés de 255 na tentativa de preservar seu funcionamento
   A velocidade máxima teórica em cm para essa restrição é de 12 cm/s ao invés de 12.5 cm/s como no manual do robô
   O controle de velocidade está sendo feito nos motores através dos encoders INDIVIDUALMENTE, isto é, ainda não há correção para a rotação
       - acredito precisamos da equação do robô pra isso
       para 7,01cm/s -> 149
*/

int SP1 = 6; //M1 Speed Control
int SP2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

float vMax = 7.01;

void setup()
{
  //setar os pinos como saída
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

float orientacao(float currentPosition[2], float setpoint[2])
{
  float theta = atan2(setpoint[0],setpoint[1]);
  
  
}

float controlP(float currentPosition, float setpoint) 
{
  // ***************************Variáveis dos encoders **********************
  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0; 
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;  
  long countR = 0;
  
  // ******************************Variáveis de Controle ************************
  float currentPositionR = currentPosition;
  float currentPositionL = currentPosition;
  float erro = setpoint - currentPosition;
  int difW = 0;
  if (erro<0)
   erro = -erro;
  float erroR = erro;
  float erroL = erro;
  float erroAnterior = 0;
  float I=0;
  
  float kp = 0.08;
  float ki = 0.008;
 
  while(erro>0.2)
  {
    //************************************************Leitura do encoder - MOTOR 2 - RIGHT************************************************
  rawsensorValueR = analogRead(0);
  if (rawsensorValueR < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
    sensorcount1R = 1;
  else 
  sensorcount1R = 0;
  if (sensorcount1R != sensorcount0R)
    countR ++;
  sensorcount0R = sensorcount1R;
    //************************************************Leitura do encoder - MOTOR 1 - LEFT************************************************
  rawsensorValueL = analogRead(1);
  if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
    sensorcount1L = 1;
  else 
  sensorcount1L = 0;
  if (sensorcount1L != sensorcount0L)
    countL ++;
  sensorcount0L = sensorcount1L;
  //*************************************Controle Proporcional Integral de velocidade*****************************************************
  currentPositionR = countR*3.14159*0.32;
  currentPositionL = countL*3.14159*0.32;
  difW = countL - countR;

  erroR = setpoint - currentPositionR;
  erroL = setpoint - currentPositionL;
  erro = (erroR + erroL)/2;
  if (erroR<0)
   erroR = -erroR;
  if (erroL<0)
   erroL = -erroL;
   
   I = I+erro;
   
    //******************************************Controle de velocidade máxima******************************************************
  float rightspeed = (erro * kp) + (I * ki);
  if(rightspeed>vMax)
    rightspeed = vMax;
   float leftspeed = (erro * kp) + (I * ki);
  if(leftspeed>vMax)
    leftspeed = vMax;
 
 //*************************************************Controle de direção*********************************************************
  if (difW>0)
    leftspeed = leftspeed-(difW/2);
  else if (difW<0)
    rightspeed = rightspeed+(difW/2);
    
  // ******************************Controle para não mandar "velocidade negativa" pro PWM******************************************
  if(rightspeed<0)
    rightspeed = 0;
  if(leftspeed<0)
    leftspeed = 0;

   float speedFoward = (leftspeed+rightspeed)/2;
   float w = (leftspeed-rightspeed)/9.2; //9.2 é a distância entre as rodas
    
  int rightspeedPWM = 149*rightspeed/vMax;
  int leftspeedPWM = 149*leftspeed/vMax;
  analogWrite (SP2,rightspeedPWM);
  digitalWrite(M2,LOW);
  delay(5);
  analogWrite (SP1,leftspeedPWM);
  digitalWrite(M1,LOW);
  Serial.print(countL); Serial.print(" ");Serial.print(countR); Serial.print(" "); Serial.print(currentPosition); Serial.print(" "); Serial.print(erro); Serial.print(" ");Serial.print(leftspeedPWM);Serial.print(" "); Serial.println(rightspeedPWM);
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
      float x = 10;
      float y = 15;
      float setpoint = sqrt(pow(x,2) + pow(y,2)); //calcula a hipotenusa
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
      if(comando=='3'){
        Serial.print("Comando 0 \n");
        int leftspeed = 100;
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
