#include "agent.h"
#include "environment.h"
#include <cstdlib>

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{

    if(dirty_){
        return actSUCK;
    }

    // En caso de que choque, colocamos en la posición correspondiente
    // un valor especial, en este caso 'wall'.


     if(bump_){

         if(last_action == 0)
            mapa[posx][posy+1] = 'wall';

        else if(last_action == 1)
            mapa[posx][posy-1] = 'wall';

        else if(last_action == 2)
            mapa[posx-1][posy] = 'wall';

        else if(last_action == 3)
            mapa[posx+1][posy] = 'wall';

     }



    // Si no choca, ponemos en la casilla que nos encontremos un valor
    // de prioridad "contador" que ira aumentando con cada acción del robot


     else{

        mapa[posx][posy] = contador;
        contador++;

        if(last_action == 0)
            posy++;

        else if(last_action == 1)
            posy--;

        else if(last_action == 2)
            posx--;

        else if(last_action == 3)
            posx++;

     }


    // Para saber cual será el próximo movimiento del robot comparamos
    // las prioridades de las casillas que tiene en cada momento a su
    // alrededor. Se moverá a la que tenga un valor de prioridad menor.


    int menor, menor2, pos_menor, pos_menor2, muevete;

    if(mapa[posx - 1][posy] <= mapa[posx + 1][posy]){
        menor = mapa[posx - 1][posy];
        pos_menor = 2;
    }

    else {
        menor = mapa[posx + 1][posy];
        pos_menor = 3;
    }

    if(mapa[posx][posy - 1] <= mapa[posx][posy + 1]){
        menor2 = mapa[posx][posy - 1];
        pos_menor2 = 1;
    }

    else {
        menor2 = mapa[posx][posy + 1];
        pos_menor2 = 0;
    }


    if(menor <= menor2)
        muevete = pos_menor;
    else
        muevete = pos_menor2;


    last_action = muevete;

    return static_cast<ActionType> (muevete);

}




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
