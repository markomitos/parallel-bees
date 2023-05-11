#include "Minimax.h"

extern int ourPlayer;
extern int opponentPlayer;
extern json gameState;
extern int depth;

string* Minimax::StartAI()
{
    string* returnValue = new string[3];
    int* playerCoordinates = helper->GetPlayerCoordinates(gameState);
    if ((playerCoordinates[0] == 0 && playerCoordinates[1] == 0 && ourPlayer == 1) || (playerCoordinates[0] == 28 && playerCoordinates[1] == 8 && ourPlayer == 2)) {
        int value = TryFeedBee(gameState, ourPlayer);
        if (value != 0) {
            returnValue[0] = "feedBeeWithNectar";
            returnValue[1] = to_string(value);
            return returnValue;
        }
        value = TryConvert(gameState, ourPlayer);
        if (value != 0) {
            returnValue[0] = "convertNectarToHoney";
            returnValue[1] = to_string(value);
            return returnValue;
        }
    }
    if (TrySkip(gameState, ourPlayer)) {
        returnValue[0] = "skipATurn";
        return returnValue;
    }
    string* bestMove = FindBestMove();
    returnValue[0] = "move";
    returnValue[1] = bestMove[0];
    returnValue[2] = bestMove[1];
    return returnValue;
}

Minimax::Minimax()
{
    helper = new Helper();
    evaluate = new Evaluate();
}

bool Minimax::TrySkip(json currentState, int playerNum)
{
    json player = currentState["player" + to_string(playerNum)];
    if (!helper->IsInHive(player) && player["energy"] < 7)
        return true;
    return false;
}

int Minimax::TryFeedBee(json currentState, int playerNum)
{
    json player = currentState["player" + to_string(playerNum)];
    int currentNectar = player["nectar"];
    int currentEnergy = player["energy"];
    if (currentEnergy > 70 || currentNectar < 5)
        return 0;
    return (100 - currentEnergy) / 2;
}

int Minimax::TryConvert(json currentState, int playerNum)
{
    json player = currentState["player" + to_string(playerNum)];
    return player["nectar"] / 20;
}

long* Minimax::EvaluateTileNumber(int x, int y, string direction, json currentState, int player, int depth)
{
    int bestNumberOfTiles = -1;
    int countTiles = 1;
    long bestScore;

    if (player == ourPlayer) {
        bestScore = numeric_limits<long>::max();

        while (true) {
            countTiles++;
            int* newVal = helper->CalculateNextStep(x, y, direction);

            json tileContent = helper->CheckTileType(newVal[0], newVal[1], currentState);

            if (tileContent.is_null() || currentState["player" + to_string(player)]["energy"] < 7)
                break;

            string myJson = currentState.dump();
            json newCurrentState = nlohmann::json::parse(myJson);
            
            if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), newVal[0]) != std::end(currentState["tiles"])) {
                if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), newVal[1]) != std::end(currentState["tiles"])) {
                    json tile = currentState["tiles"][newVal[0]][newVal[1]];
                    newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player, newVal[0], newVal[1]);
                }
                else {
                    json tile = gameState["map"]["tiles"][newVal[0]][newVal[1]];
                    newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player);
                }
            }

            long result = MiniMax(numeric_limits<long>::min(), numeric_limits<long>::max(), depth-1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);

            if (countTiles < 4)
                result += (4 - countTiles) * 550;

            if (result < bestScore) {
                if (currentState["player" + to_string(player)]["energy"] > bestNumberOfTiles * 2 + 3) {
                    bestScore = result;
                    bestNumberOfTiles = countTiles;
                }
            }
            
            x = newVal[0];
            y = newVal[1];
        }
    }
    else {
        bestScore = numeric_limits<long>::min();

        while (true) {
            countTiles++;
            int* newVal = helper->CalculateNextStep(x, y, direction);

            json tileContent = helper->CheckTileType(newVal[0], newVal[1], currentState);
            if (tileContent.is_null() || currentState["player" + to_string(player)]["energy"] < 7)
                break;

            string myJson = currentState.dump();
            json newCurrentState = nlohmann::json::parse(myJson);

            if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), newVal[0]) != std::end(currentState["tiles"])) {
                if (std::find(std::begin(currentState["tiles"][newVal[0]]), std::end(currentState["tiles"][newVal[0]]), newVal[1]) != std::end(currentState["tiles"][newVal[0]])) {
                    json tile = currentState["tiles"][newVal[0]][newVal[1]];
                    newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player, newVal[0], newVal[1]);
                }
            }
            else {
                json tile = gameState["map"]["tiles"][newVal[0]][newVal[1]];
                newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player);
            }

            long result = MiniMax(numeric_limits<long>::min(), numeric_limits<long>::max(), depth-1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
            
            if (countTiles < 4)
                result += (4 - countTiles) * 550;

            if (result > bestScore) {
                if (currentState["player" + to_string(player)]["energy"] > bestNumberOfTiles * 2 + 3) {
                    bestScore = result;
                    bestNumberOfTiles = countTiles;
                }
            }

            x = newVal[0];
            y = newVal[1];
        }
    }

    long* retVal = new long();

    retVal[0] = bestNumberOfTiles;
    retVal[1] = bestScore;

    return retVal;
}

