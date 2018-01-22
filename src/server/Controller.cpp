//
// Created by omerz on 02/01/18.
//


#include <poll.h>
#include "Controller.h"
#include "CommandManager.h"

#define MAX_CLIENTS 1000
#define DATALEN 512
#define THREADS_NUM 5


CommandManager *cManager;


struct ClientData {
    Controller *controller;
    ClientThread clientThread;
};


struct ServerData {
    int serverSocket;
    Controller *controller;
};

void executeEnvelope(ClientThread* clientT, string command) {
    cManager->executeCommand(clientT, command);
}

void *handleClient(void *data) {
    ClientData *cData = (ClientData *)data;
    //write(cData->clientThread.clientSocket, "You connected to server",DATALEN);
    // get command from client
    char buffer[DATALEN] = "";
    string commandString;
    ssize_t statusRead;

    while (cData->clientThread.status == ClientChoice) {
        statusRead = read(cData->clientThread.clientSocket, buffer, DATALEN);
        if (statusRead == 0) {
            close(cData->clientThread.clientSocket);
            break;
        }
        commandString = buffer;
        executeEnvelope(&cData->clientThread, commandString);
    }
    return NULL;
}
void *clientsListener(void* serverData) {
    //pthread_t newThread;
    ClientData clientData;
    bzero((void *) &clientData, sizeof(clientData));
    ServerData *sData = (ServerData *) serverData;
    struct sockaddr_in clientAddress;
    socklen_t client1AddressLen = sizeof((struct sockaddr *) &clientAddress);

    while(true) {
        cout << "Waiting for client connections..." << endl;
        // Accept a new client connection
        int clientSocket = accept(sData->serverSocket, (struct sockaddr *) &clientAddress
                , &client1AddressLen);
        if (clientSocket == -1) {
            throw "Error on accept";
        }
        cout << "Received connection from " << inet_ntoa(clientAddress.sin_addr) << " port " <<
             ntohs(clientAddress.sin_port) << endl;

        clientData.clientThread.clientSocket = clientSocket;
        clientData.clientThread.status = ClientChoice;
        clientData.controller = sData->controller;

        Task* task = new Task(handleClient, &clientData);
        sData->controller->addNewTask(task);
        // remove sockets and statuses from vector of ClientThread
        sData->controller->removeSockets(sData->controller->getSocketsStatus());
    }
    return NULL;
}


/*
 * main thread listener
 */
void Controller::mainThreadListener(int serverSocket) {
    this->threadPool = new ThreadPool(THREADS_NUM);
    pthread_t mainThread;
    ServerData serverData;
    bzero((void*) &serverData, sizeof(serverData));

    serverData.serverSocket = serverSocket;
    serverData.controller = this;
    CommandManager manager(this);
    cManager = &manager;

    cout << "Listening to clients.." << endl;
    listen(serverSocket, MAX_CLIENTS);

    int rc = pthread_create(&mainThread, NULL, clientsListener, &serverData);
    if (rc) {
        cout << "Error: unable to create thread, " << rc << endl;
        exit(-1);
    }
    string input;
    do {
        getline(cin, input);
    } while(strcmp(input.c_str(), "exit") != 0);

    threadPool->terminate();
    // close main thread
    pthread_cancel(mainThread);
    pthread_join(mainThread, NULL);
}

vector<pthread_t> Controller::getClientsThreads() {
    return clientsThreads;
}

void Controller::addThread(pthread_t thread) {
    this->clientsThreads.push_back(thread);
}

void Controller::removeSockets(vector <ClientThread> socketsStatus) {
    vector<ClientThread>::iterator it;
    for (it = socketsStatus.begin(); it != socketsStatus.end(); it++) {
        if (it->status == ClientEndGame) {
            socketsStatus.erase(it);
        }
    }

}

vector <ClientThread> Controller::getSocketsStatus() {
    return this->socketsStatus;
}

void Controller::closeClientsConnections() {
    vector <ClientThread>::iterator it;
    char buffer[DATALEN] = "";
    strcpy(buffer, "exit");
    for (it = this->socketsStatus.begin(); it != this->socketsStatus.end(); it++) {
        write(it->clientSocket, &buffer, DATALEN);
        close(it->clientSocket);
    }
}

