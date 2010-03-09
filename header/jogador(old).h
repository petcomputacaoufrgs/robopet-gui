#ifndef CLASS_JOGADOR
#define CLASS_JOGADOR

#include<iostream>

#include "displaySettings.h"
#include "point.h"


using namespace std;




class Jogador
{
	private:

		//posi��es s�o em milimetros
		Point pos;
		Point vector;
		double angle; //de 0 a 360 graus
		double speed; //unidade a se definir


		void drawBody(float centerX, float centerY);
		void drawAngle(float centerX, float centerY, float angle);
		void drawIndex(float centerX, float centerY, int robotNumber);
		void drawVector(float centerX, float centerY, float vecX, float vecY);

	public:
		Jogador();
		Jogador(double x, double y, double ang, double nuspeed, Point nuvector);
		~Jogador();

		double getX();
		double getY();
		double getAngle();
		double getSpeed();
		//Point getVector();
		
		void setX(double);
		void setY(double);
		void setAngle(double);
		void setSpeed(double);
		//void setVector(Point);

		void moveX(double);
		void moveY(double);		
		void setXY(double, double);

                Point getCurrentPosition();
		
		void draw(int index, DisplaySettings settings);
}; 


#endif