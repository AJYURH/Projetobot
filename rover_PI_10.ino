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

       0-111.111-222.222-333.333-444.444-555.555 (saída)
       [F-XXX.XXX-YYY.YYY-ZZZ.ZZZ-VVV.VVV-WWW.WWW]
       flag 0 - Aguardando ponto
       flag 1 - Andando
       flag 2 - Girando

       Entrada
       Flag 0 - apenas setpoint
       Flag 1 - apenas speed
       
       68-000.100-000.100-000.000 (primeiro quadrante)
       68--00.100-000.100-000.500(segundo quadrante)
       68--00.100--00.100-000.000(terceiro quadrante)
       68-000.100--00.100--01.500((quarto quadrante)
       
       
       68-111.111-222.222-333.333
*/

bool flag_inicio;

int SP1 = 6; //M1 Speed Control
int SP2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

float currentPosition[2]; 
float currentTheta;
float currentTheta1;
float setpoint[2];
float setpointFoward;
float currentPositionFoward;
float vMax = 7.1;
 
void setup()
{
 //setar os pinos como saída
 int i;
 for(i=5;i<=8;i++)
 pinMode(i, OUTPUT);
 Serial.begin(9600);

 //zerar os motores
 analogWrite (SP1,0);
 digitalWrite(M1,LOW);
 analogWrite (SP2,0);
 digitalWrite(M2,LOW);
 delay(100);

 flag_inicio=0;
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
  Serial.print("Theta do current = "); Serial.println(currentTheta);
   
  float erroTheta = thetaSP - currentTheta;

  Serial.print("O erroTheta é: "); Serial.println(erroTheta);
  
  analogWrite (SP2,rightspeedPWM);
  digitalWrite(M2,LOW);
  analogWrite (SP1,leftspeedPWM);
  digitalWrite(M1,LOW);
 
  float currentTheta1=currentTheta;
 
  if(erroTheta>0){    //Caso a orientação final esteja à esquerda da orientação atual do robô
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

      currentTheta= currentTheta1 + countR*0.043; 
 
      erroTheta = thetaSP - currentTheta;
      Serial.println(erroTheta);
      
      rightspeedPWM = 100;
      analogWrite (SP2,rightspeedPWM);
      digitalWrite(M2,LOW);
      }
       
  }
  
    if(erroTheta<0){  //Caso a orientação final esteja à direita da orientação atual do robô
      while(erroTheta<(-0.02)){       
        rawsensorValueL = analogRead(0);
        if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
          sensorcount1L = 1;
        else 
        sensorcount1L = 0;
        if (sensorcount1L != sensorcount0L)
          countL ++;
        sensorcount0L = sensorcount1L;
      
      Serial.println(erroTheta);
      currentTheta = currentTheta1 - countL*0.043;
      erroTheta = thetaSP - currentTheta;
      Serial.println(erroTheta);
      
      leftspeedPWM = 100;
      analogWrite (SP1,leftspeedPWM);
      digitalWrite(M1,LOW);
      }
       
    }
  //Setar todos os motores para 0
  analogWrite (SP1,0);
  digitalWrite(M1,LOW);
  analogWrite (SP2,0);
  digitalWrite(M2,LOW);

  //Corrige o formato da orientação deixando entre -pi e pi
  if(currentTheta>PI)
    currentTheta = currentTheta - (2*PI);
     
  if (currentTheta < -(2*PI))
    currentTheta = currentTheta+(2*PI);
   
  return currentTheta;  
}
      









// ********************************************************************Controle Proporcional e Integrativo*************************************************************
float controlP(float setpoint) 
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
  float currentPosition=0;
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
  
  float kp = 8;
  float ki = 0.8;
 
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

void set_velocidade(float v, float w){
  
  float leftspeed = 0;
  float rightspeed = 0;
  int leftspeedPWM = 0;
  int rightspeedPWM = 0;

  leftspeed = v - w*(0.092/2);
  rightspeed = v + w*(0.092/2);
  Serial.print(leftspeed); Serial.println(rightspeed);
}

