
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "team.h"
#include "Players_data.h"

float calculatePerformanceIndex(Player p)
{
    if (strcmp(p.role, "Batsman") == 0)
    {
        return (p.battingAverage * p.strikeRate) / 100.0f;
    }
    else if (strcmp(p.role, "Bowler") == 0)
    {
        return (p.wickets * 2) + (100 - p.economyRate);
    }
    else
    {
        return ((p.battingAverage * p.strikeRate) / 100.0f)
               + (p.wickets * 2);
    }
}

void initializeTeams(struct team *teamList)
{
    for (int i = 0; i < teamCount; i++)
    {
        teamList[i].teamId = i + 1;
        strcpy(teamList[i].Name, teams[i]);
        teamList[i].totalPlayers = 0;
        teamList[i].averageBattingStrikeRate = 0.0f;
        teamList[i].playerList = NULL;
    }
}

void loadPlayers(struct team *teamList)
{
    for (int i = 0; i < playerCount; i++)
    {
        int teamIndex = -1;
        for (int j = 0; j < teamCount; j++)
        {
            if (strcmp(players[i].team, teamList[j].Name) == 0)
            {
                teamIndex = j;
                break;
            }
        }
        if (teamIndex == -1)
        {
            continue;
        }
        struct player *newPlayer = malloc(sizeof(struct player));
        if (newPlayer == NULL)
        {
            printf("Memory allocation failed!\n");
            return;
        }
        newPlayer->playerId = players[i].id;
        strcpy(newPlayer->playerName, players[i].name);
        strcpy(newPlayer->teamName, players[i].team);
        strcpy(newPlayer->Role, players[i].role);

        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;
        newPlayer->performanceIndex = calculatePerformanceIndex(players[i]);
        newPlayer->next = teamList[teamIndex].playerList;
        teamList[teamIndex].playerList = newPlayer;
        teamList[teamIndex].totalPlayers++;
    }
    for (int i = 0; i < teamCount; i++)
    {
        float totalStrikeRate = 0.0f;
        int battingPlayerCount = 0;
        struct player *current = teamList[i].playerList;
        while (current != NULL)
        {
            if (strcmp(current->Role, "Batsman") == 0 ||
                strcmp(current->Role, "All-rounder") == 0)
            {
                totalStrikeRate += current->strikeRate;
                battingPlayerCount++;
            }

            current = current->next;
        }
        if (battingPlayerCount > 0)
        {
            teamList[i].averageBattingStrikeRate =
                totalStrikeRate / battingPlayerCount;
        }
    }
}

void freePlayers(struct team *teamList)
{
    for (int i = 0; i < teamCount; i++)
    {
        struct player *current = teamList[i].playerList;
        while (current != NULL)
        {
            struct player *temp = current;
            current = current->next;
            free(temp);
        }
        teamList[i].playerList = NULL;
    }
}

int main(void)
{
    struct team teamList[teamCount];
    initializeTeams(teamList);
    loadPlayers(teamList);
    int choice;
    do
    {
        printf("\n========== ODI PLAYER PERFORMANCE ANALYZER ==========\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Team\n");
        printf("3. Display Teams Sorted by Average Batting Strike Rate\n");
        printf("4. Display Top K Players by Role in a Team\n");
        printf("5. Display All Players by Role Across Teams\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
            {}
            continue;
        }
        switch (choice)
        {
            case 1:
            {
                int teamId;
                printf("Enter Team ID: ");
                scanf("%d", &teamId);
                addPlayerToTeam(teamList, teamId);
                break;
            }
            case 2:
            {
                int teamId;
                printf("Enter Team ID: ");
                scanf("%d", &teamId);
                displayTeamPlayers(teamList, teamId);
                break;
            }
            case 3:
                displaySortedTeams(teamList);
                break;
            case 4:
            {
                int teamId;
                int k;
                char role[20];

                printf("Enter Team ID: ");
                scanf("%d", &teamId);
                printf("Enter Role (Batsman/Bowler/All-rounder): ");
                scanf("%19s", role);
                printf("Enter K: ");
                scanf("%d", &k);
                displayTopKPlayers(teamList, teamId, role, k);
                break;
            }
            case 5:
            {
                char role[20];
                printf("Enter Role (Batsman/Bowler/All-rounder): ");
                scanf("%19s", role);

                displayPlayersByRole(teamList, role);
                break;
            }
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);
    freePlayers(teamList);
    return 0;
}
