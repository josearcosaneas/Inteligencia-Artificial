#include "agent.h"
#include "environment.h"
#include "state.h"
#include <cstdlib>
#include <iostream>
#include <list>
#include <queue>
#include <stack>


using namespace std;


// -----------------------------------------------------------
Environment::ActionType Agent::AgenteReactivo()
{
    if(dirty_){
        return Environment::actSUCK;
    }

     if(bump_){
        if(last_action == 0)
            mapa[posx][posy +1] = 99999;
        else if(last_action == 1)
            mapa[posx][posy -1] = 99999;
        else if(last_action == 2)
            mapa[posx -1][posy] = 99999;
        else if(last_action == 3)
            mapa[posx +1][posy] = 99999;

     }
     else{

        if(last_action == 0)
            posy++;
        else if(last_action == 1)
            posy--;
        else if(last_action == 2)
            posx--;
        else if(last_action == 3)
            posx++;

        mapa[posx][posy] = contador++;

     }

    //Para saber donde moverse
    int menor, menor2, pos_menor, pos_menor2, muevete;

    if(mapa[posx - 1][posy] < mapa[posx + 1][posy]){
        menor = mapa[posx - 1][posy];
        pos_menor = 2;
    }
    else{
        menor = mapa[posx + 1][posy];
        pos_menor = 3;
    }

    if(mapa[posx][posy - 1] < mapa[posx][posy + 1]){
        menor2 = mapa[posx][posy - 1];
        pos_menor2 = 1;
    }
    else{
        menor2 = mapa[posx][posy + 1];
        pos_menor2 = 0;
    }
    if(menor <= menor2)
        muevete = pos_menor;
    else
        muevete = pos_menor2;


    last_action = muevete;

    return static_cast<Environment::ActionType> (muevete);

}





// -----------------------------------------------------------
// Inserta un estado (st) en una lista de estados (lista) y devuelve un iterador a su posición en la lista (it)
// La función devuelve "true" e inserta el estado (st) al final de la lista si es estado no estaba ya en la lista.
// La función devuelve "false" si el estado (st) ya estaba en la lista, y NO LO INSERTA EN LA LISTA
// -----------------------------------------------------------
bool InsertarLista(list <state> &lista, const state &st, list<state>::iterator &it){
  char ch;
  it= lista.begin();
  bool salida=false;
  while (it!=lista.end() and !(*it==st) )
    it++;
  if (it==lista.end()){
    lista.push_back(st);
    it = lista.end();
    it--;
    salida=true;
  }
  return salida;
}



// -----------------------------------------------------------
// Esto sirve para declarar un método de ordenación para la cola con prioridad.
// En esto caso, los ordena de menor a mayor en el primer valor del par (double,iterador)
// -----------------------------------------------------------
struct Comparar{
bool operator() (const pair<double,list<state>::iterator > &a, const pair<double,list<state>::iterator > &b){
  return (a.first > b.first );
}
};

// -----------------------------------------------------------
// Busqueda en Anchura
// -----------------------------------------------------------
Plan Agent::Busqueda_Anchura(state start){
  Plan plan;
  typedef pair<double,list<state>::iterator > elemento_cola;

  int last_level=0; // Indica el nivel del grafo por donde va la búsqueda
  int estados_evaluados = 0; // Indica el número de nodos evaluados
  state aux = start; // start es el estado inicial
  state sigActions[6], mejor_solucion; // para almacenar las siguientes acciones y la mejor solución
  int n_act;

  list<state> lista;               // Lista que almacenara todos los estados
  list<state>::iterator p, padre;  // Declara dos iteradores a la lista
  priority_queue <elemento_cola, vector<elemento_cola>, Comparar > cola; //Declaración de la cola con prioridad

  elemento_cola siguiente; // Declara una variable del tipo almacenado en la cola con prioridad

  InsertarLista(lista,aux,padre); // Inserta el estado inicial en la lista y (padre) es un iterador a su posición.

  while (!aux.Is_Solution()){
      // Indica si ha incrementado el nivel del grafo por donde está buscando
      if (aux.Get_g()!=last_level){
        cout << "Level " << aux.Get_g() << endl;
        last_level = aux.Get_g();
      }

      estados_evaluados++; // Incremento del número de estados evaluados

      n_act=aux.Generate_New_States(sigActions); // Genera los nuevos estados a partir del estado (aux)

      // Para cada estado generado, pone un enlace al estado que lo genero,
      // lo inserta en la lista, y si no estaba ya en dicha lista, lo incluye en la cola con prioridad.
      // El valor de prioridad en la lista lo da el método "Get_g()" que indica la energía consumida en dicho estado.
      for (int i=0; i<n_act; i++){
          sigActions[i].Put_Padre(padre);
          if (InsertarLista(lista, sigActions[i], p) ){
            double value = sigActions[i].Get_g();
            cola.push( pair<double,list<state>::iterator > (value,p) );
         }
      }

      // Saca el siguiente estado de la cola con prioridad.
      padre = cola.top().second;
      aux = *padre;
      cola.pop();
  }

  // Llegados aquí ha encontrado un estado solución, e
  // incluye la solución en una variable de tipo plan.
  plan.AnadirPlan(aux.Copy_Road(), lista.size(), estados_evaluados );

  return plan; // Devuelve el plan
}



