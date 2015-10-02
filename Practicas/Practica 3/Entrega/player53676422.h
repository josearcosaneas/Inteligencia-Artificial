#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player53676422{
    public:
      Player53676422(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
    private:
      int jugador_;
      Environment actual_;
};
#endif
