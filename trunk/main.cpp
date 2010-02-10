#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <utility>
#include <gtkmm.h>

#include "ssl_client.h"
#include "ssl_server.h"
#include "vector.h"

#define GUI_HEIGHT 44
#define GUI_WIDTH (3 * (GUI_HEIGHT) / 2)

#define FIELD_WIDTH 6050
#define FIELD_HEIGHT 4050

#define MAX_ROBOTS 5

#define FieldToGui(x, y) (GUI_WIDTH * (x) / FIELD_WIDTH), (GUI_HEIGHT * (y) / FIELD_HEIGHT)

RoboCupSSLServer guitoai(PORT_GUI_TO_AI, IP_GUI_TO_AI);
RoboCupSSLClient aitogui(PORT_AI_TO_GUI, IP_AI_TO_GUI);

enum Directions {
	Direction_Up,
	Direction_Down,
	Direction_Left,
	Direction_Right,
	Direction_Total
};
const char output_directions[Direction_Total + 1] = "^v<>",
		   output_ball = 'o';

int DEBUG = 1;

struct Robot
{
	Vector position;
	double theta;
	int direction;
} robot[TEAM_TOTAL][MAX_ROBOTS];
int robot_total[TEAM_TOTAL];

struct Ball {
	Vector position;
} ball;

Cairo::RefPtr<Cairo::Context> cr;

typedef std::pair<int, int> Pair;
std::vector<Pair > object;

void drawRobot(int team, int x, int y, double theta);
void showMsg(const char *str, ...);

void getch()
{
	scanf("%*c");
}

void receive()
{
	SSL_WrapperPacket packet;
	if (aitogui.receive(packet) && packet.has_aitogui()) {
		//printf("----------------------------");
		//printf("Received AI-To-GUI!\n");

		AIToGUI data = packet.aitogui();

		robot_total[TEAM_BLUE] = data.blue_robots_size();
		for(int i = 0; i < robot_total[TEAM_BLUE]; i++) {
			robot[TEAM_BLUE][i].position = Vector(data.blue_robots(i).current_x(),
										data.blue_robots(i).current_y());
			robot[TEAM_BLUE][i].theta = data.blue_robots(i).current_theta();
		}

		robot_total[TEAM_YELLOW] = data.yellow_robots_size();
		for(int i = 0; i < robot_total[TEAM_YELLOW]; i++) {
			robot[TEAM_YELLOW][i].position = Vector(data.yellow_robots(i).current_x(),
										data.yellow_robots(i).current_y());
			robot[TEAM_YELLOW][i].theta = data.yellow_robots(i).current_theta();
		}

		ball.position = Vector(data.ball().x(), data.ball().y());
		//printf("ball(%5i, %5i)\n", (int) ball.position.getX(), (int) ball.position.getY());
	}
}

void send()
{
	 SSL_WrapperPacket packet;

	 GUIToAI *guitoaiPacket = packet.mutable_guitoai();

	 guitoaiPacket->set_nada(0);

	 guitoai.send(packet);
	//printf("Sent GUI-To-AI\n");
}

void init_screen()
{
	//graphics dependent code
	clrscr();
}

void clear_screen()
{
	for(unsigned int i = 0; i < object.size(); i++) {
		gotoxy(object[i].first, object[i].second);
		printf(" ");
	}
	object.clear();

	/* clears the botton of the screen
	for(int i = 0; i < GUI_WIDTH; i++) {
		gotoxy(i, GUI_HEIGHT + 1);
		printf(" ");
	} //*/
}

void drawRobot(int team, int x, int y, double theta)
{
	//graphics dependent code
	int direction = theta / 90;
	assert("Theta em radianos??" && direction >= 0 && direction < Direction_Total);

	gotoxy(FieldToGui(x, y));
	printf("%c", output_directions[direction]);
	object.push_back(Pair(FieldToGui(x, y)));

	showMsg("robot: (%5i, %5i) -> (%5i, %5i)", x, y, FieldToGui(x, y));

    cr->arc(x / (float) FIELD_WIDTH, y / (float) FIELD_HEIGHT, 0.01, 0, 2 * M_PI);

    cr->save();
    cr->set_source_rgba(team == TEAM_YELLOW, team == TEAM_YELLOW , team == TEAM_BLUE, 0.8);
    cr->fill_preserve();
    cr->restore();
    cr->stroke(); //_preserve
    //cr->clip();
}

