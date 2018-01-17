//
// Created by David Nakash on 24/12/2017.
//

#include "ListGamesCommand.h"

#define DATALEN 512

void ListGamesCommand::execute(ClientThread *clientT, string args) {
    char temp[DATALEN] = "";
    //map<string, GameThread >::iterator it;
    vector<string> :: iterator it;
    string result;


    if (this->controller->getJoinableGames().empty()) {
        strcpy(temp, "emptyList");
        write(clientT->clientSocket, &temp, DATALEN);
    } else {
        result = this->controller->getGameNames();
        strcpy(temp, result.c_str());
        if (write(clientT->clientSocket, &temp, DATALEN) == -1) {
            throw ("Error: sending to player 1");
        }
    }
}

ListGamesCommand::ListGamesCommand(Controller *controller) {
    this->controller = controller;
}
