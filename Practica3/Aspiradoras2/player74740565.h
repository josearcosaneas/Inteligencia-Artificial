#ifndef Player74740565_H
#define Player74740565_H

#include "environment.h"

class Player74740565{
    public:
      Player74740565(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
    private:
      int jugador_;
      Environment actual_;
};
#endif
