#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include "json.hpp"
#include <string>
#include "Helper.h"
#include "Evaluate.h"
#include "tbb/task_group.h"
#include <limits>
#include <iostream>

using namespace std;
using namespace tbb;
using json = nlohmann::json;

class Minimax
{
	Helper* helper;
	Evaluate* evaluate;
public:
	Minimax();
	string* StartAI();
	bool TrySkip(json*, int);
	int TryFeedBee(json*, int);
	int TryConvert(json*, int);
	void EvaluateTileNumber(int, int, string, json*, int, int, int*, long*);
	string* CalculateBestMoveForDirection(int, string**, json*, json*);
	void FindBestMove(string*, int*);
	long MiniMax(int, int, json*);
	bool IsEnd(json*, int);
};

#endif