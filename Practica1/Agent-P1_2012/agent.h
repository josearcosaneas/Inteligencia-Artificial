#ifndef AGENT__
#define AGENT__

#include <string>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent():bump_(false), dirty_(false){}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
	    actSUCK,
	    actIDLE
	};

	string Forward();
	string Left();
	string Right();
	string Suck();

	void registro(string temp);

	void Perceive(const Environment &env);
	ActionType Think();
private:
	bool bump_, dirty_;
    int now;
    string estado;
    char ACTUAL[2000];
};

string ActionStr(Agent::ActionType);

#endif
