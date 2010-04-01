#include <stdlib.h>
#include <mainWindow.h>
#include "interface.h"



void MainWindow::openClient(int port, char* host)
{
    aitoguiClient = new RoboCupSSLClient(port,host);

    if(IS_VERBOSE) cout<<"Ready to receive from AI..."<<endl;
    pushStatusMessage("Ready to receive from AI...");
    aitoguiClient->open(false);
}


void MainWindow::openServer(int port, char* host)
{
    guitoaiServer = new RoboCupSSLServer(port,host);

    if(IS_VERBOSE) cout<<"Ready to send to AI..."<<endl;
    pushStatusMessage("Ready to send to AI...");
    guitoaiServer->open();
}


void MainWindow::closeClient()
{
    delete aitoguiClient;
    if(IS_VERBOSE)  printf("AI->GUI Communication closed.\n");
    pushStatusMessage("AI->GUI Communication closed.");
}

void MainWindow::closeServer()
{
    delete guitoaiServer;
    if(IS_VERBOSE)  printf("GUI->AI Communication closed.\n");
    pushStatusMessage("GUI->AI Communication closed.");
}


void MainWindow::listenToAI()
{
    static char debugText[1024];


    if(aitoguiClient)
    {
        SSL_WrapperPacket packet;
        if(aitoguiClient->receive(packet))
        {
            if(IS_VERBOSE) cout<<"opa, recebi um pacote!"<<endl;

            if(packet.has_aitogui())
            {
                if(IS_VERBOSE) cout<<"ah, esse pacote eh meu."<<endl;
                //pushStatusMessage("Packet received!");



                    sprintf(debugText,"%i blue player, %i yellow players.\n",packet.aitogui().blue_robots_size(),packet.aitogui().yellow_robots_size());
                    

                game.updateNplayersTeam1(packet.aitogui().blue_robots_size());
                game.updateNplayersTeam2(packet.aitogui().yellow_robots_size());

                vector<GUIPlayer>::iterator it;
                it = game.playersTeam1.begin();
                for(int i=0; it<game.playersTeam1.end(); it++, i++) {
                //for(int i = 0; i < MAX_JOGADORES && i < packet.aitogui().blue_robots_size(); ++i) {

                    (*it).setCurrentPosition( packet.aitogui().blue_robots(i).current_x() ,
                                                        packet.aitogui().blue_robots(i).current_y() );
                    (*it).setCurrentAngle( packet.aitogui().blue_robots(i).current_theta() );
                    //------

                    (*it).setFuturePosition( packet.aitogui().blue_robots(i).future_x() ,
                                                       packet.aitogui().blue_robots(i).future_y() );
                    (*it).setFutureAngle( packet.aitogui().blue_robots(i).future_theta() );
                    //------

                    (*it).setPastPosition( packet.aitogui().blue_robots(i).past_x() ,
                                                     packet.aitogui().blue_robots(i).past_y() );

                    (*it).hasUpdatedInfo = true;

                    //cout << packet.aitogui().blue_robots(i).current_x() << " , " << packet.aitogui().blue_robots(i).current_y() << endl;
                    char buffer[1024];
                    sprintf(buffer,"Blue player %i: %.0f,%.0f\n",i, game.playersTeam1[i].getCurrentPosition().getX(),game.playersTeam1[i].getCurrentPosition().getY());
                    strcat(debugText,buffer);

                }

                it = game.playersTeam2.begin();
                for(int i=0; it<game.playersTeam2.end(); it++, i++) {
                //for(int i = 0; i < MAX_JOGADORES && i < packet.aitogui().yellow_robots_size(); ++i) {

                    (*it).setCurrentPosition( packet.aitogui().yellow_robots(i).current_x() ,
                                                        packet.aitogui().yellow_robots(i).current_y() );
                    (*it).setCurrentAngle( packet.aitogui().yellow_robots(i).current_theta() );
                    //------

                    (*it).setFuturePosition( packet.aitogui().yellow_robots(i).future_x() ,
                                                       packet.aitogui().yellow_robots(i).future_y() );
                    (*it).setFutureAngle( packet.aitogui().yellow_robots(i).future_theta() );
                    //------

                    (*it).setPastPosition( packet.aitogui().yellow_robots(i).past_x() ,
                                                     packet.aitogui().yellow_robots(i).past_y() );

                    (*it).hasUpdatedInfo = true;

                    char buffer[1024];
                    sprintf(buffer,"Yellow player %i: %.0f,%.0f\n",i, game.playersTeam2[i].getCurrentPosition().getX(),game.playersTeam2[i].getCurrentPosition().getY());
                    strcat(debugText,buffer);
                }


                game.ball.setCurrentPosition( packet.aitogui().ball().x(),
                                         packet.aitogui().ball().y()  );

                game.ball.setFuturePosition(  packet.aitogui().ball().past_x(),
                                         packet.aitogui().ball().past_y()  );

                game.ball.hasUpdatedInfo = true;

                    char buffer[1024];
                    sprintf(buffer,"Ball: %0.f,%0.f\n",game.ball.getCurrentPosition().getX(),game.ball.getCurrentPosition().getY());
                    strcat(debugText,buffer);


                    if(IS_VERBOSE) cout << debugText;
                    fillTextView(debugText);
            }

        }
    }
    //else printf("Communication Client NULL!\n");
}
