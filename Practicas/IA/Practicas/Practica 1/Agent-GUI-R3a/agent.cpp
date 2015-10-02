#include "agent.h"
#include "environment.h"
#include <cstdlib>

///////////////////////////////////////////////////////////////////

Agent::ActionType Agent::Think()
{

    if (etapa == 1) { // Etapa 1 -> inspecciono el mapa (W = pared, R = libre)

        if (bump_) {
            
            MAPA[posx][posy] = 'W';
            
            if (estado == "UP") posx = posx+1;
            else if (estado == "DOWN") posx = posx-1;
            else if (estado == "RIGHT") posy = posy-1;
            else if (estado == "LEFT") posy = posy+1;
            registro("BACK");
            if (dirty_) return actSUCK;
            else return actIDLE;
            
        }

        if (dirty_) return actSUCK;

        if (MAPA[posx-1][posy] == '-') {
            
            estado = Up();
            registro("UP");
            
        }

        else if(MAPA[posx][posy-1] == '-') {
            
            estado = Left();
            registro("LEFT");
        
        }

        else if(MAPA[posx][posy+1] == '-') {
        
            estado = Right();
            registro("RIGHT");
        
        }

        else if(MAPA[posx+1][posy] == '-') {
       
            estado = Down();
            registro("DOWN");
        
        }

        else {

            if(ACTUAL[now-1] == 'U') {
               
                accionACT[accionNow] = 'D';
                estado = Down();
            
            }
            
            else if(ACTUAL[now-1] == 'D') {
                
                accionACT[accionNow] = 'U';
                estado = Up();
                
            }
            
            else if(ACTUAL[now-1] == 'L') {
                
                accionACT[accionNow] = 'R';
                estado = Right();
                
            }
           
            else if(ACTUAL[now-1] == 'R') {
                
                accionACT[accionNow] = 'L';
                estado = Left();
                
            }

            else if(ACTUAL[now-1] == 'I') {
                
                etapa = 2;
                if (dirty_) return actSUCK;
                else return actIDLE;
                
            }
            
            registro("moreBACK");
        }
        
        
    }


    else if(etapa == 2) { // Etapa 2 -> recorro el mapa y lo limpio
        
        if (dirty_) return actSUCK;
        contador++;

        if(contador == accionNow) contador = 0;
        if(accionACT[contador] == 'U') return actUP;
        else if(accionACT[contador] == 'D') return actDOWN;
        else if(accionACT[contador] == 'L') return actLEFT;
        else if(accionACT[contador] == 'R') return actRIGHT;
        
        else if(accionACT[contador] == 'I') {
            
            if (dirty_) return actSUCK;
            return actIDLE;
        
        }
        
    }

    if(estado == "UP") return actUP;
    else if(estado == "DOWN") return actDOWN;
    else if(estado == "RIGHT") return actRIGHT;
    else if(estado == "LEFT") return actLEFT;
    
    else if(estado == "IDLE") {
        
        if (dirty_) return actSUCK;
        return actIDLE;
    
    }

}

////////////////////////////////////////////////////////////////


string Agent::Up()
{
    if(MAPA[posx][posy] == '-')
        MAPA[posx][posy] = 'F';
    posx = posx-1;
    return "UP";
}

string Agent::Down()
{
    if(MAPA[posx][posy] == '-')
        MAPA[posx][posy] = 'F';
    posx = posx+1;
    return "DOWN";
}

string Agent::Left()
{
    if(MAPA[posx][posy] == '-')
        MAPA[posx][posy] = 'F';
    posy = posy-1;
    return "LEFT";
}

string Agent::Right()
{
    if(MAPA[posx][posy] == '-')
        MAPA[posx][posy] = 'F';
    posy = posy+1;
    return "RIGHT";
}


void Agent::registro(string temp)
{
    if(temp == "BACK")
    {
        now = now - 1;
        accionNow--;
        return;
    }

    if(temp == "moreBACK")
    {
        now = now - 1;
        accionNow++;
        return;
    }

    if(temp == "UP")
    {
        ACTUAL[now] = 'U';
        accionACT[accionNow] = 'U';
    }
    else if(temp == "DOWN")
    {
        ACTUAL[now] = 'D';
        accionACT[accionNow] = 'D';
    }
    else if(temp == "LEFT")
    {
        ACTUAL[now] = 'L';
        accionACT[accionNow] = 'L';
    }
    else if(temp == "RIGHT")
    {
        ACTUAL[now] = 'R';
        accionACT[accionNow] = 'R';
    }

    now = now + 1;
    accionNow = accionNow + 1;

}


///////////////////////////////////////////////////////////


void Agent::Perceive(const Environment &env)
{
    bump_ = env.isJustBump();
    dirty_ = env.isCurrentPosDirty();
}

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
