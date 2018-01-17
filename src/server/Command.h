//
// Created by David Nakash on 23/12/2017.
//

#ifndef ADVANCED_PROGRAMMING_O_D_COMMAND_H
#define ADVANCED_PROGRAMMING_O_D_COMMAND_H
#include <vector>
#include "ServerStructs.h"
#include "Controller.h"


using namespace std;


class Command {

public:
    /**
     Generic interface function declaration.

     @param clientT struct with client info
     @param args command argument
     */
    virtual void execute(ClientThread *clientT, string args) = 0;
};

#endif //ADVANCED_PROGRAMMING_O_D_COMMAND_H
