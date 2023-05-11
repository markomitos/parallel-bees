#include "Evaluate.h"

extern int ourPlayer;
extern int opponentPlayer;
extern json gameState;

Evaluate::Evaluate()
{
	helper = new Helper();
}

enum TileValue
{
	ENERGY, BOOSTER_NECTAR_30_PCT, BOOSTER_NECTAR_50_PCT, BOOSTER_NECTAR_100_PCT,
	HIVE, EMPTY, MINUS_FIFTY_PCT_ENERGY, SUPER_HONEY, FREEZE
};

long Evaluate::Eval(json currentState, int playerNum)
{
	json player = currentState["player" + to_string(playerNum)];
	long ret = ValueInTile(currentState, playerNum);
	ret += MovingDirections(currentState, playerNum);
	ret += NumOfFree(currentState, playerNum);
	ret += NumOfSkip(playerNum);
	ret += isHive(currentState, playerNum);
	if (player["energy"] < 3) {
		ret = -999999;
	}
	if (ourPlayer == playerNum) {
		return -ret;
	}
	return ret;
}

long Evaluate::ValueInTile(json currentState, int playerNum)
{
	json player = currentState["player" + to_string(playerNum)];
	int* newVal = new int();
	if (playerNum == ourPlayer) {
		newVal = helper->GetPlayerCoordinates(currentState);
	}
	else {
		newVal = helper->GetOpponentCoordinates(currentState);
	}
	json oPlayer = currentState["player" + to_string(opponentPlayer)];

	json tileValue;
	if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), newVal[0]) != std::end(currentState["tiles"])
		&& std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), newVal[1]) != std::end(currentState["tiles"])) {
		tileValue = currentState["tiles"][newVal[0]][newVal[1]];
	}
	else {
		tileValue = gameState["map"]["tiles"][newVal[0]][newVal[1]]["tileContent"];
	}
	long addedNectar;
	long addedEnergy;
	string itemType = tileValue["itemType"];
	if (itemType == "ENERGY") {
		if (player["energy"] + 20 > 100) {
			addedEnergy = 20 - (player["energy"] + 20) % 100;
		}
		else {
			addedEnergy = 20;
		}
		return addedEnergy;
	}
	else if (itemType == "BOOSTER_NECTAR_30_PCT") {
		if (player["nectar"] * 1.3 > 100) {
			addedNectar = 100 - player["nectar"];
		}
		else {
			addedNectar = 100 - player["nectar"] * 1.3;
		}
		return addedNectar;
	}
	else if (itemType == "BOOSTER_NECTAR_50_PCT") {
		if (player["nectar"] * 1.5 > 100) {
			addedNectar = 100 - player["nectar"];
		}
		else {
			addedNectar = 100 - player["nectar"] * 1.5;
		}
		return addedNectar;
	}
	else if (itemType == "BOOSTER_NECTAR_100_PCT") {
		if (player["nectar"] * 2 > 100) {
			addedNectar = 100 - player["nectar"];
		}
		else {
			addedNectar = 100 - player["nectar"] * 2;
		}
		return addedNectar;
	}
	else if (itemType == "HIVE") {
		return 2;
	}
	else if (itemType == "EMPTY") {
		return 0;
	}
	else if (itemType == "MINUS_FIFTY_PCT_ENERGY") {
		return 100;
	}
	else if (itemType == "SUPER_HONEY") {
		return 150;
	}
	else if (itemType == "FREEZE") {
		if (opponentPlayer["frozen"]) {
			if (opponentPlayer["turnsFrozen"] > 1) {
				return 100;
			}
			return 50;
		}
		return 100;
	}
	else {
		if (player["nectar"] + (int)(tileValue["numOfItems"]) > 100) {
			addedNectar = (int)(tileValue["numOfItems"]) - (player["nectar"] + (int)(tileValue["numOfItems"])) % 100;
		}
		else {
			addedNectar = (int)(tileValue["numOfItems"]);
		}
		return addedNectar;
	}
	return 0;
}

long Evaluate::MovingDirections(json currentState, int playerNum)
{
	string** moves = helper->getMoves(currentState, playerNum);
	string player = "player" + to_string(playerNum);
	string myJson = currentState.dump();
	json myState = nlohmann::json::parse(myJson);
	int score1 = myState[player]["score"];
	int moveCounter = 0;
	int eng = 0;
	for (int i = 0; i < 6; i++)
	{
		if (moves == NULL || stoi(moves[i][0]) == -1 || stoi(moves[i][1]) == -1) {
			//moves++;
			continue;
		}
		int x = stoi(moves[i][0]);
		int y = stoi(moves[i][1]);
		string direction = moves[i][2];
		int energy = myState[player]["energy"];
		while (true)
		{
			if (helper->CheckTileType(x, y, currentState) != json()) {
				json tileValue;
				if (std::find(std::begin(myState["tiles"]), std::end(myState["tiles"]), x) != std::end(myState["tiles"])
					&& std::find(std::begin(myState["tiles"]), std::end(myState["tiles"]), y) != std::end(myState["tiles"])) {
					tileValue = myState["tiles"][x][y];
				}
				else {
					tileValue = gameState["map"]["tiles"][x][y];
				}
				myState = helper->ChangeCurrentState(myState, tileValue, playerNum, x, y);
				int* var = helper->CalculateNextStep(x, y, direction);
				x = var[0];
				y = var[1];
				myState[player]["x"] = x;
				myState[player]["y"] = y;
				moveCounter++;
				energy -= 2;
				if (energy < 3) {
					break;
				}
			}
			else {
				break;
			}
		}
		eng += energy;
	}
	int score2 = myState[player]["score"];
	if (moveCounter == 0 || score1 == score2) {
		return 0;
	}
	return (score2 - score1) / moveCounter - eng / moveCounter;
}

