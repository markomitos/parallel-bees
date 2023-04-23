#ifndef _EVAL_H_
#define _EVAL_H_

#include "json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;
class Evaluate
{
public:
	long Eval(json, int);
};

#endif