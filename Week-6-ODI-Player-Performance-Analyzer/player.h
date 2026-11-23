#ifndef PLAYER_H
#define PLAYER_H

struct team;

typedef struct player {
    int playerId;
    char playerName[50];
    char teamName[50];
    char Role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;

    struct player *next;
} player;

void addPlayerToTeam(struct team *teams, int teamId);
void displayTeamPlayers(struct team *teams, int teamId);
void displaySortedTeams(struct team *teams);
void displayTopKPlayers(struct team *teams, int teamId, char *role, int k);
void displayPlayersByRole(struct team *teams, char *role);

#endif