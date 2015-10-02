
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
#include <string>
using std::string;
#include "stdlib.h"

#include "environment.h"
#include"include/GL/glui.h"

Environment::Environment(){
    MAZE_SIZE = 0;
    agent1PosX_=agent1PosY_=agent2PosX_=agent2PosY_=0;
    maze_=0;
    marcador1_=marcador2_=unidades_suciedad_=0;
}

Environment::Environment(ifstream &infile)
{
	string comment;
	getline(infile, comment);
	infile >> MAZE_SIZE >> agent1PosX_ >> agent1PosY_ >> agent2PosX_ >> agent2PosY_;
    marcador1_=0;
    marcador2_=0;
    unidades_suciedad_=0;

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
	if (maze_[x][y] == OBSTACLE) return 0;
	else return maze_[x][y];
}
// -----------------------------------------------------------
int Environment::DirtAmount() const
{
    int suciedad=0;

    for (int x=0; x<MAZE_SIZE; x++)
      for (int y=0; y<MAZE_SIZE; y++)
	    if (maze_[x][y] >0)
	      suciedad+=maze_[x][y];

	return suciedad;
}
// -----------------------------------------------------------
void Environment::PintaAspiradora(int jugador, int w, int h, int x, int y)const {
    ActionType preAction=preAction1_;
    int agentPosX=agent1PosX_, agentPosY=agent1PosY_;
    bool bump = bump1_;

    if (jugador==2){
        preAction = preAction2_;
        agentPosX=agent2PosX_;
        agentPosY=agent2PosY_;
        bump = bump2_;
    }

 	float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
    float size=0.8*length;

    switch(preAction){
			case Environment::actIDLE:
						glBegin(GL_POLYGON);
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);

							glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
							glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
						glEnd();
						break;
			case Environment::actSUCK:
                        glBegin(GL_POLYGON);
							glColor3f(1.0,0.8,0.3);
							glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
							glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
						glEnd();
						break;
			case Environment::actUP:
						if(bump){
							glColor3f(1.0,0.0,0.0);
						}
						else{
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
						}
						glBegin(GL_POLYGON);
							glVertex2f(x,y+size);
							glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
						glEnd();
						break;
			case Environment::actDOWN:
						if(bump){
							glColor3f(1.0,0.0,0.0);
						}
						else{
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
						}
						glBegin(GL_POLYGON);
							glVertex2f(x,y-size);
							glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
						glEnd();
						break;
			case Environment::actLEFT:
						if(bump){
							glColor3f(1.0,0.0,0.0);
						}
						else{
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
						}
						glBegin(GL_POLYGON);
							glVertex2f(x-size,y);
							glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
						glEnd();
						break;
			case Environment::actRIGHT:
						if(bump){
							glColor3f(1.0,0.0,0.0);
						}
						else{
						    if (jugador==1)
							  glColor3f(0.0,1.0,0.0);
                            else
							  glColor3f(0.0,0.0,1.0);
						}
						glBegin(GL_POLYGON);
							glVertex2f(x+size,y);
							glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
						glEnd();
						break;
	}


}

