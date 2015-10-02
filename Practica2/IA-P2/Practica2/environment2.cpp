
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

Environment::Environment(ifstream &infile)
{
	string comment;
	getline(infile, comment);
	infile >> MAZE_SIZE >> agentPosX_ >> agentPosY_ >> dirtyProb_ >> orientacion_;
	randomSeed_=0;

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
			}
			else if (c == 0) {
   				maze_[row][col] = OBSTACLE;
			}
			else {
				cout << c << " is an unknown symbol in agent.map!"; exit(1);
			}
		}// for - col
	}// for - row

	bump_ = false;
	preAction_=actIDLE;
}
// -----------------------------------------------------------
Environment::~Environment(){
    for (int i=0; i<MAZE_SIZE;i++)
      delete [] maze_[i];
    delete [] maze_;

    MAZE_SIZE=0;
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
  int total=0;

  for (int x=0; x<MAZE_SIZE; x++)
    for (int y=0; y<MAZE_SIZE; y++)
	  if (maze_[x][y] != OBSTACLE)
	     total+=maze_[x][y];

  return total;
}
// -----------------------------------------------------------
void Environment::Show(int w,int h) const
{
	/*const static float dirty_level=50.0;
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
			else
			{
				char symbol = ' ';
   				if (row == agentPosX_ && col == agentPosY_){
					float size=0.8*length;
					symbol = '=';
					switch(preAction_){
						case Environment::actIDLE:
   						    switch (Orientation()){
   						      case 0: // Orientacion Norte
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x,y+size);
								     glVertex2f(x+size,y-size);	glVertex2f(x-size,y-size);
							         glEnd();
							         break;
                              case 1: // Orientacion Este
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x+size,y);
								     glVertex2f(x-size,y-size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 2: // Orientacion Sur
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x,y-size);
								     glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
								     glEnd();
								     break;
                              case 3: // Orientacion Oeste
							         glBegin(GL_POLYGON);
   						             glColor3f(0.0,0.0,1.0);
								     glVertex2f(x-size,y);
								     glVertex2f(x+size,y-size);	glVertex2f(x+size,y+size);
								     glEnd();
								     break;
   						    }
							break;
						case Environment::actSUCK:
							glBegin(GL_POLYGON);
								glColor3f(1.0,0.8,0.3);
								glVertex2f(x-size,y-size);	glVertex2f(x+size,y-size);
								glVertex2f(x+size,y+size);	glVertex2f(x-size,y+size);
							glEnd();
							break;
						case Environment::actFORWARD:
							if(bump_){
								glColor3f(1.0,0.0,0.0);
							}
							else{
								glColor3f(0.0,1.0,0.0);
							}
   						    switch (Orientation()){
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
								glColor3f(0.0,1.0,0.0);
   						    switch (Orientation()){
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
	}// for - row*/
}
// -----------------------------------------------------------
void Environment::Change()
{
}
// -----------------------------------------------------------
void Environment::AcceptAction(Environment::ActionType action)
{
	bump_ = false;

	switch (action)
	{
		case Environment::actSUCK:
			if(maze_[agentPosX_][agentPosY_] > 0){
				maze_[agentPosX_][agentPosY_] -= 1;
			}
			break;
		case Environment::actFORWARD:
		    switch(Orientation()){
                case 0: // Orientacion Norte
			            if (maze_[agentPosX_-1][agentPosY_] != OBSTACLE) agentPosX_-=1;
			            else bump_ = true;
			            break;
			       case 1: // Orientacion Este
			            if (maze_[agentPosX_][agentPosY_+1] != OBSTACLE) agentPosY_+=1;
			            else bump_ = true;
			            break;
                case 2: // Orientacion Sur
			            if (maze_[agentPosX_+1][agentPosY_] != OBSTACLE) agentPosX_+=1;
			            else bump_ = true;
                        break;
                case 3: // Orientacion Oeste
			            if (maze_[agentPosX_][agentPosY_-1] != OBSTACLE) agentPosY_-=1;
			            else bump_ = true;
                        break;

		    }
		    break;
		case Environment::actTURN_L:
		    orientacion_ = (orientacion_+3)%4;
			break;
		case Environment::actTURN_R:
		    orientacion_ = (orientacion_+1)%4;
			break;
	}
	preAction_=action;
}
// -----------------------------------------------------------
int ** Environment::SeeMap() const{
    return maze_;
}

