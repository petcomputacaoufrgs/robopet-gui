#include "jogador.h"


ostream &operator<<(ostream &stream, Jogador player)
{
	stream << player.getX() << "," << player.getY() << "," << player.getAngle() << endl;
	return stream;
}



Jogador::Jogador()
{ 
	pos.setXY(0,0);
	angle=0;
	speed=0;
};

Jogador::Jogador(double x, double y, double ang, double nuspeed, Point nuvector) { pos.setXY(x,y); angle=ang; speed=nuspeed; vector=nuvector; };

Jogador::~Jogador() {};


double Jogador::getX() { return pos.getX(); };

double Jogador::getY() { return pos.getY(); };

double Jogador::getAngle() { return angle; };

double Jogador::getSpeed() { return speed; };


void Jogador::setX(double nux) { pos.setX(nux); };

void Jogador::setY(double nuy) { pos.setY(nuy); };

void Jogador::setSpeed(double nuspeed) { speed = nuspeed; };

void Jogador::setAngle(double nuangle)
{ 
	if ( nuangle >= 0 ) {

		while ( nuangle > 360 )
			nuangle -= 360;
	
		angle=angle;
	}
};


void Jogador::moveX(double deltax) { pos.setX( pos.getX() + deltax ); }

void Jogador::moveY(double deltay) { pos.setY( pos.getY() + deltay ); }

void Jogador::setXY(double nux, double nuy){ pos.setX(nux); pos.setY(nuy); }

Point Jogador::getCurrentPosition() { return Point(pos.getX(),pos.getY()) }