#include "joy.h"

Joystick::Joystick()
{
		name = DEFAULT_JOY;
}

Joystick::Joystick(string joyname)
{
		name = joyname;
}

Joystick::~Joystick()
{

}

bool Joystick::isConfigured()
{
		return fileExists(name + ".conf");			
}

void Joystick::saveConfig()
{
		fstream file( (name + ".conf").c_str() ,ios::out);
		
		for (int i=0; i < ACTIONS_TOTAL; ++i)
		{
				file << actionStrings[i] << " = " << buttonStrings[buttonMapping[i]] << endl;
		}
}

void Joystick::loadConfig()
{
		string content = readTextFile(name + ".conf");
		content = stripComments(content);
		content = stripExtraSpaces(content);
		
		vector<string> lines = splitString(content,"\n");
		
		//FIXME sempre vem uma linha em branco no final, por isso o size - 1 ali
		for (int i=0; i < lines.size() -1; ++i)
		{
				vector<string> command = splitString(lines[i], " = ");
				command[0] = trimSpaces(command[0]);
				command[1] = trimSpaces(command[1]);

				mapButton(getActionIndex(command[0]), getButtonIndex(command[1]));		
		}
		
}

void Joystick::mapButton(int action, int button)
{
		cout << "setei: " << actionStrings[action] << " para " << button << endl;
		buttonMapping[action] = button;
}

void Joystick::printStatus()
{
		cout << "_______________________________________________" << endl;
		cout << "x: " << x << " y: " << y << " z: " << z << endl;
		printActions();
}

void Joystick::receiveInput(int mask, int x, int y, int z)
{
		setAxes(x,y,z);
		
		for (int i=0; i < ACTIONS_TOTAL; ++i)
		{
			//testa se cada um dos botões mapeados foi apertado	
				if (buttons[buttonMapping[i]] & mask)
					currentActions[i] = true;
				else
					currentActions[i] = false;
		}
}

/*************************
	Getters
***************************/

int Joystick::getX() {	return x; }

int Joystick::getY() {	return y; }

int Joystick::getZ() {	return z; }

vector<int> Joystick::getAxes()
{
		vector <int> axes(3);
		axes[0] = x;
		axes[1] = y; 
		axes[2] = z;

		return axes;
}

vector<bool> Joystick::getButtonsPressed()
{
		vector<bool> actionsVector(ACTIONS_TOTAL);
		
		for (int i=0; i < ACTIONS_TOTAL; ++i)
		{
				actionsVector[i] = currentActions[i];
		}
		
		return actionsVector;
}

/*************************
	Setters
***************************/

void Joystick::setX(int newx) {	x = newx; }

void Joystick::setY(int newy) {	y = -newy; } //porque a opengl inverte o y

void Joystick::setZ(int newz) {	z = newz; }

void Joystick::setAxes(int newx, int newy, int newz)
{
		x = newx;
		y = -newy; //porque a opengl inverte o y
		z = newz;
}

/*************************
	Métodos Privados
***************************/
int Joystick::getActionIndex(string action)
{
		int found = -1;
		for (int i=0; i < ACTIONS_TOTAL; ++i)
		{
			  if (actionStrings[i] == action)
			  	found = i; 
		}
		
		return found;
}

int Joystick::getButtonIndex(string button)
{
		int found = -1;
		for (int i=0; i < BUTTONS_TOTAL; ++i)
		{
			  if (buttonStrings[i] == button)
			  	found = i; 
		}
		
		return found;
}

void Joystick::printActions()
{
		for (int i=0; i < ACTIONS_TOTAL; ++i)
		  cout << actionStrings[i] << ": " << (currentActions[i] ? "Yes" : "No") << endl; 
}
