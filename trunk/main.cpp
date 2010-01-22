/*
 * File:   main.cpp
 * Author: pet
 *
 * Created on 4 de Dezembro de 2009, 17:05
 */

#include <stdlib.h>
#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

RoboCupSSLServer guitoai(PORT_GUI_TO_AI, IP_GUI_TO_AI);
RoboCupSSLClient aitogui(PORT_AI_TO_GUI, IP_AI_TO_GUI);

int DEBUG = 1;

void receive()
{
	SSL_WrapperPacket packet;
	if (aitogui.receive(packet) && packet.has_aitogui()) {
		printf("----------------------------");
		printf("Received AI-To-GUI!\n");
	}
}

void send()
{
	 SSL_WrapperPacket packet;

	 GUIToAI *guitoaiPacket = packet.mutable_guitoai();

	 guitoaiPacket->set_nada(0);

	 guitoai.send(packet);
	printf("Sent GUI-To-AI\n");
}

/*
 *
 */
int main(int argc, char** argv) {
	printf("GUI Running!\n");

	aitogui.open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	guitoai.open();

	while(1) {
		receive();
		send();
	}
}
