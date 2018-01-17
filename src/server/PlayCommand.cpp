//
// Created by David Nakash on 24/12/2017.
//

#include <zconf.h>
#include "PlayCommand.h"

#define DATALEN 512


void PlayCommand::execute(ClientThread *clientT, string args) {
    char buffer[DATALEN] = "";
    strcpy(buffer, args.c_str());
    write(clientT->clientSocket, &buffer, DATALEN);
}

PlayCommand::PlayCommand(Controller *controller) {
    this->controller = controller;
}
