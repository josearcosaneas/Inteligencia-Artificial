
#ifndef AGENT__
#define AGENT__

#include <string>
using std::string;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent():bump_(false), dirty_(false) {
	for (int i=0; i<10; i++)
	  for (int j=0; j<10; j++)
	    mapa[i][j]=0;
	posx=0;
	posy=0;
    last_action=5;
    }

	enum ActionType { actUP, actDOWN, actLEFT, actRIGHT, actSUCK, actIDLE };

	void Perceive(const Environment &env);
	ActionType Think();
private:
	bool bump_,
		 dirty_;
    char mapa[10][10];
    int posx,
        posy;
    int last_action;
};

string ActionStr(Agent::ActionType);

#endif
