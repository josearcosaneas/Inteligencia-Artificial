#include <iostream>
#include "state.h"
#include "environment.h"
#include <list>
#include <queue>
#include <cmath>
#include <iostream>

using namespace std;

state::state(const Environment &env)
{
    Tam_Y = Tam_X = env.SizeMaze();
    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    int **m = env.SeeMap();
    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=m[i][j];

    posX = env.Position_X();
    posY = env.Position_Y();
    Consumed_Energy = 0;
    Pending_Dirty = env.DirtAmount();
    last_action=5;
    g = h = f = 0;
    orientacion=env.Orientation();
 }

state::~state()
{
    for (int i=0; i< Tam_Y; i++)
      delete [] mundo[i];
    delete [] mundo;

    Tam_X = Tam_Y = 0;
}

state::state(const state& other)
{
    Tam_X = other.Tam_X;
    Tam_Y = other.Tam_Y;
    posX = other.posX;
    posY = other.posY;
    Consumed_Energy = other.Consumed_Energy;
    Pending_Dirty = other.Pending_Dirty;
    last_action = other.last_action;
    pos_padre = other.pos_padre;
    g = other.g;
    h = other.h;
    f = other.f;
    orientacion = other.orientacion;

    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=other.mundo[i][j];

    road = other.road;

}

state& state::operator=(const state& other)
{
    if (this == &other) return *this; // handle self assignment

    if (Tam_Y!=0){
      for (int i=0; i< Tam_Y; i++)
        delete [] mundo[i];
      delete [] mundo;
    }

    // Copiar other en this
    Tam_X = other.Tam_X;
    Tam_Y = other.Tam_Y;
    posX = other.posX;
    posY = other.posY;
    Consumed_Energy = other.Consumed_Energy;
    Pending_Dirty = other.Pending_Dirty;
    last_action = other.last_action;
    pos_padre = other.pos_padre;
    g = other.g;
    h = other.h;
    f = other.f;
    orientacion = other.orientacion;


    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=other.mundo[i][j];


    road = other.road;

    return *this;
}

//------------------------------------------------------------------------
bool state::operator<(const state &other) const{
  return (f<other.f);
}

//------------------------------------------------------------------------
bool state::operator>(const state &other) const{
  return (f>other.f);
}

//------------------------------------------------------------------------
bool state::operator==(const state &other) const{
  if (posX!=other.posX)
    return false;
  else if (posY!=other.posY)
         return false;
       else if (orientacion!=other.orientacion)
               return false;
       else if (Pending_Dirty!=other.Pending_Dirty)
               return false;
            else {
               int x=0,y;
               bool casillas_distintas = false;
               while (x<Tam_X and !casillas_distintas){
                 y=0;
                 while (y<Tam_Y and mundo[x][y]==other.mundo[x][y])
                    y++;
                 if (y<Tam_Y)
                   casillas_distintas=true;
                 else
                   x++;
               }
              return !casillas_distintas;
            }
}


//------------------------------------------------------------------------
void state::Add_Step_in_Road(int new_action){
  road.push_back(new_action);
}
//------------------------------------------------------------------------

void state::AlterState(const state &other){
  g = other.g;
  road = other.road;
  pos_padre = other.pos_padre;
  last_action = other.last_action;
}
//------------------------------------------------------------------------

void state::Show_Road(){
  int longitud=0, energia=0;
  list<int>::iterator i;
  for (i=road.begin(); i!=road.end(); i++){
    cout << ">";
    switch (*i){
       case 0: cout << "F";
               break;
       case 1: cout << "L";
               break;
       case 2: cout << "R";
               break;
       case 3: cout << "S";
               break;
       case 4: cout << "NO_ACTION";
               break;
    }
    longitud++;
    if (*i == 1)
      energia+=2;
    else if (*i<4)
           energia+=1;

  }
  cout << endl;
  cout << "Longitud: " << longitud << endl;
  cout << "Energia: " << energia << endl;
}


