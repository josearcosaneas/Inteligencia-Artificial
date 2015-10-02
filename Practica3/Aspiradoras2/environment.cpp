
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include "stdlib.h"

#include "environment.h"
//#include "random_num_gen.h"
#include"include/GL/glui.h"

Environment::Environment(){
    MAZE_SIZE = 0;
    agent1PosX_=agent1PosY_=agent2PosX_=agent2PosY_=agent1Rumbo_=agent2Rumbo_=0;
    agent1Giros_=agent2Giros_=0;
    maze_=0;
    marcador1_=marcador2_=unidades_suciedad_=0;
    jugador_activo_=1;
    instante_=0;
}

Environment::Environment(ifstream &infile)
{
	string comment;
	getline(infile, comment);
	infile >> MAZE_SIZE >> agent1PosX_ >> agent1PosY_ >> agent1Rumbo_ >> agent2PosX_ >> agent2PosY_ >> agent2Rumbo_;
    agent1Giros_=agent2Giros_=0;

    marcador1_=0;
    marcador2_=0;
    unidades_suciedad_=0;
    jugador_activo_=1;
    instante_ = 0;

	maze_ = new int*[MAZE_SIZE];
	for (int i=0; i<MAZE_SIZE; i++)
	  maze_[i] = new int[MAZE_SIZE];

	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			int c;
			infile >> c;
			if (c > 0) {
   				maze_[row][col] = c-1;
   				unidades_suciedad_+=maze_[row][col];
			}
			else if (c == 0) {
   				maze_[row][col] = OBSTACLE;
			}
			else {
				cout << c << " is an unknown symbol in agent.map!"; exit(1);
			}
		}// for - col
	}// for - row

	bump1_ = false;
	bump2_ = false;
	preAction1_=actIDLE;
	preAction2_=actIDLE;
	suciedad_objetivo_= (unidades_suciedad_/2)+1;
}
// -----------------------------------------------------------
Environment::~Environment(){
    for (int i=0; i<MAZE_SIZE; i++)
      delete [] maze_[i];
    delete [] maze_;
}
// -----------------------------------------------------------
Environment::Environment(const Environment &env){
    MAZE_SIZE=env.MAZE_SIZE;
    agent1PosX_=env.agent1PosX_;
    agent1PosY_=env.agent1PosY_;
    agent2PosX_=env.agent2PosX_;
    agent2PosY_=env.agent2PosY_;
    agent1Rumbo_=env.agent1Rumbo_;
    agent2Rumbo_=env.agent2Rumbo_;
    agent1Giros_=env.agent1Giros_;
    agent2Giros_=env.agent2Giros_;
    jugador_activo_=env.jugador_activo_;
    instante_ =env.instante_;
    suciedad_objetivo_=env.suciedad_objetivo_;

	maze_ = new int*[MAZE_SIZE];
	for (int i=0; i<MAZE_SIZE; i++)
	  maze_[i] = new int[MAZE_SIZE];

    for (int i=0; i<MAZE_SIZE; i++)
      for (int j=0; j<MAZE_SIZE; j++)
        maze_[i][j]=env.maze_[i][j];

    marcador1_=env.marcador1_;
    marcador2_=env.marcador2_;
    unidades_suciedad_=env.unidades_suciedad_;
    bump1_=env.bump1_;
    bump2_=env.bump2_;
    preAction1_=env.preAction1_;
    preAction2_=env.preAction2_;
}
// -----------------------------------------------------------
Environment& Environment::operator=(const Environment& env)
{
    if (this == &env) return *this; // handle self assignment

    if (MAZE_SIZE>0){
      for (int i=0; i< MAZE_SIZE; i++)
        delete [] maze_[i];
      delete [] maze_;
    }

    MAZE_SIZE=env.MAZE_SIZE;
    agent1PosX_=env.agent1PosX_;
    agent1PosY_=env.agent1PosY_;
    agent2PosX_=env.agent2PosX_;
    agent2PosY_=env.agent2PosY_;
    agent1Rumbo_=env.agent1Rumbo_;
    agent2Rumbo_=env.agent2Rumbo_;
    agent1Giros_=env.agent1Giros_;
    agent2Giros_=env.agent2Giros_;
    jugador_activo_=env.jugador_activo_;
    instante_ = env.instante_;
    suciedad_objetivo_ = env.suciedad_objetivo_;

	maze_ = new int*[MAZE_SIZE];
	for (int i=0; i<MAZE_SIZE; i++)
	  maze_[i] = new int[MAZE_SIZE];

    for (int i=0; i<MAZE_SIZE; i++)
      for (int j=0; j<MAZE_SIZE; j++)
        maze_[i][j]=env.maze_[i][j];

    marcador1_=env.marcador1_;
    marcador2_=env.marcador2_;
    unidades_suciedad_=env.unidades_suciedad_;
    bump1_=env.bump1_;
    bump2_=env.bump2_;
    preAction1_=env.preAction1_;
    preAction2_=env.preAction2_;
}
// -----------------------------------------------------------
int Environment::DirtAmount(int x, int y) const
{
	if (maze_[x][y] <= 0) return 0;
	else return maze_[x][y];
}
// -----------------------------------------------------------
int Environment::DirtAmount() const
{
   int suciedad=0;

   for (int x=0; x<MAZE_SIZE; x++)
     for (int y=0; y<MAZE_SIZE; y++)
        if (maze_[x][y] > 0) suciedad+=maze_[x][y];

   return suciedad;
}
// -----------------------------------------------------------
void Environment::PintaAspiradora(int jugador, int w, int h, int x, int y)const {
    ActionType preAction=preAction1_;
    int agentPosX=agent1PosX_, agentPosY=agent1PosY_, agentRumbo=agent1Rumbo_;
    bool bump = bump1_;

    if (jugador==2){
        preAction = preAction2_;
        agentPosX=agent2PosX_;
        agentPosY=agent2PosY_;
        agentRumbo=agent2Rumbo_;
        bump = bump2_;
    }

 	 float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
    float size=0.8*length;

    switch(preAction){
						case Environment::actIDLE:
   						    switch (agentRumbo){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);

								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);

								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);

								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);

								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
							break;

			case Environment::actFORWARD:
						if(bump){
							glColor3f(1.0,0.0,0.0);
						}
						else{
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
						}
   						    switch (agentRumbo){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
						break;
			case Environment::actSHOOT:
						    if (jugador==1)
							  glColor3f(0.8,1.0,0.0);
                            else
							  glColor3f(0.8,0.0,1.0);

   						    switch (agentRumbo){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
						break;

         case Environment::actTURN_L:
         case Environment::actTURN_R:
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
  						    switch (agentRumbo){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }

							  break;
	}


}

