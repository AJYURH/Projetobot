/* PROJETO ENGG68 - TOPICOS ESPECIAIS EM ENGENHARIA DE COMPUTACAO
   Distancia entre as rodas: 9.2 cm / Circunferncia relizada: 57.8053 cm
   Diâmetro das rodas: 3,5 cm / Circunferência da roda 10.9957 cm
   Passo tangencial da roda: 0.687cm (0.00687m)
   
   Passo angular: 0.0747 rad
   
   O rob precisa começar na origem (x=0,y=0) theta=0;
   
   Foi adotado como valor máximo para controle PWM dos motores 245 ao invés de 255 na tentativa de preservar seu funcionamento
   A velocidade máxima teórica em cm para essa restrição é de 12 cm/s ao invés de 12.5 cm/s como no manual do robô
   O controle de velocidade está sendo feito nos motores através dos encoders INDIVIDUALMENTE, isto é, ainda não há correção para a rotação
       - acredito precisamos da equação do robô pra isso
       para 7,01cm/s -> 149
       
       buffer [char flag; float x_setpoint; float y_setpoint; float theta_setpoint]
       buffer [FF-XXX.XXX-YYY.YYY-ZZZ.ZZZ] (26 posicoes)
       368-010.000-010.000-001.500 (primeiro quadrante)
       368--10.000-010.000-001.500 (segundo quadrante)
       368-010.000--10.000-001.500 (quarto quadrante)
       368--10.000--10.000-001.500 (terceiro quadrante)

       368-000.100-000.100-000.000 (primeiro quadrante)
       368--00.100-000.100-000.500(segundo quadrante)
       368-000.100--00.100--01.500((quarto quadrante)
       
       
       68-111.111-222.222-333.333
*/

int SP1 = 6; //M1 Speed Control
int SP2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0; 
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;  
  long countR = 0;

float vMax = 0.0701;

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



