#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"
#include "player.h"
#include "Players_data.h"

void addPlayerToTeam(struct team *teams, int teamId){
    int playerId;
    char name[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    if (scanf("%d", &playerId) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%49s", name) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%19s", role) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%d", &totalRuns) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%f", &battingAverage) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%f", &strikeRate) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%d", &wickets) != 1) {
        printf("Invalid player input\n");
        return;
    }
    if (scanf("%f", &economyRate) != 1) {
        printf("Invalid player input\n");
        return;
    }
    int foundIndex=-1;
    int low=0;
    int high=teamCount-1;
    int mid;
    while (low<=high)
    {
        mid=low+(high-low)/2;
        if (teams[mid].teamId == teamId){
            foundIndex=mid;
            break;
        }
        else if (teams[mid].teamId < teamId )
        {
            low=mid+1;
        }
        else {
            high=mid-1;
        }
    }
    if (foundIndex==-1){
        printf("Team not found\n");
        return;
    }   

    for (int i=0; i<teamCount; i++){
        for (player *existing=teams[i].playerList; existing!=NULL; existing=existing->next){
            if (existing->playerId == playerId){
                printf("Player ID already exists\n");
                return;
            }
        }
    }

    struct player *newPlayer;
    newPlayer=malloc(sizeof(struct player));
    if (newPlayer==NULL){
        printf("Allocation failed!\n");
        return;
    }

    newPlayer->playerId=playerId;
    strcpy(newPlayer->playerName,name);
    strcpy(newPlayer->teamName, teams[foundIndex].Name);
    strcpy(newPlayer->Role,role);
    newPlayer->totalRuns=totalRuns;
    newPlayer->battingAverage=battingAverage;
    newPlayer->strikeRate=strikeRate;
    newPlayer->wickets=wickets;
    newPlayer->economyRate=economyRate;
    float batsmanformula=(newPlayer->battingAverage*newPlayer->strikeRate)/100.0f;
    if (strcmp(newPlayer->Role, "Batsman") == 0){
        newPlayer->performanceIndex=batsmanformula;
    }
    else if (strcmp(newPlayer->Role, "Bowler") == 0)
    {
        newPlayer->performanceIndex=(newPlayer->wickets*2)+(100-newPlayer->economyRate);
    }
    else if (strcmp(newPlayer->Role, "All-rounder") == 0){
        newPlayer->performanceIndex=batsmanformula+(newPlayer->wickets*2);
    }
    else {
        printf("Invalid player role\n");
        free(newPlayer);
        return;
    }
    
    newPlayer->next=teams[foundIndex].playerList;
    teams[foundIndex].playerList=newPlayer;
    teams[foundIndex].totalPlayers++;

    float battingStrikeRateTotal=0.0f;
    int battingPlayerCount=0;
    for (player *current=teams[foundIndex].playerList; current!=NULL; current=current->next){
        if (strcmp(current->Role, "Batsman") == 0 || strcmp(current->Role, "All-rounder") == 0){
            battingStrikeRateTotal+=current->strikeRate;
            battingPlayerCount++;
        }
    }
    teams[foundIndex].averageBattingStrikeRate = battingPlayerCount > 0
        ? battingStrikeRateTotal / battingPlayerCount
        : 0.0f;
}

void displayTeamPlayers(struct team *teams, int teamId){
    int foundIndex=-1;
    int low=0;
    int high=teamCount-1;
    int mid;
    while (low<=high)
    {
        mid=low+(high-low)/2;
        if (teams[mid].teamId == teamId){
            foundIndex=mid;
            break;
        }
        else if (teams[mid].teamId < teamId )
        {
            low=mid+1;
        }
        else {
            high=mid-1;
        }
    }
    if (foundIndex==-1){
        printf("Team not found\n");
        return;
    }
    
    if (teams[foundIndex].playerList==NULL){
        printf("No players in this team");
        return;
    }
    printf("\nPlayers in Team: %s\n", teams[foundIndex].Name);
    struct player *current = teams[foundIndex].playerList;
    while (current != NULL)
    {
        printf("ID %d\n",current->playerId);
        printf("Name: %s\n", current->playerName);
        printf("--------------------\n");
        current=current->next;
    }
}

void displaySortedTeams(struct team *teams){
    struct team *sortedTeam[teamCount];

    for (int i = 0; i < teamCount; i++){
        sortedTeam[i] = &teams[i];
    }

    for (int i = 0; i < teamCount - 1; i++){
        for (int j = 0; j < teamCount - i - 1; j++){

            if (sortedTeam[j]->averageBattingStrikeRate <
                sortedTeam[j + 1]->averageBattingStrikeRate){

                struct team *temp = sortedTeam[j];
                sortedTeam[j] = sortedTeam[j + 1];
                sortedTeam[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < teamCount; i++){
        printf("%s - %.2f\n",
               sortedTeam[i]->Name,
               sortedTeam[i]->averageBattingStrikeRate);
    }
}

void displayTopKPlayers(struct team *teams, int teamId, char *role, int k){
    if (k<1){
        printf("Enter correct k value");
        return;
    }
    int foundIndex=-1;
    int low=0;
    int high=teamCount-1;
    int mid;
    while (low<=high)
    {
        mid=low+(high-low)/2;
        if (teams[mid].teamId == teamId){
            foundIndex=mid;
            break;
        }
        else if (teams[mid].teamId < teamId )
        {
            low=mid+1;
        }
        else {
            high=mid-1;
        }
    }
    if (foundIndex==-1){
        printf("Team not found\n");
        return;
    }
    struct player *topPlayers[k];
    int count = 0;
    struct player *current = teams[foundIndex].playerList;
    while (current != NULL)
    {
        if (strcmp(current->Role, role) == 0)
        {
            int pos;
            if (count < k)
            {
                pos = count;
                count++;
            }
            else
            {
                if (current->performanceIndex <= topPlayers[count - 1]->performanceIndex)
                {
                    current = current->next;
                    continue;
                }
                pos = count - 1;
            }
            while (pos > 0 &&
                   topPlayers[pos - 1]->performanceIndex < current->performanceIndex)
            {
                if (pos < k)
                    topPlayers[pos] = topPlayers[pos - 1];
                pos--;
            }
            topPlayers[pos] = current;
        }
        current = current->next;
    }   
    if (count == 0)
    {
        printf("No players found for this role\n");
        return;
    }
    printf("\nTop %d players:\n", count);

    for (int i = 0; i < count; i++)
    {
        printf("Rank: %d\n", i + 1);
        printf("Player: %s\n", topPlayers[i]->playerName);
        printf("Performance Index: %.2f\n",
               topPlayers[i]->performanceIndex);
        printf("--------------------\n");
    }
}

void displayPlayersByRole(struct team *teams, char *role){
    int found=0;
    for (int i=0; i < teamCount; i++){
        printf("Team name: %s\n", teams[i].Name);
        struct player *current=teams[i].playerList;
        while (current!=NULL)
        {
            if (strcmp(current->Role,role)==0){
                printf("Player:  %s\n", current->playerName);
                printf("Performance Index: %.2f\n", current->performanceIndex);
                printf("---------\n");
                found++;
            }
            current=current->next;
        }
    }
    if (found==0) printf("No matching players found for this role\n");

}