// -----------------------------------------------------------
Plan Agent::Busqueda_Profundidad(state start){
 /* Plan plan;
  state aux = start;
  int estados_evaluados = 0;
*/

  // IMPLEMENTA AQUÍ LA BUSQUEDA EN PROFUNDIDAD

  Plan plan;
  typedef pair<double,list<state>::iterator > elemento_pila;

  int last_level=0; // Indica el nivel del grafo por donde va la búsqueda
  int estados_evaluados = 0; // Indica el número de nodos evaluados
  state aux = start; // start es el estado inicial
  state sigActions[6], mejor_solucion; // para almacenar las siguientes acciones y la mejor solución
  int n_act;

  list<state> lista;               // Lista que almacenara todos los estados
  list<state>::iterator p, padre;  // Declara dos iteradores a la lista
  stack <elemento_pila, vector<elemento_pila> > pila; //Declaración de la pila

  elemento_pila siguiente; // Declara una variable del tipo almacenado en la pila

  InsertarLista(lista,aux,padre); // Inserta el estado inicial en la lista y (padre) es un iterador a su posición.

  while (!aux.Is_Solution()){
      // Indica si ha incrementado el nivel del grafo por donde está buscando
      if (aux.Get_g()!=last_level){
        cout << "Level " << aux.Get_g() << endl;
        last_level = aux.Get_g();
      }

      estados_evaluados++; // Incremento del número de estados evaluados

      n_act=aux.Generate_New_States(sigActions); // Genera los nuevos estados a partir del estado (aux)

      // Para cada estado generado, pone un enlace al estado que lo genero,
      // lo inserta en la lista, y si no estaba ya en dicha lista, lo incluye en la pila con prioridad.
      // El valor de prioridad en la lista lo da el método "Get_g()" que indica la energía consumida en dicho estado.
      for (int i=0; i<n_act; i++){
          sigActions[i].Put_Padre(padre);
          if (InsertarLista(lista, sigActions[i], p) ){
            double value = sigActions[i].Get_g();
            pila.push( pair<double,list<state>::iterator > (value,p) );
         }
      }

      // Saca el siguiente estado de la pila con prioridad.
      padre = pila.top().second;
      aux = *padre;
      pila.pop();
  }



  plan.AnadirPlan(aux.Copy_Road(), lista.size(), estados_evaluados );

 return plan;
}



// -----------------------------------------------------------
// --------------------- Busqueda Heuristica -----------------
// -----------------------------------------------------------

double Heuristica(const state &estado, int direccion){

    double heuristic;
    double g = estado.Suciedad_mas_cercana(direccion);
    int h = estado.Get_h();
    int camino = estado.Get_camino();

    heuristic = h + g + camino;
    /*cout << heuristic << endl;
    cout << "camino---->" << camino << endl;
    cout << "g -------->" << g << endl;
    cout << "h -------->" << h << endl;
    estado.dibuja();
    cout << endl << endl;*/
    return heuristic;

}


// -----------------------------------------------------------


Plan Agent::Escalada(state start){
  Plan plan;
  state aux = start;
  int last_level = 0;
  int n_act;
  double mejor;
  state sigActions[6], mejor_solucion;
  int estados_expandidos=0, estados_evaluados=0;
  //  cout << "Pensando..." << endl;

 while (!aux.Is_Solution()){
      // Indica si ha incrementado el nivel del grafo por donde está buscando
      if (aux.Get_g()!=last_level){
        cout << "Level " << aux.Get_g() << endl;
        last_level = aux.Get_g();
      }
//system("pause");
      estados_evaluados++; // Incremento del número de estados evaluados

      n_act=aux.Generate_New_States_escalada(sigActions); // Genera los nuevos estados a partir del estado (aux)


    mejor = Heuristica(sigActions[0], 0);
    mejor_solucion = sigActions[0];

    for (int i=1; i<n_act; i++){
      //  cout << "si";
        if (mejor > Heuristica(sigActions[i], i)) {
           // cout << "   cambia -------->  ";
            mejor = Heuristica(sigActions[i], i);
            mejor_solucion = sigActions[i];
        }
    }
   // cout << "last->" << mejor_solucion.Last_Action() << endl;
   aux = mejor_solucion;

 }

  plan.AnadirPlan(aux.Copy_Road(), estados_expandidos, estados_evaluados );

  return plan;
}



// -----------------------------------------------------------
Plan Agent::Think(const Environment &env, int option){
  state start(env);
  Plan plan;


  switch (option){
      case 0: //Agente Reactivo

              break;
      case 1: //Busqueda Anchura
              plan = Busqueda_Anchura(start);
              cout << "\n Longitud del Plan: " << plan.Get_Longitud_Plan() << endl;
              plan.Pinta_Plan();
              break;
      case 2: //Busqueda Profundidad
              plan = Busqueda_Profundidad(start);
              cout << "\n Longitud del Plan: " << plan.Get_Longitud_Plan() << endl;
              plan.Pinta_Plan();
              break;
      case 3: //Busqueda Profundidad
              plan = Escalada(start);
              cout << "\n Longitud del Plan: " << plan.Get_Longitud_Plan() << endl;
              plan.Pinta_Plan();
              break;

  }

  return plan;
}






// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	bump_ = env.isJustBump();
	dirty_ = env.isCurrentPosDirty();
}
// -----------------------------------------------------------
string ActionStr(Environment::ActionType action)
{
	switch (action)
	{
	case Environment::actUP: return "UP";
	case Environment::actDOWN: return "DOWN";
	case Environment::actLEFT: return "LEFT";
	case Environment::actRIGHT: return "RIGHT";
	case Environment::actSUCK: return "SUCK";
	case Environment::actIDLE: return "IDLE";
	default: return "???";
	}
}
