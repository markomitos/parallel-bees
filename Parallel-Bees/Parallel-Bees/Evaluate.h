#ifndef _EVAL_H_
#define _EVAL_H_

#include "json.hpp"
#include <string>
#include "Helper.h"
#include "tbb/task_group.h"

using namespace std;
using namespace tbb;
using json = nlohmann::json;
class Evaluate
{
	Helper* helper;
public:
	Evaluate();
	long Eval(json, int);
	long ValueInTile(json, int);
	long MovingDirections(json, int);
	int* EvaluatePositionDirection(string**, int, json, string, int);
	long NumOfFree(json, int);
	long NumOfSkip(int);
	long isHive(json, int);
};

#endif