void Controller::runOneGame(string gameName) {
    int clientSocket1 = this->games[gameName].player1.clientSocket;
    int clientSocket2 = this->games[gameName].player2.clientSocket;

    // init buffer for getting msg from player
    char buffer[DATALEN] = "";
    char temp[DATALEN] = "";
    // messages from each player
    ssize_t clientMessage;
    // send & receive from players until gets "isEnd" message
    while(this->games[gameName].player1.status == Playing && this->games[gameName].player2.status == Playing) {
        // read move from player 1
        memset(buffer, 0, DATALEN);
        clientMessage = read(clientSocket1, buffer, DATALEN);
        // check input
        if (clientMessage == 0 or clientMessage == 1) {
            cout << "Error: First player has disconnected from the game during his turn.. ending game!" << endl;
            break;
        }
        memset(temp, 0, DATALEN);
        strcpy(temp, buffer);
        // check if connection still alive with first client
        if (pollClient(clientSocket1, clientSocket2)) {
            cout << "Error : Second player is no longer connected.. ending game!" << endl;
            break;
        }
        ClientThread clientT1;
        clientT1.clientSocket = clientSocket2;
        clientT1.status = this->games[gameName].player2.status;
        executeEnvelope(&clientT1, temp);
        // end of game
        if (clientT1.status == ClientEndGame) {
            // we will delete game from games list of server afterwards
            break;
        }
        // read from player 2's client
        memset(temp, 0, DATALEN);
        memset(buffer, 0, DATALEN);
        clientMessage = read(clientSocket2, buffer, DATALEN);
        if (clientMessage == 0 or clientMessage == 1) {
            cout << "Error: Second player has disconnected from the game during his turn... ending game!" << endl;
            break;
        }
        strcpy(temp, buffer);
        // check if connection still alive with first client
        if (pollClient(clientSocket2, clientSocket1)) {
            cout << "Error : First player is no longer connected.. ending game!" << endl;
            break;
        }
        ClientThread clientT2;
        clientT2.clientSocket = clientSocket1;
        clientT2.status = this->games[gameName].player1.status;
        executeEnvelope(&clientT2, temp);
        // end of game
        if (clientT2.status == ClientEndGame) {
            // we will delete game from games list of server afterwards
            break;
        }
    } // end of while
    cout << "Game " << gameName << " has ended.." << endl;
    // delete game from map..
    this->deleteGame(gameName);
    // close client sockets
    close(clientSocket1);
    close(clientSocket2);
}

bool Controller::isClientClosed(int clientNumber) {
    pollfd pfd;
    pfd.fd = clientNumber;
    pfd.events = POLLIN | POLLHUP | POLLRDNORM;
    pfd.revents = 0;
    if (pfd.revents == 0) {
        // call poll every 100 miliseconds
        if (poll(&pfd, 1, 100) > 0) {
            // if result is bigger than 0, it means there is either data
            // on the socket, or played closed his window(closed socket)
            char buff[32];
            if (recv(clientNumber, buff, sizeof(buff), MSG_PEEK | MSG_DONTWAIT) == 0) {
                // if recv function returns zero it means the connection has been closed.
                return true;
            }
        }
    }
    return false;
}

bool Controller::pollClient(int currentClient, int otherClient) {
    char temp[DATALEN] = "";
    // check for lost connection
    if (isClientClosed(otherClient)) {
        cout << "Other player has disconnected from the game, ending..." << endl;
        strcpy(temp, "End");
        write(currentClient, &temp, DATALEN);
        return true;
    }
    return false;
}

map< string, GameThread > Controller::getGames() {
    return this->games;
}


void Controller::deleteGame(string gameName) {
    // update status of both player
    this->games[gameName].player1.status = ClientEndGame;
    this->games[gameName].player2.status = ClientEndGame;
    // delete game
    this->games.erase(gameName);
}

void Controller::addNewGame(string gameName, ClientThread *clientThread) {
    GameThread gameThread;
    bzero((void*) &gameThread, sizeof(gameThread));
    gameThread.player1 = *clientThread;
    // insert new game to games
    this->games.insert(std::pair<string , GameThread> (gameName, gameThread));
    this->joinable_games.push_back(gameName);
}
void Controller::joinGame(string gameName, ClientThread *clientThread) {
    this->games[gameName].player2 = *clientThread;
}
vector <string> Controller:: getJoinableGames() {
    return this->joinable_games;
}

ClientThread *Controller::getClientThread(string gameName, int player) {
    if (player == 1) {
        return &(this->games[gameName].player1);
    }
    else {
        return &(this->games[gameName].player2);
    }
}

void Controller::deleteJoinableGame(int index) {
    this->joinable_games.erase(this->joinable_games.begin() + index);
}

string Controller::getGameNames() {
    string res = "";
    for (int i = 0; i < this->joinable_games.size(); i++) {
        res += this->joinable_games[i] + " ";
    }
    return res;
}

void Controller::addNewTask(Task* task) {
    this->tasks.push_back(task);
    this->threadPool->addTask(task);
}