//------------------------------------------------------------------------
void state::possible_actions(bool *act) const{

    // Aspirar basura
    if ( mundo[posX][posY] > 0){
      act[0]=false;
      act[1]=false;
      act[2]=false;
      act[3]=true;
      act[4]=false;
    }
    else{
       act[3]=false;
    //Moverse
    switch(orientacion){
        case 0: // Mover al norte
           if (mundo[posX-1][posY]!=-1)
             act[0]=true;
           else
             act[0]=false;
           break;
        case 1: // Mover al este
           if (mundo[posX][posY+1]!=-1)
             act[0]=true;
           else
             act[0]=false;
           break;
        case 2: // Mover al sur
           if (mundo[posX+1][posY]!=-1)
             act[0]=true;
           else
             act[0]=false;
           break;
        case 3: // Mover al oeste
           if (mundo[posX][posY-1]!=-1)
             act[0]=true;
           else
             act[0]=false;
           break;
    }

    // Girar izquierda y derecha

    act[1]=true;
    act[2]=true;
    // No hacer nada no se considera una accion util
    act[4]=false;
    }
}

//------------------------------------------------------------------------

int state::Generate_New_States(state *V) const{
  bool act[5];
  int n_act=0;
  possible_actions(act);
  for (int i=0; i<4; i++){
    if (act[i]){
       V[n_act] = *this;
       V[n_act].last_action=i;
       V[n_act].Add_Step_in_Road(i);
       switch(i){
           case 0: // Avanzar
                   switch(V[n_act].orientacion){
                     case 0: V[n_act].posX--;
                             break;
                     case 1: V[n_act].posY++;
                             break;
                     case 2: V[n_act].posX++;
                             break;
                     case 3: V[n_act].posY--;
                             break;
                   }
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   break;
           case 1: // Giro izquierda
                   V[n_act].orientacion = (V[n_act].orientacion+3)%4;
                   V[n_act].Consumed_Energy+=2;
                   V[n_act].g+=2;
                   break;
           case 2: // Giro Derecha
                   V[n_act].orientacion = (V[n_act].orientacion+1)%4;
                   V[n_act].Consumed_Energy+=1;
                   V[n_act].g+=1;
                   break;
           case 3: // Aspirar
                   V[n_act].Pending_Dirty--;
                   V[n_act].mundo[posX][posY]--;
                   V[n_act].Consumed_Energy+=1;
                   V[n_act].g+=1;
                   break;

       }
       V[n_act].h = 0;
       V[n_act].f = V[n_act].g + V[n_act].h;
       n_act++;
    }
  }
  return n_act;
}



state state::Generate_Next_State(int i) const{
  state next_state= *this;
  bool act[5];
  int n_act=0;
  possible_actions(act);


  if (i<4 and act[i]){
      next_state.last_action=i;
      next_state.Add_Step_in_Road(i);
      switch(i){
         case 0: // Avanzar
                   switch(next_state.orientacion){
                     case 0: next_state.posX--;
                             break;
                     case 1: next_state.posY++;
                             break;
                     case 2: next_state.posX++;
                             break;
                     case 3: next_state.posY--;
                             break;
                   }
                   next_state.Consumed_Energy++;
                   next_state.g++;
                   break;
          case 1: // Giro izquierda
                   next_state.orientacion = (next_state.orientacion+3)%4;
                   next_state.Consumed_Energy+=2;
                   next_state.g+=2;
                   break;
          case 2: // Giro Derecha
                   next_state.orientacion = (next_state.orientacion+1)%4;
                   next_state.Consumed_Energy+=1;
                   next_state.g+=1;
                   break;
          case 3: // Aspirar
                   next_state.Pending_Dirty--;
                   next_state.mundo[posX][posY]--;
                   next_state.Consumed_Energy+=1;
                   next_state.g+=1;
                   break;

       }
       next_state.h = 0;
       next_state.f = next_state.g + next_state.h;
       return next_state;
  }
  else {
    next_state.do_fail();
    return next_state;
  }
}


//------------------------------------------------------------------------

void state::Datos_Finales() const{
    cout << "Energia consumida: " << Consumed_Energy << endl;
    cout << "Suciedad Presente: " << Pending_Dirty << endl;

}


void state::PrintState() const{
  cout << "Pos(" << posX <<"," << posY <<") [ORI: " << orientacion <<"] Suciedad: "<< Pending_Dirty << " Accion: " << last_action <<endl;
  cout << "g()= " << g << endl;
  for (int x=0; x<Tam_X; x++){
    for (int y=0; y<Tam_Y; y++){
      if (mundo[x][y]==-1)
        cout << "X";
      else if (x==posX and y==posY){
          switch (orientacion){
              case 0: cout <<"^";
                      break;
              case 1: cout <<">";
                      break;
              case 2: cout <<"v";
                      break;
              case 3: cout <<"<";
                      break;
         }
      }
      else if (mundo[x][y]==0)
        cout << " ";
      else
        cout << mundo[x][y];
    }
    cout << endl;
  }
  cout << endl;
}