string* Minimax::FindBestMove()
{
    string* bestMove = new string[2];
    long minVal = numeric_limits<long>::max();
    string** moves = helper->getMoves(gameState, ourPlayer);
    for (int i = 0; i < 6; i++) {
        json currentState = helper->MakeCurrentState();
        json tileContent = helper->CheckTileType(stoi(moves[i][0]), stoi(moves[i][1]), currentState);

        if (tileContent.is_null())
            continue;

        currentState = helper->ChangeCurrentState(currentState, gameState["map"]["tiles"][stoi(moves[i][0])][stoi(moves[i][1])], ourPlayer);

        long result = MiniMax(numeric_limits<long>::min(), numeric_limits<long>::max(), depth-1, opponentPlayer, currentState) + evaluate -> Eval(currentState, ourPlayer); //originalno depth-1 i global da je 3

        if (result < minVal) {
            minVal = result;
            bestMove[0] = moves[i][2];
            bestMove[1] = "1";
        }

        long* evalTile = EvaluateTileNumber(stoi(moves[i][0]), stoi(moves[i][1]), moves[i][2], currentState, ourPlayer, depth-1);
        
        if (evalTile[1] < minVal) {
            if (currentState["player" + to_string(ourPlayer)]["energy"] > evalTile[0] * 2 + 3) {
                minVal = evalTile[1];
                bestMove[0] = moves[i][2];
                bestMove[1] = to_string(evalTile[0]);
            }
        }
    }
    return bestMove;
}

