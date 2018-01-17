//
// Created by David Nakash on 30/12/2017.
//

#ifndef ADVANCED_PROGRAMMING_O_D_SERVERSTRUCTS_H

#define ADVANCED_PROGRAMMING_O_D_SERVERSTRUCTS_H

#include <string>
class CommandManager;

using namespace std;


enum clientStatus {ClientChoice, StartPlaying, Playing, ClientEndGame};

/**
 Holds the client data - socket and game status..
 */
typedef struct ClientThread {
    int clientSocket;
    clientStatus status;
} ClientThread;

/**
 Holds two instances of ClientThread from above - each game holds one of these...
 */
typedef struct GameThread {
    ClientThread player1;
    ClientThread player2;

} GameThread;

/**
 Not used at this time..
 */
typedef struct CmdArg {
    CommandManager *commandManager;
    ClientThread clientThread;
    string name;
    string param;
} CmdArg;





#endif //ADVANCED_PROGRAMMING_O_D_SERVERSTRUCTS_H
