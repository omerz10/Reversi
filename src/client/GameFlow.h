/*
 * GameFlow.h
 *	Omer Zucker
 *	ID: 200876548
 *  Created by omerz on 30/10/17.
 *      Author: zuckerod
 */

#ifndef EX2_GAMEFLOW_H
#define EX2_GAMEFLOW_H

enum ClientSelection {Choosing, StartGame, JoinGame, ShowListOfGames};

#include "Board.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Game.h"
#include "GenericLogic.h"
#include "Client.h"

/*
 * Class GameFlow managed an end to end flow of the game
 */
class GameFlow {

private:

    Game *game;
    Client *client;

public:

    /*
     * constructor
     * construct GameFlow by the first player and the board
     */
    GameFlow(Game* g);

    /*
     * constructor overloading
     * construct GameFlow by the first player and the board, this time including a client for the online game.
     */
    GameFlow(Game *g, Client *client);

    /*
     * the function responsible for running the game from A to Z, including printing gui's notifications and
     * inputs requests from the user.
     * the function update the gameFlow by boards status and player switches
     */
    void play();

    /*
     * prints the last player's move if it exists.
     * @param Player    lastPlayer
     * */
    void lastPlayerMoveMsg(Player *lastPlayer, bool playerMoves);


    /*
     * Will be called when we need to update the scores..
     */
    void updateScores();

    /*
     * Shows the endgame score to the terminal
     */
    void showScores();

    /**
     this function parses a Cell obj into a string

     @param c cell obj
     @param buff buff to parse to
     */
    void parseToString(Cell c, char *buff);
    /**
     This function parses from a string and returns a Cell obj accordingly.

     @param str buffer to parse from
     @return Cell generated cell.
     */
    Cell parseFromString(char *str);
    /**
     This function is in charge of the online play logic.
     */
    void playOnline();

};


#endif //EX2_GAMEFLOW_H
