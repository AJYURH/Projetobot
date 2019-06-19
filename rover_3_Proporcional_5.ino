/* PROJETO ENGG68 - TOPICOS ESPECIAIS EM ENGENHARIA DE COMPUTACAO
   Distancia entre as rodas: 9.2 cm / Circunferncia relizada: 57.8053 cm
   Passo tangencial da roda: 0.32cm
   Passo angular: 0.0348 rad
   
   O rob precisa começar na origem (x=0,y=0) theta=0;
   
   Foi adotado como valor máximo para controle PWM dos motores 245 ao invés de 255 na tentativa de preservar seu funcionamento
   A velocidade máxima teórica em cm para essa restrição é de 12 cm/s ao invés de 12.5 cm/s como no manual do robô
   O controle de velocidade está sendo feito nos motores através dos encoders INDIVIDUALMENTE, isto é, ainda não há correção para a rotação
       - acredito precisamos da equação do robô pra isso
       para 7,01cm/s -> 149
       
       buffer [char flag; float x_setpoint; float y_setpoint; time_t relogio]
       buffer [FF-XXX.XXX-YYY.YYY-ZZZ.ZZZ] (26 posicoes)
       68-010.000-010.000-000.000
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



//*******************************************Corrige a orientação do robô********************************************
float orientacao(float currentPosition[2], float setpoint[2], float currentTheta)
{
  float rightspeed=0;
  float leftspeed = 0;
  float thetaSP = atan2(setpoint[0],setpoint[1]);
  
  int rightspeedPWM=0;
  int leftspeedPWM=0;
  
 // float dot = currentPosition[0]*setpoint[0] + currentPosition[1]*setpoint[1];      //dot product between [x1, y1] and [x2, y2]
//  float det = currentPosition[0]*setpoint[1] - currentPosition[1]*setpoint[0];      //determinant
 // float angle = atan2(det, dot);        // atan2(y, x) or atan2(sin, cos)
  float angle = atan2(setpoint[1],setpoint[0]);
  float erroTheta = angle - currentTheta;

  
  analogWrite (SP2,rightspeedPWM);
  digitalWrite(M2,LOW);
  analogWrite (SP1,leftspeedPWM);
  digitalWrite(M1,LOW);
  
  if(erroTheta>0){
    while(erroTheta>0.1){
        
            //************************************************Leitura do encoder - MOTOR 2 - RIGHT************************************************
      rawsensorValueR = analogRead(0);
      if (rawsensorValueR < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
        sensorcount1R = 1;
      else 
      sensorcount1R = 0;
      if (sensorcount1R != sensorcount0R)
        countR ++;
      sensorcount0R = sensorcount1R;
      
      currentTheta=countR*0.0348;
      erroTheta = angle - currentTheta;
      Serial.println(erroTheta);
      
      rightspeedPWM = 100;
      leftspeed = 0;
      analogWrite (SP2,rightspeedPWM);
      digitalWrite(M2,LOW);
      }
       
  }
  
    if(erroTheta<0){
      while(erroTheta<-0.1){        
        rawsensorValueL = analogRead(1);
        if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
          sensorcount1L = 1;
        else 
        sensorcount1L = 0;
        if (sensorcount1L != sensorcount0L)
          countL ++;
        sensorcount0L = sensorcount1L;
      
      currentTheta=countL*0.0348;
      erroTheta = angle - currentTheta;
      
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
    
/*
  dot = x1*x2 + y1*y2      # dot product between [x1, y1] and [x2, y2]
  det = x1*y2 - y1*x2      # determinant
  angle = atan2(det, dot)  # atan2(y, x) or atan2(sin, cos)
*/
 
  
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
 
 char comando = 0;
 char buffer_entrada[27];
 char flag[0];
 char in_char;
 
 float currentPosition[2];
 float currentTheta;
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
      currentPositionFoward = 0; //RETIRAR ISSO DAQUI, SÓ PARA DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
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
        Serial.print("Comando 3. Insira o buffer \n");
        
        while(!Serial.available() ){
        }
        
      
         int array_int[27];
         int incoming_byte;
         char incoming_char;
         
         
  /*     if(Serial.available()>0){
          for(int i=0; i<=27; i++){
            in_char = Serial.read();
            buffer_entrada[i] = in_char;
            while(!Serial.available() ){}
                  }
          } 
       
    */ 
 
            if(Serial.available()>0){
          for(int i=0; i<=26; i++){
            incoming_char = Serial.read();
            Serial.print(incoming_char);
            array_int[i] = incoming_char;
            while(!Serial.available() ){}
                  }
          } 
          
        Serial.print("O Buffer recebido foi: "); //Serial.println(array_int);
        flag[0]= array_int[0]; flag[1]= array_int[1];//flag[2]='3';
        Serial.print("A flag armazenada e: "); Serial.println(flag);
        char array_transferencia[7];
        
        if(array_int[0]=='6' && array_int[1] =='8'){
         for(int i=0; i<=7;i++)
          array_transferencia[i] = array_int[i+3];
          
         setpoint[0]=atof(array_transferencia);
          
         for(int i =0; i<7; i++)
          array_transferencia[i] = array_int[i+11];
          
         setpoint[1] = atof(array_transferencia);
                   
         Serial.print("O resultado da conversao do setpoint (x, y) e: "); Serial.print(setpoint[0]);Serial.print(", ");Serial.println(setpoint[1]);
         setpointFoward = sqrt(pow(setpoint[0],2) + pow(setpoint[1],2)); //calcula a hipotenusa
         
         //chamar funcao de angulo
         //chamar funcao de deslocamento
         currentTheta = orientacao(currentPosition, setpoint, currentTheta);
         // controlP(float currentPosition, float setpoint) 
         

         currentPosition[0] = currentPosition[0] + currentPositionFoward*cos(currentTheta);
         currentPosition[1] = currentPosition[1] + currentPositionFoward*sin(currentTheta);
         Serial.print("Passageiros do voo ENGG68, chegamos ao destino (x,y,theta): "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentTheta);
          
        }
        

      }
     
     
   }
}
