#ifndef TEAM_H
#define TEAM_H
#include "player.h"

struct team {
    int teamId;
    char Name[50];
    int totalPlayers;
    float averageBattingStrikeRate;
    player *playerList;
};

#endif