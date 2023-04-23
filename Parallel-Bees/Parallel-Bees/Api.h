#ifndef _API_H_
#define _API_H_

#include "json.hpp"
#include <string>
#include <iostream>
#include <curl/curl.h>

using namespace std;
using json = nlohmann::json;

class Api
{
	string URL_MAKE = "http://localhost:8082/train/makeGame";
	string URL = "http://localhost:8082/train/";
public:
	void InitializeGame();
	string CreateUrl(string moveType);
	void MoveBee(string direction, int numberOfTiles);
	void convertNectarToHoney(int ammountOfHoney);
	void feedBeeWithNectar(int ammountOfNectar);
	void skipATurn();
	void makePostRequest(std::string url, std::string data);
	static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);

};

struct MemoryStruct {
	char* memory;
	size_t size;
};

#endif