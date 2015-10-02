#include <iostream>
#include "state.h"
#include "environment.h"
#include <list>
#include <queue>
#include <cmath>


using namespace std;

state::state(const Environment &env)
{
    Tam_Y = Tam_X = env.SizeMaze();

    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    camino = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      camino[i] = new int[Tam_X];

    int **m = env.SeeMap();
    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=m[i][j];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        camino[i][j]=0;

    posX = env.Position_X();
    posY = env.Position_Y();
    Consumed_Energy = 0;
    Pending_Dirty = env.DirtAmount();
    last_action=5;
     g = h = f = reinicia = 0;
 }

state::~state()
{
    for (int i=0; i< Tam_Y; i++)
      delete [] mundo[i];
    delete [] mundo;

    for (int i=0; i< Tam_Y; i++)
      delete [] camino[i];
    delete [] camino;

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
    reinicia = other.reinicia;

    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=other.mundo[i][j];

    camino = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      camino[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        camino[i][j]=other.camino[i][j];

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

    if (Tam_Y!=0){
      for (int i=0; i< Tam_Y; i++)
        delete [] camino[i];
      delete [] camino;
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
    reinicia = other.reinicia;

    mundo = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      mundo[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        mundo[i][j]=other.mundo[i][j];

    camino = new int*[Tam_Y];
    for (int i=0; i<Tam_Y; i++)
      camino[i]= new int [Tam_X];

    for (int i=0; i<Tam_Y; i++)
      for (int j=0; j<Tam_X; j++)
        camino[i][j]=other.camino[i][j];


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
    cout << "->";
    switch (*i){
       case 0: cout << "UP";
               break;
       case 1: cout << "DOWN";
               break;
       case 2: cout << "LEFT";
               break;
       case 3: cout << "RIGHT";
               break;
       case 4: cout << "SUCK";
               break;
       case 5: cout << "NO_ACTION";
               break;
    }
    longitud++;
    if (*i <4)
      energia++;
    else
      energia+=2;

  }
  cout << endl;
  cout << "Longitud: " << longitud << endl;
  cout << "Energia: " << energia << endl;
}

void state::possible_actions(bool *act) const{
    // Mover hacia arriba
    if ( last_action!= 1 and mundo[posX-1][posY] != -1)
      act[0]=true;
    else
      act[0]=false;

    // Mover hacia abajo
    if ( last_action!= 0 and mundo[posX+1][posY] != -1)
      act[1]=true;
    else
      act[1]=false;

    // Mover izquierda
    if ( last_action!= 3 and mundo[posX][posY-1] != -1)
      act[2]=true;
    else
      act[2]=false;

    // Mover derecha
    if ( last_action!= 2 and mundo[posX][posY+1] != -1)
      act[3]=true;
    else
      act[3]=false;

    // Aspirar basura
    if ( mundo[posX][posY] > 0)
      act[4]=true;
    else
      act[4]=false;

    // No hacer nada no se considera una accion util
    act[5]=false;
}

//------------------------------------------------------------------------
void state::possible_actions_escalada(bool *act, bool *lo_fue) const{
    // Mover hacia arriba
    if ( last_action!= 1 and mundo[posX-1][posY] != -1) {
      act[0]=true;

    }
    else {

      act[0]=false;
    }

    if (last_action == 1 and mundo[posX-1][posY] != -1)
        lo_fue[0] = true;
    else
        lo_fue[0] = false;

    // Mover hacia abajo
    if ( last_action!= 0 and mundo[posX+1][posY] != -1) {
      act[1]=true;
    }
    else {

      act[1]=false;
    }
    if (last_action == 0 and mundo[posX+1][posY] != -1)
        lo_fue[1] = true;
    else
        lo_fue[1] = false;
    // Mover izquierda
    if ( last_action!= 3 and mundo[posX][posY-1] != -1) {

      act[2]=true;
    }
    else {

      act[2]=false;
    }
if (last_action == 3 and mundo[posX][posY-1] != -1)
        lo_fue[2] = true;
    else
        lo_fue[2] = false;
    // Mover derecha
    if ( last_action!= 2 and mundo[posX][posY+1] != -1) {

      act[3]=true;
    }
    else {

      act[3]=false;
    }
if (last_action == 2 and mundo[posX][posY+1] != -1)
        lo_fue[3] = true;
    else
        lo_fue[3] = false;
    // Aspirar basura
    if ( mundo[posX][posY] > 0)
      act[4]=true;
    else
      act[4]=false;

    // No hacer nada no se considera una accion util
    act[5]=false;
    lo_fue[5] = false;
    lo_fue[4] = false;
}


int state::Generate_New_States(state *V) const{
  bool act[6];
  int n_act=0;
  possible_actions(act);
  for (int i=0; i<5; i++){
    if (act[i]){
       V[n_act] = *this;
       V[n_act].last_action=i;
       V[n_act].Add_Step_in_Road(i);
       switch(i){
           case 0: V[n_act].posX--;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   break;
           case 1: V[n_act].posX++;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   break;
           case 2: V[n_act].posY--;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   break;
           case 3: V[n_act].posY++;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   break;
           case 4: V[n_act].Pending_Dirty--;
                   V[n_act].mundo[posX][posY]--;
                   V[n_act].Consumed_Energy+=2;
                   V[n_act].g+=2;
                   break;

       }
       V[n_act].h = 0;
       V[n_act].f = V[n_act].g + V[n_act].h;
       n_act++;
    }
  }
  return n_act;
}

//------------------------------------------------------------------------

int state::Generate_New_States_escalada(state *V) const{
  bool act[6];
  int n_act=0;
  bool lo_fue[6];
  possible_actions_escalada(act, lo_fue);
  for (int i=0; i<5; i++){
    if (act[i]){
       V[n_act] = *this;
       V[n_act].last_action=i;
       V[n_act].Add_Step_in_Road(i);
       switch(i){
           case 0: V[n_act].posX--;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   V[n_act].h = 1;
                   V[n_act].camino[posX][posY]+=5;
                   V[n_act].cam = camino[posX-1][posY];
                   break;
           case 1: V[n_act].posX++;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   V[n_act].h = 1;
                   V[n_act].camino[posX][posY]+=5;
                   V[n_act].cam = camino[posX+1][posY];
                   break;
           case 2: V[n_act].posY--;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   V[n_act].h = 1;
                   V[n_act].camino[posX][posY]+=5;
                   V[n_act].cam = camino[posX][posY-1];
                   break;
           case 3: V[n_act].posY++;
                   V[n_act].Consumed_Energy++;
                   V[n_act].g++;
                   V[n_act].h = 1;
                   V[n_act].camino[posX][posY]+=5;
                   V[n_act].cam = camino[posX][posY+1];
                   break;
           case 4: V[n_act].Pending_Dirty--;
                   V[n_act].mundo[posX][posY]--;
                   V[n_act].Consumed_Energy+=2;
                   V[n_act].g+=2;
                   V[n_act].h = -1000;
                  /* if (V[n_act].Tam_X < 30)
                        V[n_act].camino[posX][posY]-=5;*/
                   V[n_act].cam = camino[posX][posY];
                   break;
            case 5: V[n_act].h = 0;
                    V[n_act].cam = 99;
                    break;

       }
    }

    else if(lo_fue[i]) {
        V[n_act].h = 1000;
    }

       else {
       V[n_act].h = 1000000000;
       V[n_act].f = V[n_act].g + V[n_act].h;

    }


     n_act++;
  }
  return n_act;
}



//------------------------------------------------------------------------

void state::Datos_Finales() const{
    cout << "Energia consumida: " << Consumed_Energy << endl;
    cout << "Suciedad Presente: " << Pending_Dirty << endl;

}


void state::PrintState() const{
  cout << "Pos(" << posX <<"," << posY <<") Suciedad: "<< Pending_Dirty << " Accion: " << last_action <<endl;
  cout << "g()= " << g << endl;
  for (int x=0; x<Tam_X; x++){
    for (int y=0; y<Tam_Y; y++){
      if (mundo[x][y]==-1)
        cout << "X";
      else
        cout << mundo[x][y];
    }
    cout << endl;
  }
  cout << endl;
}






double state::Suciedad_mas_cercana(int direccion)const{
    double distancia;
    double menor_distancia = 10000;
    int posicionX = posX;
    int posicionY = posY;


    if (direccion == 4 || direccion == 5)
        return 0;

    for(int i=0; i < Tam_X; i++){
        for(int a = 0; a < Tam_Y; a++){
            if(mundo[i][a] > 0){
                distancia = sqrt((i - posicionX)*(i-posicionX) + (a - posicionY)*(a-posicionY));
                if(distancia < 0)
                    distancia *= -1;
                if(distancia < menor_distancia)
                    menor_distancia = distancia;
            }

        }
    }

    return menor_distancia*3;
}
