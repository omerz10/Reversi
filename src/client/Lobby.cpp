//
// Created by David Nakash on 04/01/2018.
//
enum CommandType {Start = 1, Join = 2, ListGames = 3};
#include "Lobby.h"
#define BOARD_SIZE 8
#define DATALEN 512

using namespace std;

ServerDetails Lobby::getServerDetails(string *fileName) {
    ServerDetails serverDetails;
    string buffer;
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file.open((*fileName).c_str());
        while (!file.eof()) {
            getline(file, buffer);
        }
        file.close();
    }
    catch (ifstream::failure &e) {
        cerr << "Exception opening/reading/closing file\n";
    }
    // Insert the string into a stream
    stringstream ss(buffer);
    // extract ip and port
    string temp;
    char *ptr;
    ss >> serverDetails.serverIP;
    ss >> temp;
    istringstream(temp.c_str()) >> serverDetails.serverPort;
    return serverDetails;
}


void Lobby::menuSelection(int *playerSelection) {
    int userInput = 0;
    bool flag = false;
    cout << "Welcome to Reversi!" << endl << endl;
    cout << "Choose an oppenent type:" << endl;
    cout << "1. A human local player" << endl << "2. An AI player" << endl << "3. A remote player" << endl;
    while (!flag) {
        cin >> userInput;

        if (cin.fail()) {
            cin.clear(); // clears error flags
            cin.ignore();

            // user inputs wrong type of input, print message..
            if (!flag) {
                cout << "Characters are not allowed, Please try again !" << endl;
            }
        } else {
            // check if input is legal?
            flag = (userInput == 1) or (userInput == 2) or (userInput == 3);
            // move is illegal, print message..
            if (!flag) {
                cout << "Illegal move, Please enter 1 to play against "
                        "Human Player and 2 to play against AIPlayer!" << endl;
            }
        }
    }
    cin.clear();
    *playerSelection = userInput;
}



bool validateInput(CommandType commandType, string commandName, string commandParam) {

    if (commandType == Start) {
        return (strcmp(commandName.c_str(), "start") == 0) and (commandParam.size() != 0);
    } else if (commandType == Join) {
        return (strcmp(commandName.c_str(), "join") == 0) and (commandParam.size() != 0);
    } else if (commandType == ListGames) {
        return (strcmp(commandName.c_str(), "list_games") == 0) and (commandParam.size() == 0);
    } else {
        return false;
    }

}

