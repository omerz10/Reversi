//
// Created by omerz on 31/12/17.
//

#include <zconf.h>
#include "EndCommand.h"
#define DATALEN 512

void EndCommand::execute(ClientThread *clientT, string args) {
    char buffer[DATALEN] = "End";
    write(clientT->clientSocket, &buffer, DATALEN);

    map<string, GameThread>::iterator it;
    for (it = this->controller->getGames().begin(); it != this->controller->getGames().end(); it++) {
        if (it->second.player1.clientSocket == clientT->clientSocket
            or it->second.player2.clientSocket == clientT->clientSocket) {
            this->controller->getClientThread(it->first, 1)->status = ClientEndGame;
            this->controller->getClientThread(it->first, 2)->status = ClientEndGame;
        }
    }
}

EndCommand::EndCommand(Controller *controller) {
    this->controller = controller;
}



