//
// Created by David Nakash on 28/11/2017.
//


#ifndef PROJECT_UPDATED_STRUCTS_H
#define PROJECT_UPDATED_STRUCTS_H

#include "Board.h"

/**
 Struct that holds the possible moves (in order to make it easier for the programmer...).
 */


/**
 This struct holds the possible moves each player gets from game logic...
 */
typedef struct PossibleMove {

    vector<vector<Cell> > changeableCells;

    vector<Cell> moves;

    bool possible;
} PossibleMove;


/**
 This struct holds the server details.
 */
typedef struct ServerDetails {
    string serverIP;
    int serverPort;

} ServerDetails;


#endif //PROJECT_UPDATED_STRUCTS_H
