#ifndef ENVIRONMENT__
#define ENVIRONMENT__

#include <string>

#include <fstream>
using namespace std;

//#include "agent.h"
// -----------------------------------------------------------
//				class Environment
// -----------------------------------------------------------
// Esta clase representa el tablero del juego

class Environment
{
public:

	// Acciones posibles
	enum ActionType { actFORWARD, actTURN_L, actTURN_R, actIDLE, actSHOOT};

    // ------------------------ Constructores y destructor de la clase
    Environment();
	Environment(ifstream &infile);
	~Environment();
	Environment(const Environment &env);

	// ------------------------- Operador de Asignacion
    Environment& operator=(const Environment& env);

    // ------------------------- Operador de Igualdad
    bool operator==(const Environment & env) const;

    // Metodos internos de gestion del propio entorno (no influyen para el desarrollo del metodo a realizar)
	void PintaAspiradora(int jugador, int w, int h, int x, int y) const;
	void Show(int,int) const;
	void Change();
    void ChangePlayer(){if (jugador_activo_==1) jugador_activo_=2; else jugador_activo_=1;}
    void Increment_Instante() {instante_++;}
	void AcceptAction(ActionType accion);




    // ------------ Aqui empiezan los metodos que pueden ser de utilidad para el desarrollo del metodo ----------------

    // Devuelve verdadero si la aspiradora del jugador que se pasa como argumento esta sobre una casilla con suciedad
	bool isCurrentPosDirty(int jugador) const { return (jugador==1?maze_[Position1_X()][Position1_Y()] > 0:maze_[Position2_X()][Position2_Y()] > 0); }

    // Devuelve si el ultimo movimiento del jugador que se pasa como argumento provoco una colision con un obstaculo
	bool isJustBump(int jugador) const { return (jugador==1?bump1_:bump2_); }

    // Devuelve las unidades de suciedad que contiene la casilla (x,y) en el tablero
	int  DirtAmount(int x, int y) const;

	// Devuelve la cantidad total de suciedad que contiene el tablero
	int  DirtAmount() const;

	// Devuelve la dimension del tablero (el tablero siempre tiene el mismo numero de filas que de columnas)
	int  SizeMaze() const {return MAZE_SIZE;}

	// Permite acceder al tablero
	int **SeeMap() const {return maze_;}

	// Devuelve la coordenada X (filas) de la aspiradora verde (jugador 1)
	int Position1_X() const {return agent1PosX_;}

	// Devuelve la coordenada Y (columnas) de la aspiradora verde (jugador 1)
	int Position1_Y() const {return agent1PosY_;}

	// Devuelve la coordenada X (filas) de la aspiradora azul (jugador 2)
	int Position2_X() const {return agent2PosX_;}

	// Devuelve la coordenada Y (columnas) de la aspiradora verde (jugador 2)
    int Position2_Y() const {return agent2PosY_;}

    // Devuelve la orientacion de la aspiradora verde (jugador 1). 0 indica Norte, 1 Este, 2 Sur y 3 Oeste
	int Rumbo1() const {return agent1Rumbo_;}

    // Devuelve la orientacion de la aspiradora azul (jugador 2). 0 indica Norte, 1 Este, 2 Sur y 3 Oeste
	int Rumbo2() const {return agent2Rumbo_;}

	// Devuelve la cantidad de suciedad anotada como recogida por el jugador que se pasa como argumento.
	int Marcador(int jug) const;

	// Indica la suciedad que aun queda por recoger en el tablero
	int Total_Suciedad() const;

	// Este metodo genera todas las situaciones resultantes de aplicar todas las acciones sobre el tablero actual para el
	// jugador que le toca jugar. Cada nuevo tablero se almacena en V, un vector de objetos de esta misma clase. El metodo
	// devuelve el tamano de ese vector, es decir, el numero de movimientos posibles.
	int GenerateAllMoves(Environment *V) const;

