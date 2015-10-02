
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

	Agent():bump_(false), dirty_(false), posx(10), posy(10), now(1), estado("NULL"), contador(0), etapa(1), accionNow(1)
	{

		for(int i = 0; i < 20; i++)
			for(int j = 0; j < 20; j++)
				MAPA[i][j] = '-';

		for(int i = 0; i < 2000; i++)
			ACTUAL[i] = 'N';

		ACTUAL[0] = 'I';

		for(int i = 0; i < 1000; i++) accionACT[i] = 'I';

	}


	string Up();
	string Down();
	string Left();
	string Right();

	void registro(string temp);

	enum ActionType { actUP, actDOWN, actLEFT, actRIGHT, actSUCK, actIDLE };
    
	void Perceive(const Environment &env);
	ActionType Think();

private:
	bool bump_,
		 dirty_;
    
	int posx, posy, now, accionNow;
	string estado;
	char MAPA[20][20];
	char ACTUAL[2000];
	char accionACT[1000];
    int contador;
    int etapa;

};

string ActionStr(Agent::ActionType);



#endif
