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

    Agent(){
        bump_=false;
        dirty_=false;
    }

    ~Agent(){
    }


	void Perceive(const Environment &env);
	Environment::ActionType AgenteReactivo();
	Plan Busqueda_Profundidad(state start);
    Plan Busqueda_Anchura(state start);
    Plan Busqueda_CosteUniforme(state start);
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
