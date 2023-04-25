#include "Helper.h"

extern int ourPlayer;
extern int opponentPlayer;
extern json gameState;
extern string flowers[4];
extern string booster[3];

int* Helper::GetPlayerCoordinates(json currentState)
{
	int* xy = nullptr;
	json player = currentState["player" + to_string(ourPlayer)];
	xy[0] = player[0];
	xy[1] = player[1];
	return xy;
}

int* Helper::GetOpponentCoordinates(json currentState)
{
	int* xy = nullptr;
	json player = currentState["player" + to_string(opponentPlayer)];
	xy[0] = player[0];
	xy[1] = player[1];
	return xy;
}

string** Helper::getMoves(json currentState, int playerNum)
{
	int* xy;
	if (playerNum == ourPlayer) {
		xy = GetPlayerCoordinates(currentState);
	}
	else {
		xy = GetOpponentCoordinates(currentState);
	}
	string** s = nullptr;
	if (xy[0] % 2 == 0) {
		s[0][0] = to_string(xy[0] - 1);
		s[0][1] = to_string(xy[1] - 1);
		s[0][2] = "q";

		s[1][0] = to_string(xy[0] - 2);
		s[1][1] = to_string(xy[1]);
		s[1][2] = "w";

		s[2][0] = to_string(xy[0] - 1);
		s[2][1] = to_string(xy[1]);
		s[2][2] = "e";

		s[3][0] = to_string(xy[0] + 1);
		s[3][1] = to_string(xy[1]);
		s[3][2] = "d";

		s[4][0] = to_string(xy[0] + 2);
		s[4][1] = to_string(xy[1]);
		s[4][2] = "s";

		s[5][0] = to_string(xy[0] + 1);
		s[5][1] = to_string(xy[1] - 1);
		s[5][2] = "a";
	}
	else {
		s[0][0] = to_string(xy[0] - 1);
		s[0][1] = to_string(xy[1]);
		s[0][2] = "q";

		s[1][0] = to_string(xy[0] - 2);
		s[1][1] = to_string(xy[1]);
		s[1][2] = "w";

		s[2][0] = to_string(xy[0] - 1);
		s[2][1] = to_string(xy[1] + 1);
		s[2][2] = "e";

		s[3][0] = to_string(xy[0] + 1);
		s[3][1] = to_string(xy[1] + 1);
		s[3][2] = "d";

		s[4][0] = to_string(xy[0] + 2);
		s[4][1] = to_string(xy[1]);
		s[4][2] = "s";

		s[5][0] = to_string(xy[0] + 1);
		s[5][1] = to_string(xy[1]);
		s[5][2] = "a";
	}
	return s;
}

int* Helper::CalculateNextStep(int x, int y, string direction)
{
	int* xy = nullptr;
	if (direction == "w") {
		xy[0] = x - 2;
		xy[1] = y;
		return xy;
	}
	else if (direction == "s") {
		xy[0] = x + 2;
		xy[1] = y;
		return xy;
	}
	if (x % 2 == 0) {
		if (direction == "q") {
			xy[0] = x - 1;
			xy[1] = y - 1;
			return xy;
		}
		else if (direction == "e") {
			xy[0] = x - 1;
			xy[1] = y;
			return xy;
		}
		else if (direction == "d") {
			xy[0] = x + 1;
			xy[1] = y;
			return xy;
		}
		else if (direction == "a") {
			xy[0] = x + 1;
			xy[1] = y - 1;
			return xy;
		}
	}
	else {
		if (direction == "q") {
			xy[0] = x - 1;
			xy[1] = y;
			return xy;
		}
		else if (direction == "e") {
			xy[0] = x - 1;
			xy[1] = y + 1;
			return xy;
		}
		else if (direction == "d") {
			xy[0] = x + 1;
			xy[1] = y + 1;
			return xy;
		}
		else if (direction == "a") {
			xy[0] = x + 1;
			xy[1] = y;
			return xy;
		}
	}
}

