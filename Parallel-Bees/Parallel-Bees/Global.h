#pragma once
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

extern int const PLAYER_ID = 382538;
extern json gameState = NULL;
extern int depth = 3;
extern int ourPlayer = 1;
extern int opponentPlayer = 2;
extern int gameId = 1;
extern string actions[4]{ "move","convertNectarToHoney","feedBeeWithNectar", "skipATurn" };
extern string flowers[4]{ "CHERRY_BLOSSOM","LILAC","ROSE","SUNFLOWER" };
extern string booster[3]{ "BOOSTER_NECTAR_30_PCT","BOOSTER_NECTAR_50_PCT","BOOSTER_NECTAR_100_PCT" };