	// Este metodo genera el siguiente movimiento que se puede realizar el jugador al que le toca jugar sobre el tablero actual
	// devolviendolo como un objeto de esta misma clase. El parametro "act" indica cual fue el ultimo movimiento que se realizo
	// sobre el tablero. Este metodo asume el siguiente orden en la aplicacion de las acciones: 0 actFORWARD, 1 actTURN_LEFT, 2
	// actTURN_RIGHT, 3 actIDLE y 4 actSHOOT. Si no hay un siguiente movimiento, el metodo devuelve como tablero el actual.
	// La primera vez que se invoca en un nuevo estado se le pasa como argumento en act el valor -1.
	Environment GenerateNextMove(int &act) const;


	// Devuelve numero de acciones que puede realizar el jugador al que le toca jugar sobre el tablero. "VecAct" es un vector de
	// datos logicos que indican si una determinada accion es aplicable o no. Cada componente del vector esta asociada con una
	// accion. Asi, la [0] indica si actFORWARD es aplicable, [1] si lo es actTURN_LEFT, [2] si lo es actTURN_RIGHT, [3] si lo
	// es actIDLE y [4] si lo es actSHOOT.
    int possible_actions(bool *VecAct) const;

    // Indica la ultima accion que se aplico para llegar a la situacion actual del tablero. El entero que se devuelve es el
    // ordinal de la acción. Así, 0 es actFORWARD, 1 es actTURN_LEFT, 2 es actTURN_RIGHT, 3 es actIDLE y 4 es actSHOOT.
    int Last_Action(int jug) const;


    // Expresa en una cadena de caracteres un dato del tipo enumerado "ActionType" que se pasa como argumento.
    string ActionStr(ActionType action);

    // Devuelve el jugador al que le toca jugar, siendo 1 el jugador Verde y 2 el jugador Azul.
    int JugadorActivo()const {return jugador_activo_;}

    // Devuelve el numero de giros consecutivos realizados por el jugador que se pasa como argumento
    int Giros(int jug)const {if (jug==1) return agent1Giros_; else return agent2Giros_;}

    // Devuelve el numero de jugadas realizadas hasta el momentos desde el inicio de la partida
    int Get_Instante() const {return instante_;}

    // Devulve la cantidad de suciedad que debe contener el marcador de un jugador para ganar la partida.
    int Suciedad_Objetivo() const {return suciedad_objetivo_;}


private:

    // constantes que indican los distintos valores que pueden tomar las casillas del tablero
	static const int OBSTACLE = -1, OBSTACLE_VERDE=-2, OBSTACLE_AZUL=-3;
	static const char MAP_ROAD = '-', MAP_OBSTACLE = 'O';

    // Tamano del mapa (Siempre el numero de filas (X) es igual al numero de columnas (Y)
	int MAZE_SIZE;

    // Indican si las aspiradoras chocaron tras la ultima accion realizada
	bool bump1_, bump2_;

	// Posiciones y orientacion de las aspiradoras en el tablero
	int agent1PosX_, agent1PosY_, agent1Rumbo_, agent2PosX_, agent2PosY_, agent2Rumbo_;

	// Numero de Giros realizados consecutivamente por cada aspiradora
	int agent1Giros_, agent2Giros_;

	// Marcador de cada uno de los jugados y la suciedad pendiente de recoger en el tablero
	int marcador1_, marcador2_, unidades_suciedad_;

	// Matriz que codifica el tablero
	int **maze_;

	// Ultimas acciones realizadas por las dos aspiradoras.
	ActionType preAction1_, preAction2_;

	// Jugador al que le toca realizar el siguiente movimiento
	int jugador_activo_;

	// Numero de jugadas trascurridas desde el inicio de la partida.
	int instante_;

	// Cantidad de suciedad que debe contener el marcador de un jugador para ganar la partida.
	int suciedad_objetivo_;
};
// -----------------------------------------------------------

#endif
