
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

	Environment(ifstream &infile);
	~Environment();
	void Show(int,int) const;
	void Change();
	void AcceptAction(ActionType accion);

	bool isCurrentPosDirty() const { return maze_[agentPosX_][agentPosY_] > 0; }
	bool isJustBump() const { return bump_; }
	int  DirtAmount(int x, int y) const;
	int  DirtAmount() const;
	int  RandomSeed() const { return randomSeed_; }
	int  SizeMaze() const {return MAZE_SIZE;}
	int **SeeMap() const;
	int Position_X() const {return agentPosX_;}
	int Position_Y() const {return agentPosY_;}



private:
	static const int OBSTACLE = -1;
	static const char MAP_ROAD = '-', MAP_OBSTACLE = 'O';

	int MAZE_SIZE;

	bool bump_;
	int agentPosX_, agentPosY_;
	int **maze_; // -1: Obstacle, >=0: amount of dirt
	int randomSeed_;
	double dirtyProb_;

	/**/
	ActionType preAction_;
};
// -----------------------------------------------------------

#endif