//long Evaluate::MovingDirections(json currentState, int playerNum)
//{
//	string** moves = helper->getMoves(currentState, playerNum);
//	string player = "player" + to_string(playerNum);
//	int score1 = currentState[player]["score"];
//	int moveCounter = 0;
//	int eng = 0;
//	int** matrix = new int* [6];
//	for (int i = 0; i < 6; i++) {
//		matrix[i] = new int[3];
//	}
//	task_group t;
//	t.run([&] {matrix[0] = EvaluatePositionDirection(moves, 0, currentState, player, playerNum); });
//	t.run([&] {matrix[1] = EvaluatePositionDirection(moves, 1, currentState, player, playerNum); });
//	t.run([&] {matrix[2] = EvaluatePositionDirection(moves, 2, currentState, player, playerNum); });
//	t.run([&] {matrix[3] = EvaluatePositionDirection(moves, 3, currentState, player, playerNum); });
//	t.run([&] {matrix[4] = EvaluatePositionDirection(moves, 4, currentState, player, playerNum); });
//	t.run([&] {matrix[5] = EvaluatePositionDirection(moves, 5, currentState, player, playerNum); });
//	t.wait();
//	int score2 = 0;
//	for (int i = 0; i < 6; i++) {
//		if (matrix[i] == NULL) continue;
//		score2 += matrix[i][0];
//		eng += matrix[i][1];
//		moveCounter += matrix[i][2];
//	}
//	if (moveCounter == 0 || score1 == score2) {
//		return 0;
//	}
//	return (score2 - score1) / moveCounter - eng / moveCounter;
//}
//
//int* Evaluate::EvaluatePositionDirection(string** moves, int i, json currentState, string player, int playerNum)
//{
//	int moveCounter = 0;
//	string myJson = currentState.dump();
//	json myState = nlohmann::json::parse(myJson);
//	int* values = new int[3];
//	if (moves == NULL || stoi(moves[i][0]) == -1 || stoi(moves[i][1]) == -1) {
//		//moves++;
//		return NULL;
//	}
//	int x = stoi(moves[i][0]);
//	int y = stoi(moves[i][1]);
//	string direction = moves[i][2];
//	int energy = myState[player]["energy"];
//	while (true)
//	{
//		if (helper->CheckTileType(x, y, currentState) != json()) {
//			json tileValue;
//			if (std::find(std::begin(myState["tiles"]), std::end(myState["tiles"]), x) != std::end(myState["tiles"])
//				&& std::find(std::begin(myState["tiles"]), std::end(myState["tiles"]), y) != std::end(myState["tiles"])) {
//				tileValue = myState["tiles"][x][y];
//			}
//			else {
//				tileValue = gameState["map"]["tiles"][x][y];
//			}
//			myState = helper->ChangeCurrentState(myState, tileValue, playerNum, x, y);
//			int* var = helper->CalculateNextStep(x, y, direction);
//			x = var[0];
//			y = var[1];
//			myState[player]["x"] = x;
//			myState[player]["y"] = y;
//			moveCounter++;
//			energy -= 2;
//			if (energy < 3) {
//				break;
//			}
//		}
//		else {
//			break;
//		}
//	}
//	values[0] = myState[player]["score"];
//	values[1] = energy;
//	values[2] = moveCounter;
//	return values;
//}

long Evaluate::NumOfFree(json currentState, int playerNum)
{
	string** moves = helper->getMoves(currentState, playerNum);
	int score = 0;
	for (int i = 0; i < 6; i++)
	{
		if (moves[i] == NULL) {
			break;
		}
		if (helper->CheckTileType(stoi(moves[i][0]), stoi(moves[i][1]), currentState) != NULL) {
			score++;
		}
	}
	return score;
}

long Evaluate::NumOfSkip(int playerNum)
{
	json player = gameState["player" + to_string(playerNum)];
	if (player["numOfSkipATurnUsed"] == 149) {
		return -9999999;
	}
	return 0;
}

long Evaluate::isHive(json currentState, int playerNum)
{
	json player = currentState["player" + to_string(playerNum)];
	int* xy;
	if (playerNum == ourPlayer) {
		xy = helper->GetPlayerCoordinates(currentState);
	}
	else {
		xy = helper->GetOpponentCoordinates(currentState);
	}
	if ((xy[0] == 0 && xy[1] == 0) || (xy[0] == 26 && xy[1] == 8) && player["nectar"] > 75) {
		return 80;
	}
	return 0;
}
