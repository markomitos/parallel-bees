#ifndef _HELPER_H_
#define _HELPER_H_

#include "json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;

class Helper
{
public:
	int* GetPlayerCoordinates(json);
	int* GetOpponentCoordinates(json);
	string** getMoves(json, int);
	int* CalculateNextStep(int, int, string);
	bool NoMoreFlowers(json);
	json CheckTileType(int, int, json);
	json MakeCurrentState();
	json ChangeCurrentState(json, json, int, int x = -1, int y = -1);
	bool IsInHive(json);
};

#endif