// -----------------------------------------------------------
void Environment::Show(int w,int h) const
{
    int **mapa = new int*[MAZE_SIZE];
    for (int i=0; i<MAZE_SIZE; i++)
      mapa[i] = new int[MAZE_SIZE];

    for (int i=0; i<MAZE_SIZE; i++)
      for (int j=0; j<MAZE_SIZE; j++)
        mapa[i][j]=maze_[i][j];

    //Pintar el tiro si hay un disparo
    int pos;
    if (jugador_activo_==2 and preAction1_==actSHOOT){
        switch(agent1Rumbo_){
            case 0: pos=1;
                    while (mapa[agent1PosX_-pos][agent1PosY_]>=0 and (agent1PosX_-pos!=agent2PosX_ or agent1PosY_!=agent2PosY_ )){
                        mapa[agent1PosX_-pos][agent1PosY_]=-8;
                        pos++;
                    }
                    break;
            case 1: pos=1;
                    while (mapa[agent1PosX_][agent1PosY_+pos]>=0 and (agent1PosX_!=agent2PosX_ or agent1PosY_+pos!=agent2PosY_ )){
                        mapa[agent1PosX_][agent1PosY_+pos]=-8;
                        pos++;
                    }
                    break;
            case 2: pos=1;
                    while (mapa[agent1PosX_+pos][agent1PosY_]>=0 and (agent1PosX_+pos!=agent2PosX_ or agent1PosY_!=agent2PosY_ )){
                        mapa[agent1PosX_+pos][agent1PosY_]=-8;
                        pos++;
                    }
                    break;
            case 3: pos=1;
                    while (mapa[agent1PosX_][agent1PosY_-pos]>=0 and (agent1PosX_!=agent2PosX_ or agent1PosY_-pos!=agent2PosY_ )){
                        mapa[agent1PosX_][agent1PosY_-pos]=-8;
                        pos++;
                    }
                    break;
        }

    }

    else if (jugador_activo_==1 and preAction2_==actSHOOT){
        switch(agent2Rumbo_){
            case 0: pos=1;
                    while (mapa[agent2PosX_-pos][agent2PosY_]>=0 and (agent2PosX_-pos!=agent1PosX_ or agent1PosY_!=agent2PosY_ )){
                        mapa[agent2PosX_-pos][agent2PosY_]=-8;
                        pos++;
                    }
                    break;
            case 1: pos=1;
                    while (mapa[agent2PosX_][agent2PosY_+pos]>=0 and (agent1PosX_!=agent2PosX_ or agent2PosY_+pos!=agent1PosY_ )){
                        mapa[agent2PosX_][agent2PosY_+pos]=-8;
                        pos++;
                    }
                    break;
            case 2: pos=1;
                    while (mapa[agent2PosX_+pos][agent2PosY_]>=0 and (agent2PosX_+pos!=agent1PosX_ or agent1PosY_!=agent2PosY_ )){
                        mapa[agent2PosX_+pos][agent2PosY_]=-8;
                        pos++;
                    }
                    break;
            case 3: pos=1;
                    while (mapa[agent2PosX_][agent2PosY_-pos]>=0 and (agent1PosX_!=agent2PosX_ or agent2PosY_-pos!=agent1PosY_ )){
                        mapa[agent2PosX_][agent2PosY_-pos]=-8;
                        pos++;
                    }
                    break;
        }

    }


	const static float dirty_level=10.0;
	float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			float x=(col-MAZE_SIZE/2)*length*2+length,y=(MAZE_SIZE/2-row)*length*2-length;
			if (mapa[row][col] == OBSTACLE){
				//cout << "XXXXXX";

				glBegin(GL_POLYGON);
					glColor3f(0.5,0.25,0.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else if (mapa[row][col] == OBSTACLE_VERDE){
				//cout << "XXXXXX";

				glBegin(GL_POLYGON);
					glColor3f(0.3,1.0,0.3);
					glVertex2f(x-length/2,y-length/2);	glVertex2f(x+length/2,y-length/2);
					glVertex2f(x+length/2,y+length/2);	glVertex2f(x-length/2,y+length/2);
				glEnd();

				glBegin(GL_POLYGON);
					glColor3f(1.0,1.0,1.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else if (mapa[row][col] == OBSTACLE_AZUL){
				//cout << "XXXXXX";


				glBegin(GL_POLYGON);
					glColor3f(0.3,0.3,1.0);
					glVertex2f(x-length/2,y-length/2);	glVertex2f(x+length/2,y-length/2);
					glVertex2f(x+length/2,y+length/2);	glVertex2f(x-length/2,y+length/2);
				glEnd();

				glBegin(GL_POLYGON);
					glColor3f(1.0,1.0,1.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else if (mapa[row][col] == -8){
				//cout << "XXXXXX";


				glBegin(GL_POLYGON);
					glColor3f(0.0,0.0,0.0);
					glVertex2f(x-length/8,y-length/8);	glVertex2f(x+length/8,y-length/8);
					glVertex2f(x+length/8,y+length/8);	glVertex2f(x-length/8,y+length/8);
				glEnd();

				glBegin(GL_POLYGON);
					glColor3f(1.0,1.0,1.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else
			{
				char symbol = ' ';
   				if (row == agent1PosX_ && col == agent1PosY_){
					symbol = '=';
					PintaAspiradora(1,w,h,x,y);
				}
				else if (row == agent2PosX_ && col == agent2PosY_){
					    symbol = '=';
					    PintaAspiradora(2,w,h,x,y);
				     }
				glBegin(GL_POLYGON);
					glColor3f(((float)(dirty_level-maze_[row][col]))/dirty_level,((float)(dirty_level-maze_[row][col]))/dirty_level,((float)(dirty_level-maze_[row][col]))/dirty_level);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
				//cout << symbol;
				//cout << setw(4) << maze_[row][col];
				//cout << symbol;

			}
		}// for - col
		//cout << endl;
	}// for - row

	for (int i=0; i<MAZE_SIZE; i++)
	  delete [] mapa[i];
    delete [] mapa;
}
// -----------------------------------------------------------
void Environment::Change()
{
}
// -----------------------------------------------------------
void Environment::AcceptAction(Environment::ActionType action)
{
    //cout << "AcceptAction: " << ActionStr(action) << endl;
    int agentPosX=agent1PosX_, agentPosY=agent1PosY_, agentRumbo =agent1Rumbo_;
    int elotroPosX=agent2PosX_, elotroPosY=agent2PosY_, elotroRumbo=agent2Rumbo_;
    int agentGiros=agent1Giros_, elotroGiros=agent2Giros_;
    int marcador =marcador1_, elotromarcador=marcador2_;

    if (jugador_activo_==2){
        agentPosX = agent2PosX_;
        agentPosY = agent2PosY_;
        agentRumbo = agent2Rumbo_;
        elotroPosX = agent1PosX_;
        elotroPosY = agent1PosY_;
        elotroRumbo = agent1Rumbo_;
        agentGiros= agent2Giros_;
        elotroGiros=agent1Giros_;
        marcador = marcador2_;
        elotromarcador=marcador1_;
    }
	bool bump = false;

	switch (action)
	{
		case Environment::actFORWARD:
		    switch(agentRumbo){
                case 0: // Orientacion Norte
			            if (maze_[agentPosX-1][agentPosY] >= 0 and (agentPosX!=elotroPosX or agentPosY!=elotroPosY ) ){
			                agentPosX-=1;
			                agentGiros=0;
			            }
			            else bump = true;
			            break;
			       case 1: // Orientacion Este
			            if (maze_[agentPosX][agentPosY+1] >= 0 and (agentPosX!=elotroPosX or agentPosY!=elotroPosY )){
			                agentPosY+=1;
			                agentGiros=0;
			            }
			            else bump = true;
			            break;
                case 2: // Orientacion Sur
			            if (maze_[agentPosX+1][agentPosY] >= 0 and (agentPosX!=elotroPosX or agentPosY!=elotroPosY )){
			                agentPosX+=1;
			                agentGiros=0;
			            }
			            else bump = true;
                        break;
                case 3: // Orientacion Oeste
			            if (maze_[agentPosX][agentPosY-1] >= 0 and (agentPosX!=elotroPosX or agentPosY!=elotroPosY )){
			                agentPosY-=1;
			                agentGiros=0;
			            }
			            else bump = true;
                        break;

		    }
		    break;
		case Environment::actTURN_L:
		    agentRumbo = (agentRumbo+3)%4;
		    agentGiros++;
			break;
		case Environment::actTURN_R:
		    agentRumbo= (agentRumbo+1)%4;
		    agentGiros++;
			break;
		case Environment::actSHOOT:
		    switch(agentRumbo){
                case 0: // Orientacion Norte
                        if (marcador>=0) {
                          agentGiros=0;
                          int i=1;
                          while (maze_[agentPosX-i][agentPosY]>=0 and (agentPosX-i!=elotroPosX or agentPosY!=elotroPosY ))
                            i++;

                          if (agentPosX-i==elotroPosX and agentPosY==elotroPosY ){
                             //marcador--;
                             elotromarcador=(elotromarcador/2);
                          }
                          else if (maze_[agentPosX-i][agentPosY]==OBSTACLE_VERDE or maze_[agentPosX-i][agentPosY]==OBSTACLE_AZUL){
                                  //marcador--;
                                  maze_[agentPosX-i][agentPosY]=0;
                               }
                               else{
                                  //marcador--;
                               }
                        }
			            break;
			    case 1: // Orientacion Este
                        if (marcador>=0) {
                          agentGiros=0;
                          int i=1;
                          while (maze_[agentPosX][agentPosY+i]>=0 and (agentPosX!=elotroPosX or agentPosY+i!=elotroPosY ))
                            i++;

                          if (agentPosX==elotroPosX and agentPosY+i==elotroPosY ){
                             //marcador--;
                             elotromarcador=(elotromarcador/2);
                          }
                          else if (maze_[agentPosX][agentPosY+i]==OBSTACLE_VERDE or maze_[agentPosX][agentPosY+i]==OBSTACLE_AZUL){
                                  //marcador--;
                                  maze_[agentPosX][agentPosY+i]=0;
                               }
                               else{
                                  //marcador--;
                               }
                        }
			            break;
                case 2: // Orientacion Sur
                        if (marcador>=0) {
                          agentGiros=0;
                          int i=1;
                          while (maze_[agentPosX+i][agentPosY]>=0 and (agentPosX+i!=elotroPosX or agentPosY!=elotroPosY ))
                            i++;

                          if (agentPosX+i==elotroPosX and agentPosY==elotroPosY ){
                             //marcador--;
                             elotromarcador=(elotromarcador/2);
                          }
                          else if (maze_[agentPosX+i][agentPosY]==OBSTACLE_VERDE or maze_[agentPosX+i][agentPosY]==OBSTACLE_AZUL){
                                  //marcador--;
                                  maze_[agentPosX+i][agentPosY]=0;
                               }
                               else{
                                  //marcador--;
                               }
                        }
                        break;
                case 3: // Orientacion Oeste
                        if (marcador>=0) {
                          agentGiros=0;
                          int i=1;
                          while (maze_[agentPosX][agentPosY-i]>=0 and (agentPosX!=elotroPosX or agentPosY-i!=elotroPosY ))
                            i++;

                          if (agentPosX==elotroPosX and agentPosY-i==elotroPosY ){
                             //marcador--;
                             elotromarcador=(elotromarcador/2);
                          }
                          else if (maze_[agentPosX][agentPosY-i]==OBSTACLE_VERDE or maze_[agentPosX][agentPosY-i]==OBSTACLE_AZUL){
                                  //marcador--;
                                  maze_[agentPosX][agentPosY-i]=0;
                               }
                               else{
                                  //marcador--;
                               }
                        }

		    }

	}

    instante_++;

	if (jugador_activo_==1){
	  preAction1_=action;
	  bump1_=bump;
	  if (!bump and action==Environment::actFORWARD){
	      maze_[agent1PosX_][agent1PosY_]= OBSTACLE_VERDE;
	  }
	  agent1PosX_=agentPosX;
	  agent1PosY_=agentPosY;
	  agent1Rumbo_=agentRumbo;
      agent1Giros_=agentGiros;
	  marcador1_=marcador;
	  marcador2_=elotromarcador;
	  if (maze_[agentPosX][agentPosY]>0){
	      marcador1_+=maze_[agentPosX][agentPosY];
	      unidades_suciedad_-=maze_[agentPosX][agentPosY];
	      maze_[agentPosX][agentPosY]=0;
	  }
	}
    else{
	  preAction2_=action;
	  bump2_=bump;
	  if (!bump and action==Environment::actFORWARD){
	      maze_[agent2PosX_][agent2PosY_]= OBSTACLE_AZUL;
	  }
	  agent2PosX_=agentPosX;
	  agent2PosY_=agentPosY;
	  agent2Rumbo_=agentRumbo;
	  agent2Giros_=agentGiros;
	  marcador2_=marcador;
	  marcador1_=elotromarcador;
	  if (maze_[agentPosX][agentPosY]>0){
	      marcador2_+=maze_[agentPosX][agentPosY];
	      unidades_suciedad_-=maze_[agentPosX][agentPosY];
	      maze_[agentPosX][agentPosY]=0;
	  }
    }

}
// -----------------------------------------------------------
int Environment::Marcador(int jug) const {
    if (jug==1)
       return marcador1_;
    else
       return marcador2_;
}
// -----------------------------------------------------------
int Environment::Total_Suciedad() const {
    return unidades_suciedad_;
}
// -----------------------------------------------------------
int Environment::possible_actions(bool *act) const{
   int agentPosX=agent1PosX_, agentPosY=agent1PosY_, agentRumbo=agent1Rumbo_;
   int elotroPosX=agent2PosX_, elotroPosY=agent2PosY_, elotroRumbo=agent2Rumbo_;
   int agentGiros=agent1Giros_, elotroGiros=agent2Giros_;

   act[3]=false; // actIDLE siempre es FALSE

   if (jugador_activo_==2){
        agentPosX = agent2PosX_;
        agentPosY = agent2PosY_;
        agentRumbo = agent2Rumbo_;
        agentGiros = agent2Giros_;
        elotroPosX = agent1PosX_;
        elotroPosY = agent1PosY_;
        elotroRumbo = agent1Rumbo_;
        elotroGiros = agent1Giros_;
   }

    // Avanzar
		    switch(agentRumbo){
                case 0: // Orientacion Norte
			            if (maze_[agentPosX-1][agentPosY] >= 0 and (agentPosX-1!=elotroPosX or agentPosY!=elotroPosY ) )
			              act[0]=true;
			            else
                          act[0]=false;
			            break;
			    case 1: // Orientacion Este
			            if (maze_[agentPosX][agentPosY+1] >= 0 and (agentPosX!=elotroPosX or agentPosY+1!=elotroPosY ))
			              act[0]=true;
			            else
                          act[0]=false;
			            break;
                case 2: // Orientacion Sur
			            if (maze_[agentPosX+1][agentPosY] >= 0 and (agentPosX+1!=elotroPosX or agentPosY!=elotroPosY ))
			              act[0]=true;
			            else
                          act[0]=false;
                        break;
                case 3: // Orientacion Oeste
			            if (maze_[agentPosX][agentPosY-1] >= 0 and (agentPosX!=elotroPosX or agentPosY-1!=elotroPosY ))
			              act[0]=true;
			            else
                          act[0]=false;
                        break;

		    }
		    if (agentGiros<=4){
		      act[1]=act[2]=true;
		    }
		    else {
              act[1]=act[2]=false;
		    }
    // Disparar
		    switch(agentRumbo){
                case 0: // Orientacion Norte
                        if (marcador1_>=0) {
                          int i=1;
                          while (maze_[agentPosX-i][agentPosY]>=0 and (agentPosX-i!=elotroPosX or agentPosY!=elotroPosY ))
                            i++;

                          if (maze_[agentPosX-i][agentPosY]==-1)
                            act[4]=false;
                          else{
                            act[4]=true;
                            }
                        }
                        else
                            act[4]=false;
			            break;
			    case 1: // Orientacion Este
                        if (marcador1_>=0) {
                          int i=1;
                          while (maze_[agentPosX][agentPosY+i]>=0 and (agentPosX!=elotroPosX or agentPosY+i!=elotroPosY ))
                            i++;

                          if (maze_[agentPosX][agentPosY+i]==-1)
                            act[4]=false;
                          else{
                            act[4]=true;
                          }
                        }
                        else
                            act[4]=false;
			            break;
                case 2: // Orientacion Sur
                        if (marcador1_>=0) {
                          int i=1;
                          while (maze_[agentPosX+i][agentPosY]>=0 and (agentPosX+i!=elotroPosX or agentPosY!=elotroPosY ))
                            i++;

                          if (maze_[agentPosX+i][agentPosY]==-1)
                            act[4]=false;
                          else{
                            act[4]=true;
                          }
                        }
                        else
                            act[4]=false;
                        break;
                case 3: // Orientacion Oeste
                        if (marcador1_>=0) {
                          int i=1;
                          while (maze_[agentPosX][agentPosY-i]>=0 and (agentPosX!=elotroPosX or agentPosY-i!=elotroPosY ))
                            i++;

                          if (maze_[agentPosX][agentPosY-i]==-1)
                            act[4]=false;
                          else{
                            act[4]=true;
                           }
                        }
                        else
                            act[4]=false;
                        break;

		    }

    int n_act=0;
    for (int i=0; i<5; i++)
      if (act[i])
        n_act++;

    return n_act;
}

//------------------------------------------------------------------------

int Environment::GenerateAllMoves(Environment *V) const{
  bool act[6];
  int n_act=0;
  possible_actions(act);
  for (int i=0; i<5; i++){
    if (act[i]){
       V[n_act] = *this;
       V[n_act].AcceptAction(static_cast<ActionType> (i));
       V[n_act].ChangePlayer();
       n_act++;
    }
  }
  return n_act;
}

Environment Environment::GenerateNextMove(int &act) const{
    bool posibles[6];
    possible_actions(posibles);
    act++;
    while (act<5 and !posibles[act])
      act++;
    if (act>4){
        return *this;
    }
    else{
        Environment nuevo = *this;
        nuevo.AcceptAction(static_cast<ActionType> (act));
        nuevo.ChangePlayer();
        return nuevo;
    }
}

// -----------------------------------------------------------
int Environment::Last_Action(int jug) const{
    if (jug==1)
      return preAction1_;
    else
      return preAction2_;
}

// -----------------------------------------------------------
bool Environment::operator==(const Environment & env) const{
    if (agent1PosX_!=env.agent1PosX_ or
        agent1PosY_!=env.agent1PosY_ or
        agent1Rumbo_!=env.agent1Rumbo_ or
        agent2PosX_!=env.agent2PosX_ or
        agent2PosY_!=env.agent2PosY_ or
        agent2Rumbo_!=env.agent2Rumbo_ or
        marcador1_ != env.marcador1_ or
        marcador2_ != env.marcador2_ or
        jugador_activo_!=env.jugador_activo_ or
        unidades_suciedad_ != env.unidades_suciedad_)
        return false;
    else {
        bool iguales=true;
        for (int i=0; i<MAZE_SIZE and iguales; i++)
          for (int j=0; j<MAZE_SIZE and iguales; j++)
            if (maze_[i][j]!=env.maze_[i][j])
              iguales=false;
        return iguales;
    }
}



string Environment::ActionStr(ActionType action)
{
	switch (action)
	{
	case Environment::actFORWARD: return "FORWARD";
	case Environment::actTURN_L: return "TURN_LEFT";
	case Environment::actTURN_R: return "TURN_RIGHT";
	//case Environment::actSUCK: return "SUCK";
	case Environment::actIDLE: return "IDLE";
	case Environment::actSHOOT: return "SHOOT";
	default: return "???";
	}
}

