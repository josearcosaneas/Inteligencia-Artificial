#include <iostream>
#include <cstdlib>
#include "player.h"
#include "environment.h"

using namespace std;

Player::Player(int jug){
    jugador_=jug;
}


void Player::Perceive(const Environment & env){
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
double Heuristica (const Environment & actual, int jug){
    int marcador_propio, marcador_rival;


    if (jug==1){
        marcador_propio=actual.Marcador(1);
        marcador_rival=actual.Marcador(2);
    }
    else {
        marcador_propio=actual.Marcador(2);
        marcador_rival=actual.Marcador(1);
    }

    if (marcador_propio>marcador_rival+actual.Total_Suciedad()){
        return 1000000;
    }
    else if (marcador_rival>marcador_propio+actual.Total_Suciedad()){
        return -1000000;
    }
    else {
        return 0;
    }
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

  // Definicion del metodo minimax para obtener la siguiente accion a realizar

  // OBSERVACION: la sentencia siguiente esta puesta para que haga algo. En la version final no debe aparecer.
  accion = static_cast<Environment::ActionType> (rand()%4);

  return accion;
}




/**
Funcion que invoca al metodo Minimax
*/
Environment::ActionType Player::Think(){
    return MiniMax(actual_,jugador_,10);
}

