/* PROJETO ENGG68 - TOPICOS ESPECIAIS EM ENGENHARIA DE COMPUTACAO
   Distancia entre as rodas: 9.2 cm                     / Circunferncia relizada: 57.8053 cm
   Diâmetro das rodas: 3,5 cm                           / Circunferência da roda 10.9957 cm
   Passo tangencial da roda: 0.687cm (0.00687m)         / Passo angular: 0.0747 rad

   O Robô começa na origem (x=0,y=0,theta=0).

   Foi adotado como valor máximo para controle PWM dos motores 143 ao invés de 255 na tentativa de preservar seu funcionamento.
   A velocidade máxima teórica em cm para essa restrição é de 7 cm/s ao invés de 12.5 cm/s como no manual do equipamento.
   As medidas de entrada e saída são tomadas todas no SI (Sistema internacional de medidas)

     BUFFER DE ENTRADA:
       [char flag; float x_setpoint || float vel_linear; float y_setpoint || float vel_angular; float theta_setpoint]
       buffer [F-XXX.XXX-YYY.YYY-ZZZ.ZZZ] (26 posicoes)
       Flag 0 -> Apenas setpoint       Flag 1 -> Apenas velocidades

      BUFFER DE SAÍDA
       [char flag; float pos_x; float pos_y; float pos_theta; float velocidade_linear; float velocidade_angular]
       buffer [F-XXX.XXX-YYY.YYY-ZZZ.ZZZ-VVV.VVV-WWW.WWW]

       Flag 0 - Aguardando ponto        Flag 1 - Andando      Flag 2 - Girando

      Exemplos de entrada:
       0-000.100-000.100-000.500(primeiro quadrante)
       0--00.100-000.100-000.500(segundo quadrante)
       0--00.100--00.100-000.000(terceiro quadrante)
       0-000.100--00.100--01.500((quarto quadrante)
       1-000.050-000.200-000.000(velocidade linear = 0.05, velocidade angular = 0.2)
*/

bool flag_inicio;           //Flag para sinalizar a primeira vez que o programa é rodado

int SP1 = 6;                //M1 Speed Control
int SP2 = 5;                //M2 Speed Control
int M1 = 8;                 //M1 Direction Control
int M2 = 7;                 //M2 Direction Control

float currentPosition[2];   //Posição atual (x,y)
float currentTheta;         //Posição angular atual (theta)
float currentTheta1;
float setpoint[2];          //Variável auxiliar para armazenar setpoint recebido
float setpointFoward;
float currentPositionFoward;
float vMax = 7.01;          //Velocidade máxima em cm/s

void setup()
{
  int i;                     //Setar os pinos dos motores como saída
  for (i = 5; i <= 8; i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600);

  analogWrite (SP1, 0);      //Zerar os motores
  digitalWrite(M1, LOW);
  analogWrite (SP2, 0);
  digitalWrite(M2, LOW);
  delay(100);

  flag_inicio = 0;          //Primeiro loop do programa
}


