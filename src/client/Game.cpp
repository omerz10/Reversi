//
// Created by David Nakash on 27/12/2017.
//

//
// Created by David Nakash on 27/11/2017.
//

#include "Game.h"


Game::Game(Player* P1, Player *P2) {
    // initialize values
    this->P1 = P1;
    this->P2 = P2;


}

// change lastPlayer to member
bool Game::playOneMove(Player *p) {

    string userInput; // stores user input
    bool pMoves;

    // get possible moves for Player..
    p->getPlayerMoves();

    // check if there are any moves at all ?
    pMoves = p->getPossibleMoveStatus();

    // print msg to terminal..
    cout << p->getType() << ": It's your move." << endl;

    // Are there are possible moves for Player?
    if (pMoves) {
        // print GUI and take input here if needed... (HumanPlayer only)
        if (p->printGUI()) {

            string userInput;

            cout << "Your possible moves: ";

            p->printMoves();

            cout << "\nPlease enter your move row, col: ";

            // run loop until gets correct input for possible move
            while (true) {

                getline(cin, userInput);
                //wrong input
                if (!p->checkInput(userInput)) {
                    cout << "\nWrong input! Please enter your move row, col: ";
                }
                    // true input
                else {
                    p->playTurn();
                    break;
                }
            }
        } else {
            p->playTurn();
        }

        // now assign p to lastPlayer ptr for future reference..
        this->lastPlayer = p;
        // return
        return true;
    } else {
        // print msg to user to press any key and "eat" the next char..
        cout << "No possible moves. Play passes back to the other player. Press any key to continue." << endl;
        cin.get();

        // no moves for Player, return false..
        return false;
    }

}


Player* Game :: getP1() {
    return this->P1;
}

Player* Game :: getP2() {
    return this->P2;
}


//
void Game::updatePlayerScores() {

    int countP1 = 0, countP2 = 0;
    // run through the board, and update players' scores..
    for (int i = 0; i < this->lastPlayer->getBoardSize(); i++) {
        for (int j = 0; j < this->lastPlayer->getBoardSize(); j++) {
            if (this->lastPlayer->getBoardValueAtIndexes(i, j) == this->P1->getType()) {
                countP1++;
            } else if (this->getLastPlayer()->getBoardValueAtIndexes(i, j) == this->P2->getType()) {
                countP2++;
            }
        }
    }

    this->scoreP1 = countP1;
    this->scoreP2 = countP2;
}

int Game::getP1Score() {
    return this->scoreP1;
}

int Game::getP2Score() {
    return this->scoreP2;
}

Player* Game::getLastPlayer() {
    return this->lastPlayer;
}