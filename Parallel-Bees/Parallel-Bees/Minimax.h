#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include "json.hpp"
#include <string>
#include "Helper.h"
#include "Evaluate.h"
#include <limits>
#include <iostream>

using namespace std;
using json = nlohmann::json;

class Minimax
{
	Helper* helper;
	Evaluate* evaluate;
public:
	Minimax();
	string* StartAI();
	bool TrySkip(json, int);
	int TryFeedBee(json, int);
	int TryConvert(json, int);
	long* EvaluateTileNumber(int, int, string, json, int, int);
	string* FindBestMove();
	long MiniMax(long, long, int, int, json);
	bool IsEnd(json, int);
};

#endif