cmake_minimum_required(VERSION 3.8)
project(Reversi)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS -pthread)

set(CLIENT_FILES
        src/client/AIPlayer.cpp
        src/client/AIPlayer.h
        src/client/Board.cpp
        src/client/Board.h
        src/client/Cell.cpp
        src/client/Cell.h
        src/client/Client.cpp
        src/client/Client.h
        src/client/Game.cpp
        src/client/Game.h
        src/client/GameFlow.cpp
        src/client/GameFlow.h
        src/client/GenericLogic.cpp
        src/client/GenericLogic.h
        src/client/HumanPlayer.cpp
        src/client/HumanPlayer.h
        src/client/Logic.h
        src/client/main.cpp
        src/client/Player.h
        src/client/Structs.h src/client/Lobby.cpp src/client/Lobby.h)

set(SERVER_FILES
        src/server/Command.h
        src/server/CommandManager.cpp
        src/server/CommandManager.h
        src/server/Controller.cpp
        src/server/Controller.h
        src/server/EndCommand.cpp
        src/server/EndCommand.h
        src/server/JoinCommand.cpp
        src/server/JoinCommand.h
        src/server/ListGamesCommand.cpp
        src/server/ListGamesCommand.h
        src/server/main.cpp
        src/server/NoMoveCommand.cpp
        src/server/NoMoveCommand.h
        src/server/PlayCommand.cpp
        src/server/PlayCommand.h
        src/server/Server.cpp
        src/server/Server.h
        src/server/ServerStructs.h
        src/server/StartCommand.cpp
        src/server/StartCommand.h src/server/Task.cpp src/server/Task.h src/server/ThreadPool.cpp src/server/ThreadPool.h)

add_executable(Client ${CLIENT_FILES} src/client/main.cpp src/client/AIPlayer.cpp
        src/client/AIPlayer.h
        src/client/Board.cpp
        src/client/Board.h
        src/client/Cell.cpp
        src/client/Cell.h
        src/client/Client.cpp
        src/client/Client.h
        src/client/Game.cpp
        src/client/Game.h
        src/client/GameFlow.cpp
        src/client/GameFlow.h
        src/client/GenericLogic.cpp
        src/client/GenericLogic.h
        src/client/HumanPlayer.cpp
        src/client/HumanPlayer.h
        src/client/Logic.h src/client/Player.h
        src/client/Structs.h)
add_executable(Server ${SERVER_FILES} src/server/main.cpp src/server/Command.h
        src/server/CommandManager.cpp
        src/server/CommandManager.h
        src/server/Controller.cpp
        src/server/Controller.h
        src/server/EndCommand.cpp
        src/server/EndCommand.h
        src/server/JoinCommand.cpp
        src/server/JoinCommand.h
        src/server/ListGamesCommand.cpp
        src/server/ListGamesCommand.h src/server/NoMoveCommand.cpp
        src/server/NoMoveCommand.h
        src/server/PlayCommand.cpp
        src/server/PlayCommand.h
        src/server/Server.cpp
        src/server/Server.h
        src/server/ServerStructs.h
        src/server/StartCommand.cpp
        src/server/StartCommand.h)