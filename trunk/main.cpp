#include <stdlib.h>
#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

#define GUI_WIDTH 75
#define GUI_HEIGHT 50

#define FIELD_WIDTH 6.05
#define FIELD_HEIGHT 4.05

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



void clrscr(void) {
	for (int i = 0; i < 100; i++)
		putchar('\n');
}

int gotoxy(int x, int y) {
	char essq[100]; // String variable to hold the escape sequence
	char xstr[100]; // Strings to hold the x and y coordinates
	char ystr[100]; // Escape sequences must be built with characters

	/*
	** Convert the screen coordinates to strings
	*/

	sprintf(xstr, "%d", x);
	sprintf(ystr, "%d", y);

	/*
	** Build the escape sequence (vertical move)
	*/

	essq[0] = '\0';
	strcat(essq, "\033[");
	strcat(essq, ystr);

	/*
	** Described in man terminfo as vpa=\E[%p1%dd
	** Vertical position absolute
	*/

	strcat(essq, "d");

	/*
	** Horizontal move
	** Horizontal position absolute
	*/

	strcat(essq, "\033[");
	strcat(essq, xstr);

	// Described in man terminfo as hpa=\E[%p1%dG
	strcat(essq, "G");

	/*
	** Execute the escape sequence
	** This will move the cursor to x, y
	*/

	printf("%s", essq);
	return 0;
}

void process()
{
	
}

int main(int argc, char** argv) {
	printf("GUI Running!\n");
	printf("Set terminal size to %i %i\n", GUI_WIDTH, GUI_HEIGHT + 1);

	aitogui.open(false);

	/*
	while(1) {
		int i, j;
		scanf("%i %i", &i, &j);
		clrscr();
		gotoxy(i, j);
		printf("x");
		gotoxy(0, GUI_HEIGHT + 1);
		printf(">");
	}//*/

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	guitoai.open();

	while(1) {
		receive();
		process();
		send();
	}
}
