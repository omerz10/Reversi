//
// Created by David Nakash on 23/12/2017.
//

#include <cstring>
#include "StartCommand.h"

#define GAME_NAME 0  // name of game chosen by user
#define DATALEN  512
pthread_mutex_t startMutex;

void StartCommand::execute(ClientThread *clientT, string args) {
    char temp[DATALEN] = "";

    // check if game exists
    if (this->controller->getGames().count(args) == 0) { // not found
        pthread_mutex_lock(&startMutex);
        clientT->status = StartPlaying;
        this->controller->addNewGame(args, clientT);
        pthread_mutex_unlock(&startMutex);
        strcpy(temp, "start");
        if (write(this->controller->getGames()[args].player1.clientSocket, &temp, DATALEN) == -1) {
            throw ("Error: sending to player 1");
        }
    } else { // game found, can't start it again...
        strcpy(temp, "gameExists");
        write(clientT->clientSocket, &temp, DATALEN);
    }
}

StartCommand::StartCommand(Controller *controller) {
    this->controller = controller;
}


