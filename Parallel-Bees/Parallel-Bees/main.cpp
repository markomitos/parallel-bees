#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Api.h"
#include "Global.h"
#include "Minimax.h"
#include <chrono>

using namespace std;
using namespace chrono;
extern json gameState;

int main(void)
{
    Api api;
    api.InitializeGame();
    Minimax game{};

    while (true) {
        auto start = high_resolution_clock::now();
        string* res = game.StartAI();
        auto end = high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        cout << "Uradio sam potez " << res[0] << " " << res[1] << " " << endl;
        cout << "Vreme " << duration.count() << endl;
        if (res[0] == "move")
            api.MoveBee(res[1], stoi(res[2]));
        else if (res[0] == "feedBeeWithNectar")
            api.feedBeeWithNectar(stoi(res[1]));
        else if (res[0] == "convertNectarToHoney")
            api.convertNectarToHoney(stoi(res[1]));
        else if (res[0] == "skipATurn")
            api.skipATurn();

        if (!gameState["winnerTeamName"].is_null())
            break;
    }
    return 0;
}