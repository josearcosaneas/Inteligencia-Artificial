#include "agent.h"
#include "environment.h"
#include <cstdlib>


// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	/*if (dirty_) return actSUCK;
	else return static_cast<ActionType> (rand()%4);*/

	// Actualizo el mapa si se ha chocado con algo
	if (bump_){
	    switch (last_action){
            case actUP: mapa[posx][(posy+9)%10]=-1;
                        break;
            case actDOWN: mapa[posx][(posy+1)%10]=-1;
                        break;
            case actLEFT: mapa[(posx-1+10)%10][posy]=-1;
                        break;
            case actRIGHT: mapa[(posx+1)%10][posy]=-1;
                        break;
	    }
	    last_action=5;
	}
	else {
	    switch (last_action){
            case actUP: posy = (posy-1+10)%10;
                        break;
            case actDOWN: posy = (posy+1)%10;
                        break;
            case actLEFT: posx=(posx-1+10)%10;
                        break;
            case actRIGHT: posx=(posx+1)%10;
                        break;
	    }
	    mapa[posx][posy]++;
	}

    // Si esta sucio, limpio
	if (dirty_){
        last_action=4;
	    //mapa[posx][posy]++;
	    return actSUCK;
	}

	// Si esta limpia, decido moverme a la casilla por la que he pasado menos veces
	int valor=100000;
	for (int i=-1; i<=1; i+=2)
	  if (mapa[(posx+i+10)%10][posy]>-1 and mapa[(posx+i+10)%10][posy]<valor){
	      valor = mapa[(posx+i+10)%10][posy];
	      if (i==-1)
            last_action=2;
          else
            last_action=3;
	  }

	for (int i=-1; i<=1; i+=2)
	  if (mapa[posx][(posy+i+10)%10]>-1 and mapa[posx][(posy+i+10)%10]<valor){
	      valor = mapa[posx][(posy+i+10)%10];
	      if (i==-1)
            last_action=0;
          else
            last_action=1;
	  }

    // Toma de decisión

    switch (last_action){
        case 0: return actUP;
                break;
        case 1: return actDOWN;
                break;
        case 2: return actLEFT;
                break;
        case 3: return actRIGHT;
    }

}
// -----------------------------------------------------------







// -----------------------------------------------------------
// -----------------------------------------------------------
// -----------------------------------------------------------

















// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	bump_ = env.isJustBump();
	dirty_ = env.isCurrentPosDirty();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType action)
{
	switch (action)
	{
	case Agent::actUP: return "UP";
	case Agent::actDOWN: return "DOWN";
	case Agent::actLEFT: return "LEFT";
	case Agent::actRIGHT: return "RIGHT";
	case Agent::actSUCK: return "SUCK";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
