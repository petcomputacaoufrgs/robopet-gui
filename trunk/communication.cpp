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
    if(aitoguiClient->receive(packet)) {

        if(packet.has_aitogui()) {
			
            if(isVerbose) printf("----------------------------\nReceived AI-To-GUI\n");

            game.updateNplayers(0, packet.aitogui().blue_robots_size());
            game.updateNplayers(1, packet.aitogui().yellow_robots_size());

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
                
                (*it).clearPath();
                for(int k=0; k<packet.aitogui().blue_robots(i).path_size(); k++) {
					AIToGUI::Point p = packet.aitogui().blue_robots(i).path(k);
					(*it).addPathPoint( Point(p.x(), p.y()) );
				}
                //------
                
                (*it).setId( packet.aitogui().blue_robots(i).id() );
                //------

                (*it).hasUpdatedInfo = true;

                if(isVerbose) printf("RECEIVED Blue Robot %i: %f,%f\n", i, (*it).getPosition().getX(), (*it).getPosition().getY() );

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
                
                for(int k=0; k<packet.aitogui().yellow_robots(i).path_size(); k++) {
					AIToGUI::Point p = packet.aitogui().yellow_robots(i).path(k);
					(*it).addPathPoint( Point(p.x(), p.y()) );
				}
                //------                
                
                (*it).setId( packet.aitogui().yellow_robots(i).id() );
                //------

                (*it).hasUpdatedInfo = true;
                
                if(isVerbose) printf("RECEIVED Yellow Robot %i: %f,%f\n", i, (*it).getPosition().getX(), (*it).getPosition().getY() );
            }


            game.ball.setCurrentPosition( packet.aitogui().ball().x(),
                                     packet.aitogui().ball().y()  );
            game.ball.hasUpdatedInfo = true;
            if(isVerbose) printf("RECEIVED Ball: %f,%f\n", game.ball.getPosition().getX(), game.ball.getPosition().getY() );
            
            gtk_widget_draw(this->window, NULL);
        }
    }
    else
		if(isVerbose) printf("Didn't receive AItoGUI\n");
}

void MainWindow::sendToAI()
{
	RoboPET_WrapperPacket packet;
	GUIToAI *guitoai = packet.mutable_guitoai();

	GUIToAI::Robot *r = guitoai->add_robots();
	r->set_displacement_x( 0 );
	r->set_displacement_y( 0 );
	r->set_displacement_theta( 0 );
	r->set_drible( game.joyPlayer.isDribling() );
	r->set_kick( game.joyPlayer.isKicking() );
	r->set_chip_kick(0);
	r->set_id( game.joyPlayer.getId() );
	r->set_current_theta(0);
}
