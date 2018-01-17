#include "Lobby.h"

/*
 * main.cpp
 *	Omer Zucker
 *	ID: 200876548
 *
 *  David Nakash
 *  ID: 203752902
 */



int main() {
    Lobby *lobby = new Lobby();
    lobby->menu();
    delete (lobby);
    return 0;
}