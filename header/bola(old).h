#ifndef CLASS_BOLA
#define CLASS_BOLA

#include <iostream>

#include "displaySettings.h"


using namespace std;



class Bola
{
	private:
		
		//posi��es s�o em milimetros
		double posx;
		double posy;
		double speed;
		//double altura;
	

	public:

		Bola() {posx=0;posy=0;speed=0;};
		Bola(double x, double y, double nuspeed) {posx=x;posy=y;speed=nuspeed;};
		~Bola() {};


		double getX() {return posx;};
		double getY() {return posy;};
		double getSpeed() {return speed;};
		
		void setX(double nux) {posx=nux;};
		void setY(double nuy) {posy=nuy;};
		void setSpeed(double nuspeed) {speed=nuspeed;};
		void setXY(double nux, double nuy) {posx=nux;posy=nuy;};

		void draw(DisplaySettings settings);
}; 


#endif