/// <summary>
/// def noMoreFlowers(currentState):
///   for tiles in globalVar.gameStateJSON["map"]["tiles"]:
///   for tile in tiles :
///   if (tile["row"] not in currentState["tiles"]) or ((tile["row"] in currentState["tiles"]) and tile["column"] not in currentState["tiles"][tile["row"]]) :
///   	if (tile["tileContent"]["itemType"] in globalVar.FLOWERS) :
///   		return False
///   	return True
/// </summary>
/// <param name="currentState"></param>
/// <returns></returns>

bool Helper::NoMoreFlowers(json currentState)
{
	vector<json> tiles = gameState["map"]["tiles"];
	for (int i = 0; i < tiles.size(); i++) {
		json tile = currentState["tiles"];
		//if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), tiles[i]["row"]) != std::end(currentState["tiles"])) || 
		//	(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), tiles[i]["row"]) != std::end(currentState["tiles"]) && 
		//		(std::find(std::begin(tile[tiles[i]["row"]]), std::end(currentState["tiles"][tiles[i]["row"]]), tiles[i]["column"]) != std::end(currentState["tiles"][tiles[i]["row"]])))) {
		if (std::find(std::begin(flowers[0]), std::end(flowers[3]), tiles[i]["tileContent"]["itemType"]) != std::end(flowers[3])) {
			return false;
		}
		//}
	}
	return true;
}

json Helper::CheckTileType(int x, int y, json currentState)
{
	if ((x < 0 or x > 26) || (x % 2 == 0 && (y > 8 || y < 0)) || (x % 2 == 1 && (y > 7 || y < 0))) {
		return NULL;
	}

	int* xy = nullptr;
	xy[0] = gameState["player" + to_string(opponentPlayer)]["hiveX"];
	xy[1] = gameState["player" + to_string(opponentPlayer)]["hiveY"];

	if (GetPlayerCoordinates(currentState) == xy) {
		return NULL;
	}

	json tileContent = gameState["map"]["tiles"][x][y]["tileContent"];
	if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]) &&
		std::find(std::begin(currentState["tiles"][x]), std::end(currentState["tiles"][x]), y) != std::end(currentState["tiles"][x])) {
		tileContent = currentState["tiles"][x][y];
	}

	if (tileContent["itemType"] == "POND" || tileContent["itemType"] == "HOLE") {
		return NULL;
	}

	if (xy == GetOpponentCoordinates(currentState)) return NULL;
	return tileContent;
}

json Helper::MakeCurrentState()
{
	json newCurrentState = {
		{"player1", {
			{"x", gameState["player1"]["x"]},
			{"y", gameState["player1"]["y"]},
			{"energy", gameState["player1"]["energy"]},
			{"nectar", gameState["player1"]["nectar"]},
			{"score", gameState["player1"]["score"]},
			{"honey", gameState["player1"]["honey"]},
			{"frozen", gameState["player1"]["frozen"]}
		}},
		{"player2", {
			{"x", gameState["player2"]["x"]},
			{"y", gameState["player2"]["y"]},
			{"energy", gameState["player2"]["energy"]},
			{"nectar", gameState["player2"]["nectar"]},
			{"score", gameState["player2"]["score"]},
			{"honey", gameState["player2"]["honey"]},
			{"frozen", gameState["player2"]["frozen"]}
		}},
		{"tiles", {}}
	};
	return newCurrentState;
}

