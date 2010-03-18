#ifndef __GUI_COMMUNICATION_H__
#define __GUI_COMMUNICATION_H__

#include "ssl_client.h"
#include "ssl_server.h"


typedef RoboCupSSLClient GUIClient;

/*class GUIClient
{
    public:

        GUIClient(int port, char* hostname);
        GUIClient();
        ~GUIClient();


        //void sslInitClient(int port, char* hostname);

        void receivePacket();

//        void testClient();


    private:

        char* hostname;

        int port;

        RoboCupSSLClient *client;

};*/


#endif /* __GUI_COMMUNICATION_H__ */