// -----------------------------------------------------------
void Environment::Show(int w,int h) const
{
	const static float dirty_level=10.0;
	float length=(float)(w>h?h:w)/(float)(MAZE_SIZE+4);
	for (int row=0; row<MAZE_SIZE; row+=1)
	{
		for (int col=0; col<MAZE_SIZE; col+=1)
		{
			float x=(col-MAZE_SIZE/2)*length*2+length,y=(MAZE_SIZE/2-row)*length*2-length;
			if (maze_[row][col] == OBSTACLE){
				//cout << "XXXXXX";

				glBegin(GL_POLYGON);
					glColor3f(0.5,0.25,0.0);
					glVertex2f(x-length,y-length);	glVertex2f(x+length,y-length);
					glVertex2f(x+length,y+length);	glVertex2f(x-length,y+length);
				glEnd();
			}
			else if (maze_[row][col] == OBSTACLE_VERDE){
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
			else if (maze_[row][col] == OBSTACLE_AZUL){
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
}
// -----------------------------------------------------------
void Environment::Change()
{
}
// -----------------------------------------------------------
void Environment::AcceptAction(int jugador, Environment::ActionType action)
{
    int agentPosX=agent1PosX_, agentPosY=agent1PosY_, elotroPosX=agent2PosX_, elotroPosY=agent2PosY_;

    if (jugador==2){
        agentPosX = agent2PosX_;
        agentPosY = agent2PosY_;
        elotroPosX = agent1PosX_;
        elotroPosY = agent1PosY_;
    }
	bool bump = false;
	switch (action)
	{
		case Environment::actSUCK:
			if(maze_[agentPosX][agentPosY] > 0){
				maze_[agentPosX][agentPosY] -= 1;
			}
			break;
		case Environment::actUP:
			if (maze_[agentPosX-1][agentPosY] >= 0 and (agentPosX-1!=elotroPosX or agentPosY!=elotroPosY) ) agentPosX-=1;
			else bump = true;
			break;
		case Environment::actDOWN:
			if (maze_[agentPosX+1][agentPosY] >= 0 and (agentPosX+1!=elotroPosX or agentPosY!=elotroPosY) ) agentPosX+=1;
			else bump = true;
			break;
		case Environment::actLEFT:
			if (maze_[agentPosX][agentPosY-1] >= 0 and (agentPosX!=elotroPosX or agentPosY-1!=elotroPosY) ) agentPosY-=1;
			else bump = true;
			break;
		case Environment::actRIGHT:
			if (maze_[agentPosX][agentPosY+1] >= 0 and (agentPosX!=elotroPosX or agentPosY+1!=elotroPosY) ) agentPosY+=1;
			else bump = true;
			break;
        default:
            bump = true;
	}

	if (jugador==1){
	  preAction1_=action;
	  bump1_=bump;
	  if (!bump1_ and action!=Environment::actIDLE and action!=Environment::actSUCK){
	      maze_[agent1PosX_][agent1PosY_]= OBSTACLE_VERDE;
	  }
	  agent1PosX_=agentPosX;
	  agent1PosY_=agentPosY;
	  if (maze_[agentPosX][agentPosY]>0){
	      marcador1_+=maze_[agentPosX][agentPosY];
	      unidades_suciedad_-=maze_[agentPosX][agentPosY];
	      maze_[agentPosX][agentPosY]=0;
	  }
	}
    else{
	  preAction2_=action;
	  bump2_=bump;
	  if (!bump2_ and action!=Environment::actIDLE and action!=Environment::actSUCK){
	      maze_[agent2PosX_][agent2PosY_]= OBSTACLE_AZUL;
	  }
	  agent2PosX_=agentPosX;
	  agent2PosY_=agentPosY;
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
void Environment::possible_actions(bool *act, int jug) const{
    if (jug==1){
    // Mover hacia arriba
      if ( maze_[agent1PosX_-1][agent1PosY_] >= 0 and (agent1PosX_-1!=agent2PosX_ or agent1PosY_!=agent2PosY_) )
        act[0]=true;
      else
        act[0]=false;

    // Mover hacia abajo
      if ( maze_[agent1PosX_+1][agent1PosY_] >= 0 and (agent1PosX_+1!=agent2PosX_ or agent1PosY_!=agent2PosY_) )
        act[1]=true;
      else
        act[1]=false;

    // Mover izquierda
      if ( maze_[agent1PosX_][agent1PosY_-1] >= 0 and (agent1PosX_!=agent2PosX_ or agent1PosY_-1!=agent2PosY_) )
        act[2]=true;
      else
        act[2]=false;

    // Mover derecha
      if ( maze_[agent1PosX_][agent1PosY_+1] >= 0 and (agent1PosX_!=agent2PosX_ or agent1PosY_+1!=agent2PosY_) )
        act[3]=true;
      else
        act[3]=false;
    }
    else {
      if ( maze_[agent2PosX_-1][agent2PosY_] >= 0 and (agent2PosX_-1!=agent1PosX_ or agent2PosY_!=agent1PosY_) )
        act[0]=true;
      else
        act[0]=false;

    // Mover hacia abajo
      if ( maze_[agent2PosX_+1][agent2PosY_] >= 0 and (agent2PosX_+1!=agent1PosX_ or agent2PosY_!=agent1PosY_) )
        act[1]=true;
      else
        act[1]=false;

    // Mover izquierda
      if ( maze_[agent2PosX_][agent2PosY_-1] >= 0 and (agent2PosX_!=agent1PosX_ or agent2PosY_-1!=agent1PosY_) )
        act[2]=true;
      else
        act[2]=false;

    // Mover derecha
      if ( maze_[agent2PosX_][agent2PosY_+1] >= 0 and (agent2PosX_!=agent1PosX_ or agent2PosY_+1!=agent1PosY_) )
        act[3]=true;
      else
        act[3]=false;
    }

}

//------------------------------------------------------------------------

int Environment::GenerateNextMove(Environment *V, int jug) const{
  bool act[6];
  int n_act=0;
  possible_actions(act, jug);
  for (int i=0; i<4; i++){
    if (act[i]){
       V[n_act] = *this;
       V[n_act].AcceptAction(jug, static_cast<ActionType> (i));
       n_act++;
    }
  }
  return n_act;
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
        agent2PosX_!=env.agent2PosX_ or
        agent2PosY_!=env.agent2PosY_ or
        marcador1_ != env.marcador1_ or
        marcador2_ != env.marcador2_ or
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

bool Environment::a_movement_is_possible(int jug) const{
  bool act[6];
  possible_actions(act,jug);
  int i=0;
  while (i<4 and !act[i])
    i++;

  return (i<4);

}
