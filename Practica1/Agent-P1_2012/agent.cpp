#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
    ActionType accion;//valor que va ha devolver la funcion. Del tipo ActionType
    if(dirty_ && !(ACTUAL[now-2]=='S')) //si esta sucio y el agento no limpio en su penultima accion limpia
     {
        estado=Suck();
        registro("SUCK");
        accion=actSUCK;
    }
    //Si la ultima accion fue un giro la siguiente sera ir hacia delante
    //esto va a pasar siempre ya que sino fuera asi solo giraria sobre si mismo
    //o si a limpiado tambien va hacia delante por que sino limpiaria siempre
    else if((ACTUAL[now-1] == 'R')|| (ACTUAL[now-1] == 'L')|| (ACTUAL[now-1]=='S')){
            estado = Forward();
            registro("FORWARD");
            accion=actFORWARD;
    }//Si choca con un obtaculo.
    else if(bump_){
    //comprobamos la accion que halla 4 casillas antes de now ya que depues de cada giro y de cada hacia delante hay un suck.
        if((ACTUAL[now-4]=='F') || (ACTUAL[now-4]=='R') || (ACTUAL[now-4]=='L') || (ACTUAL[now-2]=='S')){//si fue Forward despues ha tenido que haber otro Forward por eso no lo tengo en cuenta
            switch(rand()%2){//Por tanto girara ,O,a derecha ,O, a iquierda sin contemplar la posibilidad de ir hacia delante. Seguro que tambien hubo un SUCK lo miro en la dos menos que now
                case 0: estado=Left();
                    registro("LEFT");
                    accion=actTURN_L;
                    break;

                case 1: estado==Right();
                    registro("RIGHT");
                    accion = actTURN_R;

	       }
        }
    }

    else{//en el caso en que no este sucio  siguiente se elige de forma aleatoria a donde ir
    switch(rand()%3){
        case 0: estado=Forward();
                registro("FORWARD");
                accion=actFORWARD;
                break;
        case 1: estado=Left();
                registro("LEFT");
                accion=actTURN_L;
                break;

        case 2: estado=Right();
                registro("RIGHT");
                accion = actTURN_R;


	       }



}
return accion;

}

//////////////////////////////////////////////
/*Estas funciones solo me ayudan a saber en que direccion va
  el agente o que accion ha tomado y facilita pasarselo a la funcion registro
  */
string Agent::Forward()
{
    return "FORWARD";
}
 string Agent::Suck()
 {
     return "SUCK";
 }
string Agent::Left()
{
    return "LEFT";
}

string Agent::Right()
{
    return "RIGHT";
}
///////////////////////////////////////////////////////
/* Registro es una funcion que almacena en un array
los movimientos que ha llevado a cabo el agente
Los almaceno en ACTUAL con un char dependiendo de accion que
ha llevado a cabo
*/
void Agent::registro(string temp)
{

    if(temp == "FORWARD")
    {
        ACTUAL[now] = 'F';

    }
    if (temp=="SUCK")
    {
        ACTUAL[now] = 'S';
    }

    else if(temp == "LEFT")
    {
        ACTUAL[now] = 'L';

    }
    else if(temp == "RIGHT")
    {
        ACTUAL[now] = 'R';

    }

    now = now + 1;


}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	bump_ = env.isJustBump();
	dirty_ = env.isCurrentPosDirty();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actSUCK: return "SUCK";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
