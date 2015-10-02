#include <fstream>
#include "environment.h"
#include "agent.h"


using namespace std;

int main (int argc, char *argv[]){

ifstream f (argv[1]);

Environment env(f);

Agent agent;
Plan plan;
int opcion=1; // 1 Anchura, 2 Profundidad, 3 Escalada

  plan = agent.Think(env, opcion);

  return 0;
}
