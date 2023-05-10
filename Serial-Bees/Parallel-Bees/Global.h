#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int PLAYER_ID = 382538;
json gameState = NULL;
int depth = 2;
int ourPlayer = 1;
int opponentPlayer = 2;
int gameId = 1;
string actions[4]{ "move","convertNectarToHoney","feedBeeWithNectar", "skipATurn" };
string flowers[4]{ "CHERRY_BLOSSOM","LILAC","ROSE","SUNFLOWER" };
string booster[3]{ "BOOSTER_NECTAR_30_PCT","BOOSTER_NECTAR_50_PCT","BOOSTER_NECTAR_100_PCT" };

#endif