void Lobby::menu() {

    string path = "../exe/clientConfig.txt";
    int playerSelection;
    char buff[DATALEN];
    memset(&buff, 0, sizeof(buff));
    menuSelection(&playerSelection);
    cin.get();
    // play against a player
    if (playerSelection == 1) {
        Board board = Board(BOARD_SIZE); // board is created and initialized
        GenericLogic gameLogic = GenericLogic();
        HumanPlayer p1 = HumanPlayer('X', &board, &gameLogic);
        HumanPlayer p2 = HumanPlayer('O', &board, &gameLogic);
        Game game = Game(&p1, &p2);

        GameFlow gameFlow = GameFlow(&game);
        gameFlow.play();
    }
    // play against a AI player
    if (playerSelection == 2) {
        Board board = Board(BOARD_SIZE);
        GenericLogic gameLogic = GenericLogic();
        HumanPlayer p1 = HumanPlayer('X', &board, &gameLogic);
        AIPlayer aiPlayer = AIPlayer('O', &board, &gameLogic);
        Game game = Game(&p1, &aiPlayer);
        GameFlow gameFlow = GameFlow(&game);
        gameFlow.play();
    }
    // play against an remote player
    if (playerSelection == 3) {

        GenericLogic gameLogic = GenericLogic();
        Board board = Board(BOARD_SIZE);
        ServerDetails serverDetails = getServerDetails(&path);
        Client client = Client(serverDetails.serverIP.c_str(), serverDetails.serverPort);
        // connect
        client.connectToServer();
        runOnlineGame(&client, &board, &gameLogic);

    }
}
void Lobby::runOnlineGame(Client *client, Board *board, GenericLogic *gameLogic) {
    char buffer[DATALEN] = "";
    char temp[DATALEN] = "";
    int player = 3;
    bool flag = false;
    int selection;
    cout << "Waiting for server response" << endl;
    read(client->getClientSock(), buffer, DATALEN);
    cout << "Connected to server" << endl;

    do {
        cout << "Please choose one of the following options:" << endl
             << "1. Start a new online game." << endl
             << "2. Join an ongoing game." << endl
             << "3. Show list of joinable games." << endl;

        cin >> selection;
        cin.get();
        switch(selection) {
            case Start:
            {
                string command, commandName, commandParam, userInput;
                cout << "Please type the command 'start' followed by game name." << endl;
                // get user input
                getline(cin, userInput);
                stringstream ss1(userInput);
                ss1 >> commandName;
                ss1 >> commandParam;
                flag = validateInput(Start, commandName, commandParam);
                if (flag) {
                    // create command
                    command = commandName + " " + commandParam;
                    // memsets..
                    memset(temp, '\0', DATALEN);
                    memset(buffer, '\0', DATALEN);
                    // send to server
                    strcpy(temp, command.c_str());
                    write(client->getClientSock(), &temp, DATALEN);// in case of error use &
                    // wait for answer
                    read(client->getClientSock(), buffer, DATALEN);
                    if (strcmp(buffer, "start") == 0) {
                        player = 1;
                        cout << "Waiting for other player to join..." << endl;
                        break;
                    } else if (strcmp(buffer, "gameExists") == 0) {
                        cout << "Game already exists..." << endl;
                        break;
                    } else {
                        cout << "Wrong input.." << endl;
                        break;
                    }
                } else {
                    cout << "Wrong command format.." << endl;
                    break;
                }
            }

            case Join: {
                string command, commandName, commandParam, userInput;
                cout << "Please type the command 'join' followed by game name." << endl;
                // get user input
                getline(cin, userInput);
                stringstream ss2(userInput);
                ss2 >> commandName;
                ss2 >> commandParam;
                flag = validateInput(Join, commandName, commandParam);
                if (flag) {
                    // create command
                    command = commandName + " " + commandParam;
                    // memsets
                    memset(temp, '\0', DATALEN);
                    memset(buffer, '\0', DATALEN);
                    // send to server
                    strcpy(temp, command.c_str());
                    write(client->getClientSock(), &temp, DATALEN);// in case of error use &
                    // wait for answer
                    read(client->getClientSock(), buffer, DATALEN);
                    // receive answer from server
                    if (strcmp(buffer, "join") == 0) {
                        player = 2;
                        break;
                    } else if (strcmp(buffer, "cantJoin") == 0) {
                        cout << "Cannot join this game..." << endl;
                        break;
                    } else {
                        cout << "Error executing.." << endl;
                        break;
                    }
                } else {
                    cout << "Wrong command format.." << endl;
                    break;
                }
            }
            case ListGames:
            {
                string commandName, userInput;
                cout << "Please type the command 'list_games'." << endl;
                // get user input
                getline(cin, userInput);
                stringstream ss3(userInput);
                ss3 >> commandName;
                flag = validateInput(ListGames, commandName, "");
                if (flag) {
                    // memsets
                    memset(temp, '\0', DATALEN);
                    // send to server
                    strcpy(temp, commandName.c_str());
                    write(client->getClientSock(), &temp, DATALEN);// in case of error use &
                    memset(buffer, '\0', DATALEN);
                    // wait for answer
                    read(client->getClientSock(), buffer, DATALEN);
                    if (strcmp(buffer,"emptyList") == 0) {
                        cout << "Game list is empty..." << endl;
                        break;
                    } else {
                        // receive answer from server
                        int i = 1;
                        stringstream ss(buffer);
                        string tempString;
                        while (ss >> tempString) {
                            cout << i << ". " << tempString << endl;
                            i++;
                        }
                        break;
                    }
                } else {
                    cout << "Wrong command format.." << endl;
                    break;
                }
            }
            default:
                cout << "Wrong input, please choose again.." << endl;
                break;
        }
    } while (player == 3);

    // get player number from server
    client->waitingForOtherPlayer();

    // the client got "1" from server
    if (player == 1) {
        HumanPlayer p1 = HumanPlayer('X', board, gameLogic);
        HumanPlayer p2 = HumanPlayer('O', board, gameLogic);
        Game game = Game(&p1, &p2);
        GameFlow gameFlow = GameFlow(&game, client);
        client->setID(1);
        gameFlow.playOnline();
    // the client got "2" from server
    } else if (player == 2) {
        HumanPlayer p1 = HumanPlayer('O', board, gameLogic);
        HumanPlayer p2 = HumanPlayer('X', board, gameLogic);
        Game game = Game(&p1, &p2);
        GameFlow gameFlow = GameFlow(&game, client);
        client->setID(2);
        gameFlow.playOnline();
    }
}
