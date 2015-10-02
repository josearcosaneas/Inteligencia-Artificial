
#ifndef ENVIRONMENT__
#define ENVIRONMENT__

#include <fstream>
using std::ifstream;

// -----------------------------------------------------------
//				class Environment
// -----------------------------------------------------------


class Environment
{
public:

	enum ActionType { actUP, actDOWN, actLEFT, actRIGHT, actSUCK, actIDLE };

    Environment();
	Environment(ifstream &infile);
	~Environment();
	Environment(const Environment &env);
    Environment& operator=(const Environment& env);

	void PintaAspiradora(int jugador, int w, int h, int x, int y) const;

	void Show(int,int) const;
	void Change();
	void AcceptAction(int jugador, ActionType accion);

	bool isCurrentPosDirty(int jugador) const { return (jugador==1?maze_[Position1_X()][Position1_Y()] > 0:maze_[Position2_X()][Position2_Y()] > 0); }
	bool isJustBump(int jugador) const { return (jugador==1?bump1_:bump2_); }
	int  DirtAmount(int x, int y) const;
	int  DirtAmount() const;
	int  SizeMaze() const {return MAZE_SIZE;}
	int **SeeMap() const {return maze_;};
	int Position1_X() const {return agent1PosX_;}
	int Position1_Y() const {return agent1PosY_;}
	int Position2_X() const {return agent2PosX_;}
	int Position2_Y() const {return agent2PosY_;}
	int Marcador(int jug) const;
	int Total_Suciedad() const;
	void SaveMap() const;
	int GenerateNextMove(Environment *V, int jug) const;
    void possible_actions(bool *act, int jug) const;
    int Last_Action(int jug) const;
    bool operator==(const Environment & env) const;
    bool a_movement_is_possible(int jug) const;



private:
	static const int OBSTACLE = -1, OBSTACLE_VERDE=-2, OBSTACLE_AZUL=-3;
	static const char MAP_ROAD = '-', MAP_OBSTACLE = 'O';

	int MAZE_SIZE;

	bool bump1_, bump2_;
	int agent1PosX_, agent1PosY_, agent2PosX_, agent2PosY_;
	int marcador1_, marcador2_, unidades_suciedad_;
	int **maze_; // Tablero

	/**/
	ActionType preAction1_, preAction2_;
};
// -----------------------------------------------------------

#endif
