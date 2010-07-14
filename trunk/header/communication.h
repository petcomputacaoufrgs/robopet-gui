#ifndef __GUI_COMMUNICATION_H__
#define __GUI_COMMUNICATION_H__

#include "rp_client.h"
#include "rp_server.h"

#include <mainWindow.h>


void openClient(int port, char* host);

void openServer(int port, char* host);

void closeClient();

void closeServer();

void comunicate();

void listenToAI();

void sendToAI();


#endif /* __GUI_COMMUNICATION_H__ */
