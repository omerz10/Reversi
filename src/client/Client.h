//
// Created by omerz on 07/12/17.
//

#ifndef ADVANCED_PROGRAMMING_O_D_CLIENT_H
#define ADVANCED_PROGRAMMING_O_D_CLIENT_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

class Client {

private:
    const char *serverIP;
    int serverPort;
    int clientSocket;
    int id;

public:
    /**
     Constructor.
     */
    Client(const char *serverIP, int serverPort);

    /**
     this func connects the client with the server.
     */
    void connectToServer();

    /**
     This function waits for input from server so that the players may begin the game at the same time - in sync..
     */
    void waitingForOtherPlayer();

    /**
     Returns the client's sock number

     @return int socket num
     */
    int getClientSock();

    /**
     This function sends the message in buffer to the server according to the client's sock number.

     @param buffer buffer to send
     */
    void sendExercise(char *buffer);

    /**
     This function receives a message from the server and inputs it into the buffer.

     @param buffer receiving buffer
     */
    void getMessage(char *buffer);

    /**
     Sets client ID.

     @param id client
     */
    void setID(int id);

    /**
     Gets client ID.

     @return client ID
     */
    int getID();
};


#endif //ADVANCED_PROGRAMMING_O_D_CLIENT_H
