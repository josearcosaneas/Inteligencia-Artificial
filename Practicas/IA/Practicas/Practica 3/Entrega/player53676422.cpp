#include <iostream>
#include <cstdlib>
#include "Player53676422.h"
#include "environment.h"

using namespace std;

const int INFINITO = 99999999999999;

int valorMin(const Environment & actual, int jug, int profundidad, int limite_profundidad, int marcador);
int valorMax(const Environment & actual, int jug, int profundidad, int limite_profundidad, int marcador);


Player53676422::Player53676422(int jug){
    jugador_=jug;
}


void Player53676422::Perceive(const Environment & env){
    actual_=env;
}


/**
Calcula el valor heuristico de un estado de la frontera de busqueda
Devuelve: El valor heuristico asociado al estado "actual" desde el punto de vista del jugador "jug"
Parametros:
    "actual" estado que contiene el tablero a evaluar.
    "jug" indique que jugador esta pidiendo el valor heuristico.
OBSERVACION: esta parametrizacion es solo indicativa, y el alumno podra modificarla segun sus necesidades
*/
double Heuristica (const Environment & actual, int jug, int marcador){

    int valor_heuristico = 0, jug_adversario;

    if (jug == 1)
        jug_adversario = 2;
    else
        jug_adversario = 1;

    if (actual.Marcador(jug) > (actual.Marcador(jug_adversario) + actual.Total_Suciedad()) )
        valor_heuristico += 9999;
    else if ((actual.Marcador(jug) + actual.Total_Suciedad()) < actual.Marcador(jug_adversario))
        valor_heuristico += -9999;

    if (actual.Marcador(jug) >= actual.Marcador(jug_adversario))
        valor_heuristico += 3;
    else
        valor_heuristico += -3;

    if (!actual.a_movement_is_possible(jug))
        valor_heuristico += -8;
    if (!actual.a_movement_is_possible(jug_adversario))
        valor_heuristico += 100;


    for (int i = 0; i < 20; i++)
        if (marcador+i < actual.Marcador(jug))
            valor_heuristico += 10;

    return valor_heuristico;

}


/**
Esta funcion devuelve la siguiente mejor accion guiada por la heuristica usando el algoritmo minimax.
Devuelve: La siguiente accion a realizar.
Parametros:
    "actual" contiene el estado del tablero.
    "jug" indica que jugador esta pidiendo el valor heuristico.
    "limite_profundidad" establece el limite de exploracion del grafo.
OBSERVACION: esta parametrizacion es solo indicativa, y el alumno podra modificarla segun sus necesidades
*/
Environment::ActionType MiniMax(const Environment & actual, int jug, int limite_profundidad){
  Environment::ActionType accion;

  int mayor = -INFINITO, mayor_actual, profundidad = 0;

  Environment sigact[4];
  int n_act = actual.GenerateNextMove(sigact, jug);

  for (int i = 0; i < n_act; i++) {
        mayor_actual = valorMin(sigact[i], jug, profundidad+1, limite_profundidad, actual.Marcador(jug));
        if (mayor_actual > mayor) {
            mayor = mayor_actual;
            accion = static_cast<Environment::ActionType> (sigact[i].Last_Action(jug));
        }
  }
  cout << "Jugador: " << jug;
  cout << "     mayor " << mayor;
  cout << endl;
  return accion;
}

int minimo (int a, int b) {
    if (a<b) return a;
    else return b; }

int mayor (int a, int b) {
    if (a<b) return b;
    else return a; }


int valorMax(const Environment & actual, int jug, int profundidad, int limite_profundidad, int marcador) {

    int valor_max;

    Environment sigact[4];

    if (profundidad == limite_profundidad || !actual.a_movement_is_possible(jug))
        return Heuristica(actual, jug, marcador);

    else {
        int n_act = actual.GenerateNextMove(sigact, jug);
        valor_max = -INFINITO;
        for (int i = 0; i < n_act; i++) {
            valor_max = mayor(valor_max, valorMin(sigact[i], jug, profundidad+1, limite_profundidad, marcador));
        }
        return valor_max;
    }
}

int valorMin(const Environment & actual, int jug, int profundidad, int limite_profundidad, int marcador) {

    int valor_min;
    int jug_adversario;

    if (jug == 1)
        jug_adversario = 2;
    else if (jug == 2)
        jug_adversario = 1;

    Environment sigact[4];

    if (profundidad == limite_profundidad || !actual.a_movement_is_possible(jug_adversario))
        return Heuristica(actual, jug, marcador);

    else {
        int n_act = actual.GenerateNextMove(sigact, jug_adversario);
        valor_min = INFINITO;
        for (int i = 0; i < n_act; i++) {
            valor_min = minimo(valor_min, valorMax(sigact[i], jug, profundidad+1, limite_profundidad, marcador));
        }
        return valor_min;
    }
}

/*int encerrado(const Environment & actual, int jug) {

    int **maze = actual.SeeMap();
    int posx, posy, cantidad_basura = -1;
    bool esta_encerrado;
    int basura;

    if (jug == 1) {
        posx = actual.Position1_X();
        posy = actual.Position1_Y();
    }

    else {
        posx = actual.Position2_X();
        posy = actual.Position2_Y();
    }





    if (esta_encerrado)
        cantidad_basura = basura;

    return cantidad_basura;
}*/

/**
Funcion que invoca al metodo Minimax
*/
Environment::ActionType Player53676422::Think(){
    return MiniMax(actual_,jugador_,10);
}