json Helper::ChangeCurrentState(json currentState, json tile, int playerNum, int x, int y)
{
		string itemType, player, opponent;
		int numOfItems;
	
		if (tile.contains("row")) {
			x = tile["row"];
			y = tile["column"];
			itemType = tile["tileContent"]["itemType"];
			numOfItems = tile["tileContent"]["numOfItems"];
		}
		else {
			itemType = tile["itemType"];
			numOfItems = tile["numOfItems"];
		}
	
		if (playerNum == opponentPlayer) {
			player = "player" + to_string(opponentPlayer);
			opponent = "player" + to_string(ourPlayer);
		}
		else {
			opponent = "player" + to_string(opponentPlayer);
			player = "player" + to_string(ourPlayer);
		}
	
		currentState[player]["x"] = x;
		currentState[player]["y"] = y;
		currentState[player]["energy"] = currentState[player]["energy"] - 2;
	
		int currentNectar = currentState[player]["nectar"];
		int currentEnergy = currentState[player]["energy"];
	
		json emptyContent = {
			{"itemType", "EMPTY"},
			{"numOfItems", 0}
		};
		
		bool b = false;
		for (int i = 0; i < 4; i++)
		{
			if (flowers[i] == itemType) {
				b = true;
				break;
			}
			if (i != 4 && booster[i] == itemType) {
				b = true;
				break;
			}
		}

		if (b) {
			if (currentNectar < 100) {
				if (currentState["tiles"].is_null()) {
					currentState["tiles"][x] = json();
				}
				if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]))) {
					currentState["tiles"][x] = json();
				}
				currentState["tiles"][x][y] = emptyContent;
	
				int pointsToAdd = numOfItems;
				if ((currentNectar + numOfItems) > 100) {
					pointsToAdd = numOfItems - (currentNectar + numOfItems - 100);
				}
				currentState[player]["nectar"] = currentState[player]["nectar"] + pointsToAdd;
				currentState[player]["score"] = currentState[player]["score"] + pointsToAdd;
			}
		}
		else {
			if (itemType == "FREEZ") {
				currentState[opponent]["frozen"] = true;
				if (currentState["tiles"].is_null()) {
					currentState["tiles"][x] = json();
				}
				if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]))) {
					currentState["tiles"][x] = json();
				}
				currentState["tiles"][x][y] = emptyContent;
				currentState[player]["score"] = currentState[player]["score"] + 100;
			}
			else if (itemType == "MINUS_FIFTY_PCT_ENERGY") {
				currentState[opponent]["energy"] = currentState[opponent]["energy"] / 2;
				if (currentState["tiles"].is_null()) {
					currentState["tiles"][x] = json();
				}
				if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]))) {
					currentState["tiles"][x] = json();
				}
				currentState["tiles"][x][y] = emptyContent;
				currentState[player]["score"] = currentState[player]["score"] + 50;
			}
			else if (itemType == "SUPER_HONEY") {
				if (currentState["tiles"].is_null()) {
					currentState["tiles"][x] = json();
				}
				if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]))) {
					currentState["tiles"][x] = json();
				}
				currentState["tiles"][x][y] = emptyContent;
				currentState[opponent]["score"] = currentState[opponent]["score"] + 150;
				currentState[opponent]["honey"] = currentState[opponent]["honey"] + 5;
			}
			else if (itemType == "ENERGY") {
				if (currentEnergy < 100) {
					if (currentState["tiles"].is_null()) {
						currentState["tiles"][x] = json();
					}
					if (!(std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), x) != std::end(currentState["tiles"]))) {
						currentState["tiles"][x] = json();
					}
					currentState["tiles"][x][y] = emptyContent;
				}
				int pointsToAdd = numOfItems;
				if (currentEnergy + numOfItems > 100) {
					pointsToAdd = numOfItems - (currentEnergy + numOfItems - 100);
				}
				currentState[opponent]["energy"] = currentState[opponent]["energy"] + pointsToAdd;
				currentState[opponent]["score"] = currentState[opponent]["score"] + pointsToAdd;
			}
		}
		return currentState;
	return json();
}

bool Helper::IsInHive(json player)
{
	if ((player["x"] == 0 && player["y"] == 0) || (player["x"] == 26 && player["y"] == 8)) {
		return true;
	}
	return false;
}
