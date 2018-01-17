//
// Created by David Nakash on 04/01/2018.
//

#ifndef REVERSI_LOBBY_H
#define REVERSI_LOBBY_H


#include <cstdio>
#include <fstream>
#include <sstream>
#include "GameFlow.h"

class Lobby {

public:

    /**
     This function builds the server details and returns the struct object it created.

     @param fileName settings file path
     @return struct ServerDetails
     */
    ServerDetails getServerDetails(string *fileName);

    /**
     This function is in charge of the menu selection at the beginning of the client's input.

     @param playerSelection value passed by ref from main func
     */
    void menuSelection(int *playerSelection);

    /**
     Menu function in charge of building the objects, running the game, etc.
     */
    void menu();

    /**
     This function handles the online game part, creation of objects, etc.

     @param client Client obj by ref
     @param board Board obj by ref
     @param gameLogic Logic obj by ref
     */
    void runOnlineGame(Client *client, Board *board, GenericLogic *gameLogic);

};

#endif //REVERSI_LOBBY_H
