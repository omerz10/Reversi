//
// Created by omerz on 07/12/17.
//

#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include "Client.h"

#define DATALEN 512

using namespace std;

Client::Client(const char *serverIP, int serverPort): serverIP(serverIP), serverPort(serverPort), clientSocket(0) {}


void Client::connectToServer() {
    // create a socket
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        throw "Error: opening socket";
    }

    // Convert the ip string to a network address
    struct in_addr address;
    if (!inet_aton(this->serverIP, &address)) {
        throw "Can't parse IP address";
    }

    // Get a hostent structure for the given host address
    struct hostent *server;
    server = gethostbyaddr((const void *)&address, sizeof address, AF_INET);
    if (server == NULL) {
        throw "Host is unreachable";
    }
    // Create a structure for the server address
    struct sockaddr_in serverAddress;
    bzero((char *)&address, sizeof(address));
    serverAddress.sin_family = AF_INET;
    memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    // htons converts values between host and network byte
    //orders
    serverAddress.sin_port = htons(this->serverPort);
    // Establish a connection with the TCP server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error connecting to server";
    }
    
}

void Client::waitingForOtherPlayer() { // eat the read
    char buff[DATALEN] = "";

    if (read(clientSocket, buff, DATALEN) == -1) {
        throw "Error reading result from socket";
    }

    if (strcmp(buff, "secondJoined") == 0) { // this is the FIRST player (one that STARTED the game)
        cout << "Second player joined" << endl;
    }
    else if(strcmp(buff, "startGame") == 0) { // this is the SECOND player (one that JOINED the game)
        cout << "Game has started..." << endl;
    }
    else {
        cout << "Error: Player did not receive a message..." << endl;
    }
}


int Client::getClientSock() {
    return this->clientSocket;
}

void Client::getMessage(char *buffer) {
    char temp[DATALEN] ="";
    
    if (read(clientSocket, temp, DATALEN) == -1) {
        throw "Error reading from server";
    }
    strcpy(buffer, temp);
}

void Client::sendExercise(char *buffer) {
    char temp[DATALEN];
    strcpy(temp, buffer);
    if (write(clientSocket, &temp, DATALEN) == -1) {
        throw "Error: writing buffer";
    }
}

void Client::setID(int id) {
    this->id = id;

}

int Client::getID() {
    return this->id;
}

