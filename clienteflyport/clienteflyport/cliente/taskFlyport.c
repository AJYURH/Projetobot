#include "taskFlyport.h"

void FlyportTask()
{
	//UDP Udpsock = INVALID_SOCKET;
	char msg[500];
	//int cnt = 0;
	BOOL flagErr = FALSE;
	int myAdcValue = 0;
	
	
	// Flyport connects to default network
	WFConnect(WF_DEFAULT);
	while(WFGetStat() != CONNECTED);
	while(!DHCPAssigned);
	
	UARTWrite(1,"Tentando UDP!\n");
	int UdpSocket = UDPClientOpen("192.168.43.65","5555");
	int UdpRxLength = 0;
	
	while(1){
		vTaskDelay(50);
		
		
		//UARTWrite(1,"Flyport Wi-fi connected UDP...hello world!\r\n");
		
		myAdcValue = ADCVal(2);
		sprintf(msg,"Flyport Wi-fi connected UDP temp: %d hello world!\r\n", myAdcValue);
		
		UARTWrite(1,msg);
		
		
		//Cria mensagem para enviar para o servidor
		//sprintf(msg,"%s","Hello server aqui eh o Fly!!");
		
		//Connecta o cliente TCP ao servidor
		if(!UdpSocket)
		{
			UARTWrite(1, "Não foi possivel encontrar o servidor UDP\r\n");
		
		}
		
		
		
		if(flagErr)
		{
			UARTWrite(1,"\r\nTime error... \r\n");

		}
		
		//envia a mensagem
		
		UdpRxLength = UDPRxLen(UdpSocket);
		UDPWrite(UdpSocket,(BYTE*)msg,strlen(msg));
		
		sprintf(msg,"\r\nLength data: %d\r\n",UdpRxLength);
		UARTWrite(1,msg);
		
	}
		

}


