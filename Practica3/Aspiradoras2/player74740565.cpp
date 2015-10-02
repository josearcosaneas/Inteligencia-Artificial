#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "Player74740565.h"
#include "environment.h"
#include <cmath>
using namespace std;

// Constructor
Player74740565::Player74740565(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player74740565::Perceive(const Environment & env){
    actual_=env;
}


// Funcion de valoracion para testear MiniMax y Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    if (jugador==1)
      return estado.Marcador(1);
    else
      return estado.Marcador(2);
}
/**
Calcula el valor heuristico de un estado de la frontera de busqueda
Devuelve: El valor heuristico asociado al estado "actual" desde el punto de vista del jugador "jug"
Parametros:
    "actual" estado que contiene el tablero a evaluar.
    "jug" indique que jugador esta pidiendo el valor heuristico.
*/
double Heuristica (const Environment & estado, int jug){
    double distancia, distancia_robot;
    int tamanio_matriz=estado.SizeMaze();       //devuelve el tamaño de la matriz
    int Aspiradora1X, Aspiradora1Y, Aspiradora2X, Aspiradora2Y;
    int **mundo=estado.SeeMap();          //devuelve el mundo en el que se mueven los agentes
    double menor_distancia = 10000;
    double distancia1,distancia2;
    double aux1,aux2;
    aux1=0;
    aux2=0;
    if(jug==1){     //escogemos jugador
        Aspiradora1X=estado.Position1_X();
        Aspiradora1Y=estado.Position1_Y();

    }

    else{
        Aspiradora1X=estado.Position2_X();
        Aspiradora1Y=estado.Position2_Y();

    }

    for(int i=0; i<tamanio_matriz; i++)
    {
        for(int j=0; j<tamanio_matriz; j++)
        {

            if(mundo[i][j]>0)
                aux1=Aspiradora1X-i;
                if(aux1<0)
                   distancia+=(tamanio_matriz+aux1);
                else distancia+=(tamanio_matriz-aux1);
                aux2=Aspiradora1Y-j;
                if(aux2<0)
                     distancia+=(tamanio_matriz+aux2);
                else
                distancia+=(tamanio_matriz-aux2);
        }

        return distancia;

    }

}

