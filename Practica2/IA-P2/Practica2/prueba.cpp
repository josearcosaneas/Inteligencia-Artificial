#include <fstream>
#include <iostream>
#include "environment.h"
#include "agent.h"


using namespace std;

int main (int argc, char *argv[]){

ifstream f (argv[1]);

Environment env(f);



Agent agent;
Plan plan;
int opcion=1; // 1 Profunidad,  2 Anchura, 3 Costo Uniforme, 4 Escalada

  plan = agent.Think(env, opcion);

  return 0;
}
