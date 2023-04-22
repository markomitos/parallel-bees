#pragma once
#include "json.hpp"
#include <string>
#include <iostream>
#include "curl/curl.h"

using namespace std;
using json = nlohmann::json;

class Api
{
public:
	string URL_MAKE = "http://localhost:8082/train/makeGame";
	string URL = "http://localhost:8082/train/";
	void InitializeGame();
	string CrateUrl(string move);
	json MoveBee(string direction, int numberOfTiles);
	json convertNectarToHoney(int ammountOfHoney);
	json feedBeeWithNectar(int ammountOfNectar);
	json skipATurn();
	string makePostRequest(std::string url, std::string data);
};	