double Valoracion(const Environment &estado, int jugador){
    if (jugador==1)
      return ((estado.Marcador(1)*100000) + (estado.SizeMaze()*2) - Heuristica(estado,jugador));
    else
      return estado.Marcador(2);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

/*

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j, int giros){
    j=0;
    for (int i=0; i<5; i++){
        if (aplicables[i]){
           if (i==0) { // Doy más probabilidad a usar actFORWARD
              for (int k=0; k<4; k++){
                  opciones[j]=0;
                  j++;
              }
           }
           if ( (i==1 or i==2) and  ( (!aplicables[4] and j==0) or giros==0  ) ){
                  opciones[j]=i;
                  j++;
           }
           if (i==4) { // Doy más probabilidad a usar actSHOOT
              for (int k=0; k<2; k++){
                  opciones[j]=4;
                  j++;
              }
           }
        }
    }
}

*/

// Algoritmo MINIMAX
double MiniMax (const Environment & tablero, int jug, int prof, int limite, Environment::ActionType & accion){
  const double INF = 100000000.0, mINF = -100000000.0, gano = 99999999, pierdo = -99999999;
  Environment::ActionType accion_anterior;
  bool opciones[5];
  double mejor, aux;

  if (prof==limite){ // Condicion de parada
      return Valoracion(tablero,jug);
  }
  else { // Proceso recursivo de calculo de MiniMax
      int n_act = tablero.possible_actions(opciones); // Determino cuantas acciones se pueden realizar

      if (n_act==0){ // Si no se puede realizar ninguna acción
          if (prof%2==0) // En los niveles pares juega MAX
             return pierdo; // Yo no puedo mover y entonces pierdo
          else
             return gano;   // El que n puede mover es el adversario y gano
      }

      // Inicializo el mejor valor en funcion del nivel

      if (prof%2==0)
        mejor = mINF; // Inicializar un nivel MAX
      else
        mejor = INF;  // Inicializar un nivel MIN


      int ult_act=-1;
      Environment hijo = tablero.GenerateNextMove(ult_act); // Genero el siguiente tablero

      while (!(hijo==tablero)){ // Condicion de que sigo pudiendo aplicar nuevas acciones al tablero
          aux = MiniMax(hijo,jug,prof+1,limite,accion_anterior); // Evaluo el valor del hijo

          if (prof%2==0){ // Juega MAX
              if (aux>mejor){
                  mejor=aux;
                  accion = static_cast <Environment::ActionType > (ult_act);
              }
          }
          else {         // Juega MIN
              if (aux<mejor){
                  mejor=aux;
                  accion = static_cast <Environment::ActionType > (ult_act);
              }
          }

          hijo = tablero.GenerateNextMove(ult_act); // Genero el siguiente descendiente del tablero
      }

      return mejor;

  }


}


// Algoritmo Poda AlfaBeta
double Poda_AlphaBeta (const Environment & tablero, int jug, int prof, int limite, Environment::ActionType & accion,double alpha,double beta){
  const double INF = 100000000.0, mINF = -100000000.0, gano = 99999999, pierdo = -99999999;
  Environment::ActionType accion_anterior;
  bool opciones[5];
  double mejor, aux;

  if (prof==limite){ // Condicion de parada
      return Valoracion(tablero,jug);
  }
  else { // Proceso recursivo de calculo de MiniMax
      int n_act = tablero.possible_actions(opciones); // Determino cuantas acciones se pueden realizar

      if (n_act==0){ // Si no se puede realizar ninguna acción
          if (prof%2==0) // En los niveles pares juega MAX
             return pierdo; // Yo no puedo mover y entonces pierdo
          else
             return gano;   // El que n puede mover es el adversario y gano
      }

      // Inicializo el mejor valor en funcion del nivel

      if (prof%2==0)
        mejor = mINF; // Inicializar un nivel MAX
      else
        mejor = INF;  // Inicializar un nivel MIN


      int ult_act=-1;
      Environment hijo = tablero.GenerateNextMove(ult_act); // Genero el siguiente tablero

      while (!(hijo==tablero)){ // Condicion de que sigo pudiendo aplicar nuevas acciones al tablero
          aux = Poda_AlphaBeta(hijo,jug,prof+1,limite,accion_anterior,alpha,beta); // Evaluo el valor del hijo

          if (prof%2==0){ // Juega MAX
              if (aux>mejor){
                  mejor=aux;
                  accion = static_cast <Environment::ActionType > (ult_act);
              }
              if(mejor>=alpha) alpha=mejor;
            if(mejor>beta) hijo=tablero;
            else
            hijo = tablero.GenerateNextMove(ult_act);

          }
          else {         // Juega MIN
              if (aux<mejor){
                  mejor=aux;
                  accion = static_cast <Environment::ActionType > (ult_act);
              }

          if(mejor<=beta) beta=mejor;
            if(mejor<alpha) hijo=tablero;
            else
            hijo = tablero.GenerateNextMove(ult_act);

          }

      }

      return mejor;

  }


}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player74740565::Think(){
    const int PROFUNDIDAD_MINIMAX = 10;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 16; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[5]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si actFORWARD es aplicable
                        // aplicables[1]==true si actTURN_LEFT es aplicable
                        // aplicables[2]==true si actTURN_RIGHT es aplicable
                        // aplicables[3]==true si actIDLE es aplicable (aplicables[3] siempre es FALSE)
                        // aplicables[4]==true si actSHOOT es aplicable

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    cout << " Acciones aplicables para Jugador ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<5; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;



    //--------------------- COMENTAR Desde aqui
    /*cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones, actual_.Giros(jugador_));

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
     */
    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------
    // Opcion: Metodo Minimax
    // NOTA: La parametrizacion es solo orientativa
   valor = MiniMax(actual_, jugador_, 0, PROFUNDIDAD_MINIMAX, accion);
   cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;



    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    /*
    alpha=-1000000000000;
    beta=10000000000;
    valor = Poda_AlphaBeta(actual_, jugador_, 0,PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor Poda_AlphaBeta: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
*/
    return accion;
}


