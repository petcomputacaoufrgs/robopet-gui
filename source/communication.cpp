#include <stdlib.h>
#include "communication.h"
#include "interface.h"



void MainWindow::openClient(int port, char* host)
{
    aitoguiClient = new RoboPETClient(port,host);

    if(isVerbose) cout<<"Ready to receive from AI..."<<endl;
    pushStatusMessage("Ready to receive from AI...");
    aitoguiClient->open(false);
}


void MainWindow::openServer(int port, char* host)
{
    guitoaiServer = new RoboPETServer(port,host);

    if(isVerbose) cout<<"Ready to send to AI..."<<endl;
    pushStatusMessage("Ready to send to AI...");
    guitoaiServer->open();
}


void MainWindow::closeClient()
{
    delete aitoguiClient;
    if(isVerbose)  printf("AI->GUI Communication closed.\n");
    pushStatusMessage("AI->GUI Communication closed.");
}

void MainWindow::closeServer()
{
    delete guitoaiServer;
    if(isVerbose)  printf("GUI->AI Communication closed.\n");
    pushStatusMessage("GUI->AI Communication closed.");
}


void MainWindow::communicate()
{
    if( aitoguiClient )
        listenToAI();

    if( guitoaiServer )
        sendToAI();
}

void MainWindow::listenToAI()
{

    RoboPET_WrapperPacket packet;
    if(aitoguiClient->receive(packet))
    {
        if(isVerbose) cout<<"opa, recebi um pacote!"<<endl;

        if(packet.has_aitogui())
        {
            if(isVerbose) cout<<"ah, esse pacote eh meu."<<endl;
            //pushStatusMessage("Packet received!");



            game.updateNplayersTeam1(packet.aitogui().blue_robots_size());
            game.updateNplayersTeam2(packet.aitogui().yellow_robots_size());

            vector<guiPlayer>::iterator it;
            it = game.players[0].begin();
            for(int i=0; it<game.players[0].end(); it++, i++) {

                (*it).setCurrentPosition( packet.aitogui().blue_robots(i).current_x() ,
                                                    packet.aitogui().blue_robots(i).current_y() );
                (*it).setCurrentAngle( packet.aitogui().blue_robots(i).current_theta() );
                //------

                (*it).setFuturePosition( packet.aitogui().blue_robots(i).future_x() ,
                                                   packet.aitogui().blue_robots(i).future_y() );
                (*it).setFutureAngle( packet.aitogui().blue_robots(i).future_theta() );
                //------

                (*it).hasUpdatedInfo = true;

                //cout << packet.aitogui().blue_robots(i).current_x() << " , " << packet.aitogui().blue_robots(i).current_y() << endl;

            }

            it = game.players[1].begin();
            for(int i=0; it<game.players[1].end(); it++, i++) {

                (*it).setCurrentPosition( packet.aitogui().yellow_robots(i).current_x() ,
                                                    packet.aitogui().yellow_robots(i).current_y() );
                (*it).setCurrentAngle( packet.aitogui().yellow_robots(i).current_theta() );
                //------

                (*it).setFuturePosition( packet.aitogui().yellow_robots(i).future_x() ,
                                                   packet.aitogui().yellow_robots(i).future_y() );
                (*it).setFutureAngle( packet.aitogui().yellow_robots(i).future_theta() );
                //------

                (*it).hasUpdatedInfo = true;

            }


            game.ball.setCurrentPosition( packet.aitogui().ball().x(),
                                     packet.aitogui().ball().y()  );

            game.ball.hasUpdatedInfo = true;

        }

    }
    //else printf("Communication Client NULL!\n");
}

void MainWindow::sendToAI()
{

    //nothing here yet
}
