
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
	Agent():bump_(false), dirty_(false), last_action(5), contador(1), posx(10), posy(10) {
        
        for(int i = 0; i < 25; i++){
            for(int a = 0; a < 25; a++){
                mapa[i][a] = -1;
            }
        }
        
        mapa[10][10] = 0;
	}

	enum ActionType { actUP, actDOWN, actLEFT, actRIGHT, actSUCK, actIDLE };

	void Perceive(const Environment &env);
	ActionType Think();
    
private:
	bool bump_,
		 dirty_;
    
	int mapa[25][25];
	int contador, posx, posy;
	int last_action;
};

string ActionStr(Agent::ActionType);

#endif
