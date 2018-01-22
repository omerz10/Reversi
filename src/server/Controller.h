//
// Created by omerz on 02/01/18.
//

#ifndef ADVANCED_PROGRAMMING_O_D_CONTROLLER_H
#define ADVANCED_PROGRAMMING_O_D_CONTROLLER_H

#include <zconf.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include "ServerStructs.h"
#include <pthread.h>
#include "Task.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>

class Controller {

private:
    vector<pthread_t> clientsThreads; // vector of pthreads - manages threads...

    vector <ClientThread> socketsStatus; // vector of ClientThread (which has clientSocket and status)

    map < string , GameThread > games; // map of games

    vector <string> joinable_games; // vector of joinable games

    vector <Task*> tasks;

    ThreadPool *threadPool;

public:

    Controller() {
        this->threadPool = NULL;
    }

    ~Controller() {
        for (unsigned int i = 0; i < tasks.size(); i++) {
            delete this->tasks[i];
        }
        delete threadPool;
    }
    // runners
    /**
     This function runs the server, calls mainThreadListener.
     */
    void runServer();
    /**
     This function is in charge of listening to incoming client connections.

     @param serverSocket socket info
     */
    void mainThreadListener(int serverSocket); // controller function that executes the void* funcs (in threads..)
    /**
     This function runs one instance of a game, sends and receives information using read and write and the generic commands (doesn't know anything about the game commands)

     @param gameName <#gameName description#>
     */
    void runOneGame(string gameName);
    /**
     This function closes the clients' connections - used at the end of each game..
     */
    void closeClientsConnections();

    // getters
    /**
     Gets joinable games vector

     @return joinable games vector
     */
    vector <string> getJoinableGames();
    /**
     Gets all of the clients' thread

     @return vector of threads
     */
    vector<pthread_t> getClientsThreads(); // get vector of threads
    /**
     Gets the socket statuses of all the players.

     @return client thread vector
     */
    vector <ClientThread> getSocketsStatus();
    /**
     Returns the games map -> "gameName":GameThread format

     @return map of games
     */
    map < string, GameThread > getGames();
    /**
     Gets the client thread.

     @param gameName string
     @param player number
     @return ClientThread struct
     */
    ClientThread *getClientThread(string gameName, int player);
    /**
     Gets all the game names from joinable games vector

     @return returns a string of game names delimited by space.
     */
    string getGameNames();

    // setters
    /**
     Adds a thread to the thread vector.

     @param thread to add
     */
    void addThread(pthread_t thread);
    /**
     Adds a new game to the games and joinable games map and vector.

     @param gameName desc
     @param clientThread info
     */
    void addNewGame(string gameName, ClientThread *clientThread);
    /**
     Joins the game, changes sockets in ClientThread accordingly, adds the player to the game and launches playOneGame.

     @param gameName desc
     @param clientThread desc
     */
    void joinGame(string gameName, ClientThread *clientThread);
    /**
     Deletes a joinable game using index.

     @param index desc
     */
    void deleteJoinableGame(int index);
    /**
     Deletes a game from the normal games map using gamename (key)

     @param gameName key
     */
    void deleteGame(string gameName);
    /**
     Removes a specific socket from the vector of ClientThreads supplied.

     @param v <#v description#>
     */
    void removeSockets(vector <ClientThread> v);

    // polling
    /**
     Polling func, this func asks the client if he is still connected.
     Used mainly to detect disconnections in midgame..

     @param clientNumber client socket info
     @return true/false
     */
    bool isClientClosed(int clientNumber);
    /**
     Helper function to ClientClosed..

     @param currentClient first client socket
     @param otherClient second client socket
     @return true/false
     */
    bool pollClient(int currentClient, int otherClient);

    // parsing
    /**
     Parsing func to CmdArg struct

     @param msg string
     @param clientSocket desc
     @return struct CmdArg
     */
    CmdArg parseMessage(string msg, int clientSocket);

    void addNewTask(Task* task);

};


#endif //ADVANCED_PROGRAMMING_O_D_CONTROLLER_H
