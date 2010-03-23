#include <stdlib.h>
#include <mainWindow.h>
#include "interface.h"



void MainWindow::openCommunication(int port, char* host)
{
    communicationClient = new GUIClient(port,host);
    if(IS_VERBOSE) cout<<"Ready to receive AI Packets..."<<endl;
    communicationClient->open(false);
}

void MainWindow::closeCommunication()
{
    delete communicationClient;
    if(IS_VERBOSE)  printf("Communication closed.\n");
}


void MainWindow::listenToAI()
{
    if(communicationClient)
    {
        SSL_WrapperPacket packet;
        if(communicationClient->receive(packet))
        {
            if(IS_VERBOSE) cout<<"opa, recebi um pacote!"<<endl;

            if(packet.has_aitogui())
            {
                if(IS_VERBOSE) cout<<"ah, esse pacote eh meu."<<endl;



                    if(IS_VERBOSE) cout << endl << "*------------------------------------------------*" << endl;
                    if(IS_VERBOSE) cout << "* " << packet.aitogui().blue_robots_size() << " jogadores azuis e " << packet.aitogui().yellow_robots_size() << " jogadores amarelos."<< "\t *" << endl;


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
                        if(IS_VERBOSE) cout << "* jogador azul " << i << ": " << game.playersTeam1[i].getCurrentPosition().getX() << "," << game.playersTeam1[i].getCurrentPosition().getY() << "\t\t\t *" << endl;

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

                        if(IS_VERBOSE) cout << "* jogador amarelo " << i << ": " << game.playersTeam2[i].getCurrentPosition().getX() << "," << game.playersTeam2[i].getCurrentPosition().getY() << "\t\t\t *" << endl;
                }


                game.ball.setCurrentPosition( packet.aitogui().ball().x(),
                                         packet.aitogui().ball().y()  );

                game.ball.setFuturePosition(  packet.aitogui().ball().past_x(),
                                         packet.aitogui().ball().past_y()  );

                game.ball.hasUpdatedInfo = true;

                    if(IS_VERBOSE) cout << "*------------------------------------------------*" << endl << endl;
            }

        }
    }
    //else printf("Communication Client NULL!\n");
}
