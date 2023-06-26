#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
	char name[50];
	char pass[50];
	int score;
}Player;

void add_player(Player players[], int* num_players) {
	scanf("%s %s", players[*num_players].name, players[*num_players].pass);
	(*num_players)++;
}

void save_players(Player players[], int num_players) {
	FILE* file = fopen("scoreboard.txt", "w");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	for (int i = 0; i < num_players; i++) {
		fprintf(file, "%s %s\n", players[i].name, players[i].pass);
	}
	fclose(file);
}

void load_players(Player players[], int* num_players) {
	FILE* file = fopen("scoreboard.txt", "r");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(0);
	}
	while (fscanf(file, "%s %s %d\n", players[*num_players].name, players[*num_players].pass,
		& players[*num_players].score) != EOF) {
		(*num_players)++;
	}
	fclose(file);
}

int main() {
	Player players[50];
	int num_players = 0;
	load_players(players, &num_players);

	char init_command[10] = "guest"; //Default value as guest
	while (1) {

		scanf("%s", &init_command);

		if (!(strcmp(init_command, "login"))) {

			printf("It is login.\n");
		}


		else if (!(strcmp(init_command, "signup"))) {
			add_player(players, &num_players);
			save_players(players, num_players);
			printf("It is signup.\n");
		}


		else if (!(strcmp(init_command, "guest"))) {
			//Start
			printf("Start!\n");
		}
		
		else if (!(strcmp(init_command, "exit"))) {
			printf("Thanks for playing.\n");
			exit(0);
		}

		else {
			printf("invalid command\n");
		}
	}
}