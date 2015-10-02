#include "evaluator.h"
#include "environment.h"

Evaluator::Evaluator(const Environment *env){
    consumedEnergy_=0;
    dirtyDegree_=0;
	for (int row=0; row<env->SizeMaze(); row+=1)
	{
		for (int col=0; col<env->SizeMaze(); col+=1)
		{
			long long da = env->DirtAmount(row, col);
			dirtyDegree_ += da;
		}
	}
}

void Evaluator::Eval(Environment::ActionType action, const Environment &env)
{
	if (action == Environment::actTURN_L)
		consumedEnergy_ += 2;
	else if (action != Environment::actIDLE)
		consumedEnergy_ += 1;


    dirtyDegree_=0;
	for (int row=0; row<env.SizeMaze(); row+=1)
	{
		for (int col=0; col<env.SizeMaze(); col+=1)
		{
			long long da = env.DirtAmount(row, col);
			dirtyDegree_ += da;
		}
	}
	//env.SaveMap();
}