//*******************************************Corrige a orientação do robô********************************************
float orientacao(float currentTheta, float thetaSP)
{
  float rightspeed=0;
  float leftspeed = 0;
  int rightspeedPWM=0;
  int leftspeedPWM=0;

  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0; 
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;  
  long countR = 0;
 
  Serial.print("Theta do setpoint = "); Serial.println(thetaSP);
   
  float erroTheta = thetaSP - currentTheta;

  Serial.print("O erroTheta é: "); Serial.println(erroTheta);
  
  analogWrite (SP2,rightspeedPWM);
  digitalWrite(M2,LOW);
  analogWrite (SP1,leftspeedPWM);
  digitalWrite(M1,LOW);
  int n=0;
  float currentTheta1=currentTheta;
  if(erroTheta>0){
    while(erroTheta>0.02){
        
            //************************************************Leitura do encoder - MOTOR 2 - RIGHT************************************************
      rawsensorValueR = analogRead(1);
      if (rawsensorValueR < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
        sensorcount1R = 1;
      else 
      sensorcount1R = 0;
      if (sensorcount1R != sensorcount0R)
        countR ++;
      sensorcount0R = sensorcount1R;

      currentTheta= countR*0.043; 
      if(n==0){
        currentTheta = currentTheta1+currentTheta;
        n=1;
      }
 
      erroTheta = thetaSP - currentTheta;
      Serial.println(erroTheta);
      
      rightspeedPWM = 100;
      leftspeed = 0;
      analogWrite (SP2,rightspeedPWM);
      digitalWrite(M2,LOW);
      }
       
  }
  n=0;
    if(erroTheta<0){
      while(erroTheta<(-0.02)){       
        Serial.println("Entrei no while do erro menor que zero"); 
        rawsensorValueL = analogRead(0);
        if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
          sensorcount1L = 1;
        else 
        sensorcount1L = 0;
        if (sensorcount1L != sensorcount0L)
          countL ++;
        sensorcount0L = sensorcount1L;
      
      Serial.println(erroTheta);
      currentTheta = currentTheta - countL*0.043;
      if(n==0){
        currentTheta = currentTheta1+currentTheta;
        n=1;
      }
      erroTheta = thetaSP - currentTheta;
      Serial.println(erroTheta);
      
      leftspeedPWM = 100;

      analogWrite (SP1,leftspeedPWM);
      digitalWrite(M1,LOW);
      }
       
  }
  analogWrite (SP1,0);
  digitalWrite(M1,LOW);
  analogWrite (SP2,0);
  digitalWrite(M2,LOW);
   if(currentTheta>PI)
     currentTheta = currentTheta - (2*PI);
     
   if (currentTheta < -(2*PI))
     currentTheta = currentTheta+(2*PI);
   
   return currentTheta;  
      
}



// ********************************************************************Controle Proporcional e Integrativo*************************************************************
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
 
  while(erro>0.01)
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
  //*************************************Controle Proporcional Integral de velocidade*****************************************************
  currentPositionR = countR*0.00687;
  currentPositionL = countL*0.00687;
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
   float w = (leftspeed-rightspeed)/0.092; //9.2 cm é a distância entre as rodas
    
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
 
 char comando = 0;
 char buffer_entrada[27];
 int flag[2];
 char in_char;
 
 float currentPosition[2]; currentPosition[0]=0;currentPosition[1]=0; 
 float currentTheta=0;
 float setpoint[2];
 float setpointFoward =0;
 float currentPositionFoward = 0;
      
  if(Serial.available() > 0 )
   {
 
     comando = Serial.read();
     if(comando=='1'){
      Serial.print("Comando 1 \n");
      
      setpoint[0]=10; setpoint[1]=20;
      currentPosition[2]; currentPosition[0]=0; currentPosition[1]=0;
      
      setpointFoward = sqrt(pow(setpoint[0],2) + pow(setpoint[1],2)); //calcula a hipotenusa
      currentPositionFoward = 0; //RETIRAR ISSO DAQUI, SÓ PARA DEBUG
   
      currentPositionFoward = controlP(currentPositionFoward,setpointFoward);
      Serial.print("Chegamos ao Ponto 1! CurrentPosition = "); Serial.println(currentPositionFoward);
      delay(50);

     }
     
      if(comando=='2'){
        Serial.print("Comando 2 \n");
        int leftspeed = 240;
        int rightspeed = 240;
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
        
        String buffer_in = String(27);
        int flag;
        float x_setpoint;
        float y_setpoint;
        float theta_final;
        
        Serial.print("Comando 3. Insira o buffer \n");
        delay(100);
              
    while( !Serial.available() ){}

    if(Serial.available()>0){ 
    buffer_in = Serial.readString();
  
  
    flag = buffer_in.substring(0,2).toInt();
    x_setpoint = buffer_in.substring(3,10).toFloat();
    y_setpoint = buffer_in.substring(11,18).toFloat();
    theta_final = buffer_in.substring(19,26).toFloat();


    float setpoint[2]; setpoint[0]=x_setpoint; setpoint[1]=y_setpoint;
    float thetaSP = atan2(setpoint[1],setpoint[0]);
    float currentTheta1;
    
    if(flag == 68 ){
        Serial.println("deu 68");
    }
    
    Serial.print("Flag: "+String(flag));
    Serial.print(" x_setpoint: "+ String(x_setpoint));
    Serial.print(" y_setpoint: "+ String(y_setpoint));
    Serial.println("theta_final: "+ String(theta_final));
          
  
        // Serial.print("O resultado da conversao do setpoint (x, y) e: "); Serial.print(setpoint[0]);Serial.print(", ");Serial.println(setpoint[1]);
         setpointFoward = sqrt(pow(x_setpoint,2) + pow(y_setpoint,2)); //calcula a hipotenusa
         Serial.print("A hipotenusa: "); Serial.println(setpointFoward);

         currentTheta1 = orientacao(currentTheta, thetaSP);
         Serial.print("Primeira etapa de orientação ok. currentTheta = "); Serial.println(currentTheta1);
         currentTheta = currentTheta1;         
         
         currentPositionFoward = controlP(currentPositionFoward,setpointFoward);
         Serial.print("Chegada ao setpointFoward. currentPositionFoward = "); Serial.println(currentPositionFoward);

         currentTheta = orientacao(currentTheta1, theta_final);
         Serial.print("Segunda etapa de orientação ok. currentTheta = "); Serial.println(currentTheta);
            
         currentPosition[0] = currentPosition[0] + currentPositionFoward*cos(currentTheta1);
         currentPosition[1] = currentPosition[1] + currentPositionFoward*sin(currentTheta1);
         Serial.print("Passageiros do voo ENGG68, chegamos ao destino (x,y,theta): "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentTheta);
          
        }
        
      }
      }
     
     
   }