//*********************************Função que corrige a orientação do robô para um dado setpoint thetaSP***********************************
float orientacao(float currentTheta, float thetaSP)
{
  //Variáveis para controle do motor
  float rightspeed = 0;
  float leftspeed = 0;
  int rightspeedPWM = 0;
  int leftspeedPWM = 0;
  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0;
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;
  long countR = 0;

//  Serial.print("Theta do setpoint = "); Serial.println(thetaSP);
//  Serial.print("Theta do current = "); Serial.println(currentTheta);
  float erroTheta = thetaSP - currentTheta;    //Calcula o erro entre o ângulo final e atual
//  Serial.print("O erroTheta é: "); Serial.println(erroTheta);

  analogWrite (SP2, rightspeedPWM);
  digitalWrite(M2, LOW);
  analogWrite (SP1, leftspeedPWM);
  digitalWrite(M1, LOW);

  float currentTheta1 = currentTheta;

  //*********************************************************************************************************************************************
  if (erroTheta > 0) {                         //Caso a orientação final esteja à esquerda da orientação atual do robô, aciona-se apenas o motor direito
    while (erroTheta > 0.02) {                 //Motor acionado enquanto o erro for maior que 0.02rad

      //Leitura do encoder - MOTOR 2 - RIGHT
      rawsensorValueR = analogRead(1);
      if (rawsensorValueR < 600)              //Min value is 400 and max value is 800, so state chance can be done at 600.
        sensorcount1R = 1;
      else
        sensorcount1R = 0;
      if (sensorcount1R != sensorcount0R)
        countR ++;
      sensorcount0R = sensorcount1R;
      //**********************************

      currentTheta = currentTheta1 + countR * 0.043;

      erroTheta = thetaSP - currentTheta;
 //     Serial.println(erroTheta);

      rightspeedPWM = 100;
      analogWrite (SP2, rightspeedPWM);
      digitalWrite(M2, LOW);
      envia_pacote(2, currentPosition[0], currentPosition[1], currentTheta, 0, 0.02);
      
    }

  }

  if (erroTheta < 0) {                     //Caso a orientação final esteja à direita da orientação atual do robô
    while (erroTheta < (-0.02)) {          //Motor acionado enquanto o erro for menor que -0.02rad
      //Leitura do encoder - MOTOR 1 - LEFT
      rawsensorValueL = analogRead(0);
      if (rawsensorValueL < 600)           //Min value is 400 and max value is 800, so state chance can be done at 600.
        sensorcount1L = 1;
      else
        sensorcount1L = 0;
      if (sensorcount1L != sensorcount0L)
        countL ++;
      sensorcount0L = sensorcount1L;
      //****************************

//      Serial.println(erroTheta);
      currentTheta = currentTheta1 - countL * 0.043;
      erroTheta = thetaSP - currentTheta;
//      Serial.println(erroTheta);

      leftspeedPWM = 100;
      analogWrite (SP1, leftspeedPWM);
      digitalWrite(M1, LOW);
      envia_pacote(2, currentPosition[0], currentPosition[1], currentTheta, 0, -0.02);
    }
  envia_pacote(2, currentPosition[0], currentPosition[1], currentTheta, 0, 0);
  }
  //Setar todos os motores para 0
  analogWrite (SP1, 0);
  digitalWrite(M1, LOW);
  analogWrite (SP2, 0);
  digitalWrite(M2, LOW);

  //Corrige o formato da orientação deixando entre -pi e pi
  if (currentTheta > PI)
    currentTheta = currentTheta - (2 * PI);

  if (currentTheta < -(2 * PI))
    currentTheta = currentTheta + (2 * PI);

  return currentTheta;
}




