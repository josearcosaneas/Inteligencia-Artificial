#include "Plan.h"
#include <list>
#include <iostream>

using namespace std;

Plan::Plan()
{
    energiaConsumida=0;
    nodosExpandidos=0;
    nodosExplorados=0;
    longitudCamino=0;
}

Plan::~Plan()
{
    //dtor
}

Plan::Plan(const Plan& other)
{
    energiaConsumida=other.energiaConsumida;
    nodosExpandidos=other.nodosExpandidos;
    nodosExplorados=other.nodosExplorados;
    longitudCamino=other.longitudCamino;
    camino=other.camino;
    siguiente =camino.begin();
}

Plan& Plan::operator=(const Plan& other)
{
    if (this == &other) return *this; // handle self assignment
    energiaConsumida=other.energiaConsumida;
    nodosExpandidos=other.nodosExpandidos;
    nodosExplorados=other.nodosExplorados;
    longitudCamino=other.longitudCamino;
    camino=other.camino;
    siguiente =camino.begin();
    return *this;
}


void Plan::AnadirPlan(const list<int> &c, int nExpan, int nExplo){
  camino=c;
  siguiente=camino.begin();
  longitudCamino=camino.size();
  nodosExpandidos=nExpan;
  nodosExplorados=nExplo;

  energiaConsumida=0;
  list<int>::iterator it=camino.begin();
  while (it!=camino.end()){
      if (*it==4)
        energiaConsumida+=2;
      else
        energiaConsumida++;
      it++;
  }

}

int Plan::Siguiente_Accion(){
    int a;
    if (siguiente!=camino.end()){
      a=(*siguiente);
      siguiente++;
    }
    else
      a=5;

    return a;
}


void Plan::Pinta_Plan() {
  list<int>::iterator p = camino.begin();
  while (p!=camino.end()){
    cout << "->";
    switch (*p){
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
     p++;
  }
  cout << endl;
}
