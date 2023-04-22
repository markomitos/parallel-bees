#include "Api.h"

using namespace std;

void Api::InitializeGame()
{
	//map<string, int> data;
    string s = "playerId=" + to_string(PLAYER_ID) + "&playerSpot=1";
	//data["playerId"] = PLAYER_ID;
	//data["playerSpot"] = 1;
    string request = makePostRequest(URL_MAKE, s);
    cout << request;
}

string Api::CrateUrl(string move)
{
	return string();
}

json Api::MoveBee(string direction, int numberOfTiles)
{
	return json();
}

json Api::convertNectarToHoney(int ammountOfHoney)
{
	return json();
}

json Api::feedBeeWithNectar(int ammountOfNectar)
{
	return json();
}

json Api::skipATurn()
{
	return json();
}

//size_t Api::WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
//{
//    ((std::string*)userp)->append(contents, size * nmemb);
//    return size * nmemb;
//}

string Api::makePostRequest(std::string url, std::string data)
{
	CURL* curl = curl_easy_init();
	if (curl) {
		// Set the URL to which the request will be sent
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// Set the HTTP method to POST
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		// Set the data that will be sent in the request body
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		// Perform the request
		CURLcode res = curl_easy_perform(curl);

		// Clean up
		curl_easy_cleanup(curl);
	}
    return string();
}
