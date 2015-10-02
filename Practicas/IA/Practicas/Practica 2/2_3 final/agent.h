#ifndef AGENT__
#define AGENT__

#include <string>
#include "environment.h"
#include "state.h"
#include "plan.h"

using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------

class Agent
{
public:
Agent():bump_(false), dirty_(false), last_action(5), contador(1), posx(80), posy(80) {
        for(int i = 0; i < 160; i++){
            for(int a = 0; a < 160; a++){
                mapa[i][a] = -1;
            }
        }
        mapa[80][80] = 0;
	}

    ~Agent(){
    }


	void Perceive(const Environment &env);
	Environment::ActionType AgenteReactivo();
    Plan Busqueda_Anchura(state start);
	Plan Busqueda_Profundidad(state start);
    Plan Escalada(state start);
    Plan Think(const Environment &env, int option);


private:
	bool bump_,
		 dirty_;
	int mapa[160][160];
	int contador, posx, posy;
	int last_action;


};

string ActionStr(Environment::ActionType);

#endif
