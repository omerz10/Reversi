//
// Created by omerz on 31/12/17.
//

#include <zconf.h>
#include "NoMoveCommand.h"
#define DATALEN 512
void NoMoveCommand::execute(ClientThread *clientT, string args) {
    char buffer[DATALEN]= "NoMove";
    write(clientT->clientSocket, &buffer, DATALEN);
}

NoMoveCommand::NoMoveCommand(Controller *controller) {
    this->controller = controller;
}
