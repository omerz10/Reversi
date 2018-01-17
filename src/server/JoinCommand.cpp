//
// Created by David Nakash on 24/12/2017.
//


#include "JoinCommand.h"
#define DATALEN 512
pthread_mutex_t joinMutex;

void JoinCommand::execute(ClientThread *clientT, string args) {
    char buffer1[DATALEN] = "";
    char buffer2[DATALEN] = "";
    char buffer3[DATALEN] = "";
    bool found = false;

    // iterator
    unsigned long sizeOfVector = this->controller->getJoinableGames().size();
    unsigned long i;
    for (i = 0; i < sizeOfVector; i++) {
        if (this->controller->getJoinableGames()[i] == args) {
            found = true;
            break;
        }
    }
    if (found) {
        pthread_mutex_lock(&joinMutex);
        // insert 2nd player into the game
        this->controller->joinGame(args, clientT);

        this->controller->deleteJoinableGame(i);
        pthread_mutex_unlock(&joinMutex);
        cout << "Server completed connection with 2 players.." << endl
             << "-------- The Game '" << args << "' Begins --------" << endl;

        strcpy(buffer1, "secondJoined");
        // send
        if (write(this->controller->getGames()[args].player1.clientSocket, &buffer1, DATALEN) == -1) {
            throw ("Error: sending to player 1");
        }
        strcpy(buffer2, "join");
        // send
        if (write(this->controller->getGames()[args].player2.clientSocket, &buffer2, DATALEN) == -1) {
            throw ("Error: sending to player 2");
        }
        pthread_mutex_lock(&joinMutex);
        clientT->status = Playing;
        // the players will now start playing with each other
        this->controller->getClientThread(args, 1)->status = Playing;
        this->controller->getClientThread(args, 2)->status = Playing;
        pthread_mutex_unlock(&joinMutex);
        strcpy(buffer3, "startGame");
        if (write(this->controller->getGames()[args].player2.clientSocket, &buffer3, DATALEN) == -1) {
            throw ("Error: sending to player 2");
        }
        this->controller->runOneGame(args);
    } else {
        // return error value
        char buffer4[DATALEN] = "";
        strcpy(buffer4, "cantJoin");
        if (write(clientT->clientSocket, &buffer4, DATALEN) == -1) {
            throw ("Error: sending to player 1");
        }
    }

}


JoinCommand::JoinCommand(Controller *controller) {
    this->controller = controller;
}

