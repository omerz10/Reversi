//
// Created by David Nakash on 24/12/2017.
//


#include "CommandManager.h"



#define DATALEN 512

CommandManager::CommandManager(Controller *controller) {
    this->commands["start"] = new StartCommand(controller);
    this->commands["list_games"] = new ListGamesCommand(controller);
    this->commands["join"] = new JoinCommand(controller);
    this->commands["play"] = new PlayCommand(controller);
    this->commands["NoMove"] = new NoMoveCommand(controller);
    this->commands["End"] = new EndCommand(controller);
}

CommandManager::~CommandManager() {

}


map<string, Command* > CommandManager::getCommands() {
    return this->commands;
}

vector <string> buildResponse(string commandString) {
    vector <string> argV;

    string::size_type pos = commandString.find(' ', 0);
    if (pos != std::string::npos) {
        argV.push_back(commandString.substr(0, pos));
        argV.push_back(commandString.substr(pos + 1));
    } else {
        argV.push_back(commandString);
        argV.push_back("");
    }
    return argV;
}


void CommandManager::executeCommand(ClientThread *clientT, string commandString) {
    vector<string> argV = buildResponse(commandString);
    string commandName = argV.front();
    argV.erase(argV.begin());
    string commandParam = argV.front();

    this->commands[commandName]->execute(clientT, commandParam);


}