// *****************************************************Controle Proporcional e Integrativo*************************************************
float controlP(float setpoint)
{
  //Variáveis para controle dos motores***************
  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0;
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;
  long countR = 0;

  // Variáveis de Controle ***************************
  float currentPosition = 0;
  float currentPositionR = currentPosition;
  float currentPositionL = currentPosition;
  float erro = setpoint - currentPosition;
  int difW = 0;

  if (erro < 0)
    erro = -erro;

  float erroR = erro;
  float erroL = erro;
  float erroAnterior = 0;
  float I = 0;

  float kp = 8;       //constante do controle proporcional
  float ki = 0.8;     //constante do controle integrativo

  while (erro > 0.01)
  {
    //Leitura do encoder - MOTOR 2 - RIGHT********************************
    rawsensorValueR = analogRead(1);
    if (rawsensorValueR < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
      sensorcount1R = 1;
    else
      sensorcount1R = 0;
    if (sensorcount1R != sensorcount0R)
      countR ++;
    sensorcount0R = sensorcount1R;
    //Leitura do encoder - MOTOR 1 - LEFT***********************************
    rawsensorValueL = analogRead(0);
    if (rawsensorValueL < 600) //Min value is 400 and max value is 800, so state chance can be done at 600.
      sensorcount1L = 1;
    else
      sensorcount1L = 0;
    if (sensorcount1L != sensorcount0L)
      countL ++;
    sensorcount0L = sensorcount1L;

    //Controle Proporcional Integral de velocidade******************************
    currentPositionR = countR * 0.00687;
    currentPositionL = countL * 0.00687;
    difW = countL - countR;

    erroR = setpoint - currentPositionR;
    erroL = setpoint - currentPositionL;
    erro = (erroR + erroL) / 2;
    if (erroR < 0)
      erroR = -erroR;
    if (erroL < 0)
      erroL = -erroL;

    I = I + erro;

    //Controle de velocidade máxima**********************************************
    float rightspeed = (erro * kp) + (I * ki);
    if (rightspeed > vMax)
      rightspeed = vMax;
    float leftspeed = (erro * kp) + (I * ki);
    if (leftspeed > vMax)
      leftspeed = vMax;

    //Controle de direção*********************************************************
    if (difW > 0)
      leftspeed = leftspeed - (difW / 2);
    else if (difW < 0)
      rightspeed = rightspeed + (difW / 2);

    //Controle para não mandar "velocidade negativa" pro PWM*********************
    if (rightspeed < 0)
      rightspeed = 0;
    if (leftspeed < 0)
      leftspeed = 0;

    float speedFoward = (leftspeed + rightspeed) / 2; //Cálculo da velocidade linear
    float w = (leftspeed - rightspeed) / 0.092;    //Cálculo da velocidade angular (9.2 cm é a distância entre as rodas)

    int rightspeedPWM = 149 * rightspeed / vMax;  //Cálculo do PWM (regra de três simples)
    int leftspeedPWM = 149 * leftspeed / vMax;
    analogWrite (SP2, rightspeedPWM);
    analogWrite (SP1, leftspeedPWM);
    digitalWrite(M2, LOW);
    digitalWrite(M1, LOW);

    envia_pacote(1, currentPositionR, currentPositionL, 0, speedFoward, 0);

//    Serial.print(countL); Serial.print(" "); Serial.print(countR); Serial.print(" "); Serial.print(currentPosition); Serial.print(" "); Serial.print(erro); Serial.print(" "); Serial.print(leftspeedPWM); Serial.print(" "); Serial.println(rightspeedPWM);
  }
  analogWrite (SP1, 0);
  analogWrite (SP2, 0);
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  currentPosition = (currentPositionR + currentPositionL) / 2;
  countR = 0;
  countL = 0;
  return currentPosition;
}





void set_velocidade(float v, float w, float pos[2], float alpha) {

  //Variáveis dos encoders ********************
  int rawsensorValueR = 0;
  int sensorcount0R = 0;
  int sensorcount1R = 0;
  int rawsensorValueL = 0;
  int sensorcount0L = 0;
  int sensorcount1L = 0;
  long countL = 0;
  long countR = 0;
  float leftspeed = 0;
  float rightspeed = 0;
  int leftspeedPWM = 0;
  int rightspeedPWM = 0;
  int counts = 0; int counts1 = 0;
  //*****************************************

  //A velocidade de cada roda é calculada através da velocidade linear v, velocidade angular w e a distância entre as duas rodas
  leftspeed = v - w * (0.092 / 2);
  rightspeed = v + w * (0.092 / 2);
//  Serial.print(leftspeed); Serial.println(rightspeed);

  //O PWM é calculado através de uma regra de 3 simples
  leftspeedPWM = (leftspeed * 143) / 0.07;
  rightspeedPWM = (rightspeed * 143) / 0.07;
  analogWrite (SP2, rightspeedPWM);
  analogWrite (SP1, leftspeedPWM);
  digitalWrite(M2, LOW);
  digitalWrite(M1, LOW);

  while (!Serial.available()) {       //Espera até chegar outro comando para sair da configuração de velocidades

    //Leitura do encoder - MOTOR 2 - RIGHT**************
    rawsensorValueR = analogRead(1);
    if (rawsensorValueR < 600)
      sensorcount1R = 1;
    else
      sensorcount1R = 0;
    if (sensorcount1R != sensorcount0R)
      countR ++;
    sensorcount0R = sensorcount1R;

    //Leitura do encoder - MOTOR 1 - LEFT****************
    rawsensorValueL = analogRead(0);
    if (rawsensorValueL < 600)
      sensorcount1L = 1;
    else
      sensorcount1L = 0;
    if (sensorcount1L != sensorcount0L)
      countL ++;
    sensorcount0L = sensorcount1L;

    //Cálculo da posição***********************************
    counts = (countR + countL) / 2;
    alpha = alpha + (countR - countL) * 0.0747;
    pos[0] = pos[0] + (counts1 - counts) * 0.00687 * cos(alpha);
    pos[1] = pos[1] + (counts1 - counts) * 0.00687 * sin(alpha);

    counts1 = counts;             //Arquiva-se o valor para pegar sempre apenas a parcela que ele andou a mais
    envia_pacote(1, pos[0], pos[1], alpha, v, w);

  }
  currentPosition[0] = pos[0]; currentPosition[1] = pos[1]; currentTheta = alpha;
  analogWrite (SP2, 0);
  analogWrite (SP1, 0);
  digitalWrite(M2, LOW);
  digitalWrite(M1, LOW);
  envia_pacote(1, currentPosition[0], currentPosition[1], currentTheta, 0, 0);

//  Serial.print(leftspeed); Serial.println(rightspeed);
}



//Função para enviar o pacote de dados. Simplesmente não funciona, nem chamei no loop.*********************************************************
void envia_pacote(int flag, float x, float y, float theta, float v, float w) {

  Serial.println(flag);
  Serial.println("x");
  Serial.println(x);
  Serial.println("y");
  Serial.println(y); 
  Serial.println("theta");
  Serial.println(theta); 
  Serial.println("v"); 
  Serial.println(v); 
  Serial.println("w"); 
  Serial.println(w);
}

void loop() {

  if (flag_inicio == 0) {            //Zera as variáveis para as posições relativas à origem(~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~)
    currentPosition[0] = 0;
    currentPosition[1] = 0;
    currentTheta = 0;
    currentTheta1 = 0;
    currentPositionFoward = 0;
  }
  Serial.print(' ');
  if (Serial.available() > 0 )     //Aguarda uma entrada serial
  {
    String buffer_in = String(26);        //String que armazena a entrada
    char buffer_out[41];                  //Buffer de saída (só falta funcionar)
    String buffer_out_1 = String(41);     //Mais uma tentativa frustrada de buffer de saída
    int flag;                             //Variável para armazenar a flag de entrada (recebida pelo supervisório)
    float x_setpoint;                     //Variável para armazenar o setpoint x (recebido pelo supervisório)
    float y_setpoint;                     //Variável para armazenar o setpoint y (recebido pelo supervisório)
    float theta_final;                    //Variável para armazenar o setpoint theta (recebido pelo supervisório)
    flag_inicio = 1;                      //Sinaliza-se que o primeiro pacote foi recebido

    buffer_in = Serial.readString();      //Captura a entrada Serial
    Serial.print(buffer_in);
  
    //Conversão do buffer de saída para as variáveis de interesse
    flag = buffer_in.substring(0, 1).toInt();
    x_setpoint = buffer_in.substring(2, 9).toFloat();
    y_setpoint = buffer_in.substring(10, 17).toFloat();
    theta_final = buffer_in.substring(18, 25).toFloat();
    setpoint[0] = x_setpoint; setpoint[1] = y_setpoint;

    if (flag == 0) {                     //CASO o buffer recebido seja de setpoint (e não de velocidades)

      float er_x = abs(setpoint[0] - currentPosition[0]);
      float er_y = abs(setpoint[1] - currentPosition[1]);
      float er_t = abs(theta_final - currentTheta);

      float thetaSP = atan2(setpoint[1] - currentPosition[1], setpoint[0] - currentPosition[0]); //Calcula-se o angulo entre o setpoint e o angulo atual


     // Serial.print("currentTheta antes da correçao:"); Serial.println(currentTheta);
      //A primeira chamada da função de correção de orientação é chamada apenas se os erros de x ou y (em módulo) forem maiores que 2 cm
      if (er_x > 0.02 || er_y > 0.02)
        currentTheta = orientacao(currentTheta1, thetaSP);

      currentTheta1 = currentTheta;
 //     Serial.print("Primeira etapa de orientação ok. currentTheta = "); Serial.println(currentTheta);

      setpointFoward = sqrt(pow(x_setpoint - currentPosition[0], 2) + pow(y_setpoint - currentPosition[1], 2)); //calcula a hipotenusa
  //    Serial.print("A hipotenusa: "); Serial.println(setpointFoward);
      if (er_x > 0.02 || er_y > 0.02)                 //~~~~~~~~~~~~~~~~~~~~~~~~~~~acho que posso tirar isso daqui~~~~~~~~~~~~~~~~~~~~~~~~
        currentPositionFoward = controlP(setpointFoward);
      else currentPositionFoward = 0;
 //     Serial.print("Chegada ao setpointFoward. currentPositionFoward = "); Serial.println(currentPositionFoward);

      currentPosition[0] = currentPosition[0] + currentPositionFoward * cos(currentTheta1);
      currentPosition[1] = currentPosition[1] + currentPositionFoward * sin(currentTheta1);

      currentTheta = orientacao(currentTheta1, theta_final);    //Corrige a orientação para a dada pelo supervisório
      currentTheta1 = currentTheta;

   //   Serial.print("Passageiros do voo ENGG68, chegamos ao destino (x,y,theta): "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[1]); Serial.print(", "); Serial.print(currentTheta);
    //  Serial.println(" ");

      envia_pacote(0, currentPosition[0], currentPosition[1], currentTheta, 0, 0);  
     // Serial.print("teste: "); Serial.println(currentPosition[0]);
    }

    if (flag == 1) {                      //CASO o buffer recebido seja de velocidade (e não de setpoint)
      float vel_linear = x_setpoint;
      float vel_angular = y_setpoint;
      set_velocidade(vel_linear, vel_angular, currentPosition, currentTheta);
      Serial.print("Posicao final da configuracao de velocidades: "); Serial.print(currentPosition[0]); Serial.print(", "); Serial.print(currentPosition[1]); Serial.print(", "); Serial.println(currentTheta);
    }
  }
 envia_pacote(0, currentPosition[0], currentPosition[1], currentTheta, 0, 0);

}