void envia_pacote(int flag, float x, float y, float theta, float v, float w){
  
   char buffer_total[41];
   char buffer_x[7];
   char buffer_y[7];
   char buffer_theta[7];
   char buffer_v[7]; 
   char buffer_w[7];
  //dtostrf(floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, charBuf);
  dtostrf(x, 7, 3, buffer_x);
  dtostrf(y, 7, 3, buffer_y);
  dtostrf(theta, 7, 3, buffer_theta);   
  dtostrf(v, 7, 3, buffer_v);
  dtostrf(w, 7, 3, buffer_w);

  for(int j=0; j<7; j++){
    if (buffer_x[j]==' '){
      buffer_x[j] = '0';
   }
  }
  
    Serial.println(buffer_x);
}









void loop() {

 char buffer_entrada[27];
 int flag[2];
 char in_char;

 
 if(flag_inicio==0){
  currentPosition[0]=0;
  currentPosition[1]=0;
  currentTheta=0;
  currentTheta1=0;
  currentPositionFoward = 0;
  
 }
      
  if(Serial.available() > 0 )
   {
    String buffer_in = String(27);
    char buffer_out[41];
    String buffer_out_1 = String(41);
    int flag;
    float x_setpoint;
    float y_setpoint;
    float theta_final;
    flag_inicio=1;
    
    Serial.print("flag_inicio: "); Serial.println(flag_inicio);
                      
    if(Serial.available()>0){ 
    buffer_in = Serial.readString();
    
    flag = buffer_in.substring(0,2).toInt();
    x_setpoint = buffer_in.substring(3,10).toFloat();
    y_setpoint = buffer_in.substring(11,18).toFloat();
    theta_final = buffer_in.substring(19,26).toFloat();
    setpoint[0]=x_setpoint; setpoint[1]=y_setpoint;

    float er_x = abs(setpoint[0] - currentPosition[0]);
    float er_y = abs(setpoint[1] - currentPosition[1]);
    float er_t = abs(theta_final - currentTheta);
    
    float thetaSP = atan2(setpoint[1]-currentPosition[1],setpoint[0]-currentPosition[0]);     
  
    
      Serial.print("currentTheta antes da correçao:"); Serial.println(currentTheta);
      if(er_x > 0.02 || er_y > 0.02)
        currentTheta = orientacao(currentTheta1, thetaSP);
     
      currentTheta1 = currentTheta;
      Serial.print("Primeira etapa de orientação ok. currentTheta = "); Serial.println(currentTheta);
      Serial.print("flag_inicio2: "); Serial.println(flag_inicio);
    
      setpointFoward = sqrt(pow(x_setpoint-currentPosition[0],2) + pow(y_setpoint-currentPosition[1],2)); //calcula a hipotenusa
      Serial.print("A hipotenusa: "); Serial.println(setpointFoward);
      if(er_x > 0.02 || er_y > 0.02)
        currentPositionFoward = controlP(setpointFoward);
      else currentPositionFoward = 0;
      Serial.print("Chegada ao setpointFoward. currentPositionFoward = "); Serial.println(currentPositionFoward);
    

    currentPosition[0] = currentPosition[0] + currentPositionFoward*cos(currentTheta1);
    currentPosition[1] = currentPosition[1] + currentPositionFoward*sin(currentTheta1);

    Serial.print("Quase lá: (x,y,theta): "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[1]); Serial.print(", "); Serial.print(currentTheta);
    
    currentTheta = orientacao(currentTheta1, theta_final);
    currentTheta1=currentTheta;

    Serial.print("Passageiros do voo ENGG68, chegamos ao destino (x,y,theta): "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[1]); Serial.print(", "); Serial.print(currentTheta);
    Serial.println(" ");

    //envia_pacote(1, currentPosition[0], currentPosition[1], currentTheta, 0, 0);


    
        }
        
      }
 

}