void drawBall(double x, double y)
{
	//graphics dependent code
	gotoxy(FieldToGui(x, y));
	printf("%c", output_ball);
	object.push_back(Pair(FieldToGui(x, y)));

    cr->arc(x / FIELD_WIDTH, y / FIELD_HEIGHT, 0.01, 0, 2 * M_PI);

    cr->save();
    cr->set_source_rgba(1.0, 0.5, 0.0, 0.8);
    cr->fill_preserve();
    cr->restore();
    cr->stroke(); //_preserve
    //cr->clip();
}

void showMsg(const char *str, ...)
{
	char msg[100];
	va_list params;
	va_start(params, str);
	vsprintf(msg, str, params);
	va_end(params);
	gotoxy(0, GUI_HEIGHT + 1);
	printf("%s", msg);
}

void process()
{
	for(int team = 0; team < TEAM_TOTAL; team++)
		for(int i = 0; i < robot_total[team]; i++) {
			drawRobot(team, robot[team][i].position.getX(),
				   		 	robot[team][i].position.getY(),
				   		 	robot[team][i].theta);
		}
	drawBall(ball.position.getX(), ball.position.getY());
	//showMsg("ball: (%f, %f)", ball.position.getX(), ball.position.getY());
	fflush(stdout);

	cr->clip();
}


class Clock : public Gtk::DrawingArea
{
public:
  Clock() {
    Glib::signal_timeout().connect( sigc::mem_fun(*this, &Clock::on_timeout), 10 );

#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    signal_expose_event().connect(sigc::mem_fun(*this, &Clock::on_expose_event), false);
#endif
  }
  virtual ~Clock(){}

protected:
  virtual bool on_expose_event(GdkEventExpose* event) {
      // This is where we draw on the window
      Glib::RefPtr<Gdk::Window> window = get_window();
      if(window)
      {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        cr = window->create_cairo_context();

        if(event)
        {
            // clip to the area indicated by the expose event so that we only
            // redraw the portion of the window that needs to be redrawn
            cr->rectangle(event->area.x, event->area.y,
                    event->area.width, event->area.height);
            cr->clip();
        }

        // scale to unit square
        cr->scale(width, height);
        //cr->translate(0.5, 0.5); //(0, 0) to center of window
        cr->set_line_width(0.001);

        cr->save(); //background
        cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);   // green
        cr->paint();
        cr->restore();

        process();
    /*
        cr->arc(ball.position.getX() / FIELD_WIDTH, ball.position.getY() / FIELD_HEIGHT, 0.01, 0, 2 * M_PI);

        cr->save();
        cr->set_source_rgba(1.0, 0.0, 0.0, 0.8);
        cr->fill_preserve();
        cr->restore();
        cr->stroke_preserve();
        cr->clip();


        cr->arc(ball.position.getX() / FIELD_WIDTH, ball.position.getY() / FIELD_HEIGHT, 0.01, 0, 2 * M_PI);

        cr->save();
        cr->set_source_rgba(1.0, 0.0, 0.0, 0.8);
        cr->fill_preserve();
        cr->restore();
        cr->stroke_preserve();
        cr->clip();//*/
      }

      return true;
  }

  bool on_timeout() {
		clear_screen();
		//printf("drawning\n");
		receive();
		//process(); //inside on_expose_event
		send();

        // force our program to redraw the entire window.
        Glib::RefPtr<Gdk::Window> win = get_window();
        if (win)
        {
            Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                    get_allocation().get_height());
            win->invalidate_rect(r, false);
        }
        return true;
  }
};

int main(int argc, char** argv) {
	Gtk::Main kit(argc, argv);


	printf("GUI Running!\n");
	printf("Set terminal size to %i %i\n", GUI_WIDTH, GUI_HEIGHT + 1);

	aitogui.open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	guitoai.open();

	clrscr();
	/*
	while(1) {
		clear_screen();
		receive();
		process();
		send();
	}//*/

   Gtk::Window win;
   win.set_title("GUI");
   win.maximize();

   Clock c;
   win.add(c);
   c.show();

   Gtk::Main::run(win);
}

