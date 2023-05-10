#include "Api.h"

using namespace std;

extern int PLAYER_ID;
extern json gameState;
extern int gameId;

void Api::InitializeGame()
{
    string data = R"({"playerId": 382538, "playerSpot": 1})";
    makePostRequest(URL_MAKE, data);
    cout << "Game ID -> " << gameId << endl;
}

string Api::CreateUrl(string moveType)
{
    return URL + moveType;
}

void Api::MoveBee(string direction, int numberOfTiles)
{
    string data = R"({"gameId": )" + to_string(gameId) + R"(,"playerId": )" + to_string(PLAYER_ID) + R"(,"direction": ")" + direction + R"(","distance": )" + to_string(numberOfTiles) + R"(})";
    makePostRequest(CreateUrl("move"), data);
}

void Api::convertNectarToHoney(int amountOfHoney)
{
    string data = R"({"gameId": )" + to_string(gameId) + R"(,"playerId": )" + to_string(PLAYER_ID) + R"(,"amountOfHoneyToMake": )" + to_string(amountOfHoney) + R"(})";
    makePostRequest(CreateUrl("convertNectarToHoney"), data);
}

void Api::feedBeeWithNectar(int amountOfNectar)
{
    string data = R"({"gameId": )" + to_string(gameId) + R"(,"playerId": )" + to_string(PLAYER_ID) + R"(,"amountOfNectarToFeedWith": )" + to_string(amountOfNectar) + R"(})";
    makePostRequest(CreateUrl("feedBeeWithNectar"), data);
}

void Api::skipATurn()
{
    string data = R"({"gameId": ")" + to_string(gameId) + R"(","playerId": )" + to_string(PLAYER_ID) + R"(})";
    makePostRequest(CreateUrl("skipATurn"), data);
}

void Api::makePostRequest(std::string url, std::string data)
{
    CURL* curl;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    // set the content type header
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

    /* get it! */
    res = curl_easy_perform(curl);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else{
        gameState = json::parse(chunk.memory);
        gameId = gameState["gameId"];
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl);

    free(chunk.memory);

    /* we are done with libcurl, so clean it up */
    curl_global_cleanup();
}

size_t Api::WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}
