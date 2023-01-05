cmake_minimum_required(VERSION 3.22)
project(GhostHunters C)

set(CMAKE_C_STANDARD 99)

add_executable(GhostHunters
        main.c
        gameDefs.h
        sharedFunctions.c
        building.c
        room.c
        ghost.c
        evidence.c
        hunter.c
        )
