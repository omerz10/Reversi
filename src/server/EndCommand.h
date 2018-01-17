//
// Created by omerz on 31/12/17.
//

#ifndef ADVANCED_PROGRAMMING_O_D_ENDCOMMAND_H
#define ADVANCED_PROGRAMMING_O_D_ENDCOMMAND_H


#include "ServerStructs.h"
#include "Command.h"

#include <map>

class EndCommand : public Command {
private:
    Controller *controller;
public:

    EndCommand(Controller *controller);
    /**
     Generic interface function.
     
     @param clientT struct with client info
     @param args command argument
     */
    void execute(ClientThread *clientT, string args);

};


#endif //ADVANCED_PROGRAMMING_O_D_ENDCOMMAND_H