long Minimax::MiniMax(long alpha, long beta, int depth, int player, json currentState)
{
    if (depth == 0 || IsEnd(currentState, depth))
        return evaluate -> Eval(currentState, player);

    json newCurrentState;
    if (player == ourPlayer) {
        long minVal = numeric_limits<long>::max();
        bool doneSomething = false;

        int* playerCoordinates = helper->GetPlayerCoordinates(currentState);
        if ((playerCoordinates[0] == 0 && playerCoordinates[1] == 0) || (playerCoordinates[0] == 28 && playerCoordinates[1] == 8)) {
            int value = TryFeedBee(currentState, player);
            if (value != 0) {
                doneSomething = true;
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["energy"] = newCurrentState["player" + to_string(player)]["energy"] + value * 2;
                newCurrentState["player" + to_string(player)]["nectar"] = newCurrentState["player" + to_string(player)]["nectar"] - value;

                long result = MiniMax(alpha, beta, depth - 1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                minVal = min(result, minVal);
            }
            value = TryConvert(currentState, player);
            if (value != 0) {
                doneSomething = true;
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["honey"] = newCurrentState["player" + to_string(player)]["honey"] + value;
                newCurrentState["player" + to_string(player)]["nectar"] = newCurrentState["player" + to_string(player)]["nectar"] - value*20;

                long result = MiniMax(alpha, beta, depth - 1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                minVal = min(result, minVal);
            }
        }

        if (!doneSomething) {
            if (TrySkip(currentState, player)) {
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["energy"] = newCurrentState["player" + to_string(player)]["energy"] + 5;;

                long result = MiniMax(alpha, beta, depth - 1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                minVal = min(result, minVal);
            }
            else {
                string** moves = helper->getMoves(currentState, ourPlayer);
                for (int i = 0; i < 6; i++) {
                    json tileContent = helper->CheckTileType(stoi(moves[i][0]), stoi(moves[i][1]), currentState);
                    if (tileContent.is_null())
                        continue;

                    if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), stoi(moves[i][0])) != std::end(currentState["tiles"])) {
                        if (std::find(std::begin(currentState["tiles"][stoi(moves[i][0])]), std::end(currentState["tiles"][stoi(moves[i][0])]), stoi(moves[i][1])) != std::end(currentState["tiles"][stoi(moves[i][0])])) {
                            json tile = currentState["tiles"][stoi(moves[i][0])][stoi(moves[i][1])];
                            string myJson = currentState.dump();
                            newCurrentState = nlohmann::json::parse(myJson);
                            newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player, stoi(moves[i][0]), stoi(moves[i][1]));
                        }
                    }
                    else {
                        json tile = gameState["map"]["tiles"][stoi(moves[i][0])][stoi(moves[i][1])];
                        string myJson = currentState.dump();
                        newCurrentState = nlohmann::json::parse(myJson);
                        newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player);
                    }

                    long result = MiniMax(alpha, beta, depth - 1, opponentPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);

                    minVal = min(result, minVal);
                    beta = min(beta, result);
                    if (beta <= alpha)
                        continue;

                    long* bestScore = EvaluateTileNumber(stoi(moves[i][0]), stoi(moves[i][1]), moves[i][2], newCurrentState, ourPlayer, depth);

                    minVal = min(bestScore[1], minVal);
                    /*beta = min(beta, bestScore[1]);
                    if (beta <= alpha)
                        continue;*/
                }
            }
        }
        return minVal;
    }
    else {
        long maxVal = numeric_limits<long>::min();
        bool doneSomething = false;
        int* playerCoordinates = helper->GetOpponentCoordinates(currentState);
        if ((playerCoordinates[0] == 0 && playerCoordinates[1] == 0 && opponentPlayer == 1) || (playerCoordinates[0] == 28 && playerCoordinates[1] == 8 && opponentPlayer == 2)) {
            int value = TryFeedBee(currentState, player);
            if (value != 0) {
                doneSomething = true;
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["energy"] = newCurrentState["player" + to_string(player)]["energy"] + value * 2;
                newCurrentState["player" + to_string(player)]["nectar"] = newCurrentState["player" + to_string(player)]["nectar"] - value;

                long result = MiniMax(alpha, beta, depth - 1, ourPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                maxVal = max(result, maxVal);
            }
            value = TryConvert(currentState, player);
            if (value != 0) {
                doneSomething = true;
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["honey"] = newCurrentState["player" + to_string(player)]["honey"] + value;
                newCurrentState["player" + to_string(player)]["nectar"] = newCurrentState["player" + to_string(player)]["nectar"] - value * 20;

                long result = MiniMax(alpha, beta, depth - 1, ourPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                maxVal = max(result, maxVal);
            }
        }

        if (!doneSomething) {
            if (TrySkip(currentState, player)) {
                string myJson = currentState.dump();
                newCurrentState = nlohmann::json::parse(myJson);
                newCurrentState["player" + to_string(player)]["energy"] = newCurrentState["player" + to_string(player)]["energy"] + 5;

                long result = MiniMax(alpha, beta, depth - 1, ourPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);
                maxVal = max(result, maxVal);
            }
            else {
                string** moves = helper->getMoves(currentState, opponentPlayer);
                for (int i = 0; i < 6; i++) {
                    json tileContent = helper->CheckTileType(stoi(moves[i][0]), stoi(moves[i][1]), currentState);
                    if (tileContent.is_null())
                        continue;

                    if (std::find(std::begin(currentState["tiles"]), std::end(currentState["tiles"]), stoi(moves[i][0])) != std::end(currentState["tiles"])) {
                        if (std::find(std::begin(currentState["tiles"][stoi(moves[i][0])]), std::end(currentState["tiles"][stoi(moves[i][0])]), stoi(moves[i][1])) != std::end(currentState["tiles"][stoi(moves[i][0])])) {
                            json tile = currentState["tiles"][stoi(moves[i][0])][stoi(moves[i][1])];
                            string myJson = currentState.dump();
                            newCurrentState = nlohmann::json::parse(myJson);
                            newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player, stoi(moves[i][0]), stoi(moves[i][1]));
                        }
                    }
                    else {
                        json tile = gameState["map"]["tiles"][stoi(moves[i][0])][stoi(moves[i][1])];
                        string myJson = currentState.dump();
                        newCurrentState = nlohmann::json::parse(myJson);
                        newCurrentState = helper->ChangeCurrentState(newCurrentState, tile, player);
                    }

                    long result = MiniMax(alpha, beta, depth - 1, ourPlayer, newCurrentState) + evaluate->Eval(newCurrentState, player);

                    maxVal = max(result, maxVal);
                    alpha = max(alpha, result);
                    if (beta <= alpha)
                        continue;

                    long* bestScore = EvaluateTileNumber(stoi(moves[i][0]), stoi(moves[i][1]), moves[i][2], newCurrentState, opponentPlayer, depth);

                    maxVal = max(bestScore[1], maxVal);
                    /*alpha = max(alpha, bestScore[1]);
                    if (beta <= alpha)
                        continue;*/
                }
            }
        }
        return maxVal;
    }
}

bool Minimax::IsEnd(json currentState, int depth)
{
    if (gameState["numOfMove"] + depth / 2 >= 300)
        return true;
    if (helper->NoMoreFlowers(currentState))
        return true;
    if (gameState["player1"]["numOfSkipATurnUsed"] >= 150)
        return true;

    return false;
}
