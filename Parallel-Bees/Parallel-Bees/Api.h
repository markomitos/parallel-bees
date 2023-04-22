#pragma once
#include "json.hpp"
#include "Global.h"
#include <string>
#include <curl/curl.h>
#include <iostream>

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
	size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);
	string makePostRequest(std::string url, std::string data);
};	

