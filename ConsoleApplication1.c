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
	char name[50];
	char pass[50];
	
	scanf("%s", name);
	scanf("%s", pass);


	for (int i = 0; i < *num_players; i++) {
		if (!(strcmp(players[i].name, name))) {
			printf("Existing user. Please change your username.\n");
			return;	
		}
	}

	strcpy(players[*num_players].name, name);
	strcpy(players[*num_players].pass, pass);
	players[*num_players].score = 0;

	(*num_players)++;
}

void save_players(Player players[], int num_players) {
	FILE* file = fopen("scoreboard.txt", "w");
	if (file == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	for (int i = 0; i < num_players; i++) {
		fprintf(file, "%s %s %d\n", players[i].name, players[i].pass, players[i].score);
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

int login(Player players[], int num_players) {
	char name[50];
	char pass[50];

	scanf("%s", name);
	scanf("%s", pass);

	int name_found = 0;
	for (int i = 0; i < num_players; i++) {
		if (strcmp(players[i].name, name) == 0) {
			name_found = 1;
			break;
		}
	}

	if (!name_found) {
		printf("Username not found!\n");
		return -1;
	}


	for (int i = 0; i < num_players; i++) {
		if (strcmp(players[i].name, name) == 0 && strcmp(players[i].pass, pass) == 0) {
			return i;
		}
	}

	printf("Incorrect password!\n");
	return -1;
}

void display_scoreboard(Player players[], int num_players) {
	printf("\nScoreboard:\n");
	for (int i = 0; i < num_players; i++) {
		printf("%d- %s - %d\n", i + 1, players[i].name, players[i].score);
	}
}

void myrank(Player players[], int num_players, int player_index) {
	if (player_index == -1) {
		printf("You are guest!\n");
		return;
	}
	printf("\n%d- %s - %d\n", player_index + 1, players[player_index].name, players[player_index].score);
}

void main_menu(Player players[], int num_players, int player_index) {
	char init_command[20];
	while (1) {
		scanf("%s", init_command);

		if (!(strcmp(init_command, "start"))) {
			int size;
			scanf("%d", &size);

			//start_game(players, player_index, size);
		}

		else if (!(strcmp(init_command, "scoreboard"))) {
			display_scoreboard(players, num_players);
		}

		else if (!(strcmp(init_command, "myrank"))) {
			myrank(players, num_players, player_index);
		}

		else if (!(strcmp(init_command, "back"))) {
			printf("You are now at login page.\n");
			break;
		}
		else if (!(strcmp(init_command, "exit"))) {
			printf("Thanks for playing.\n");
			exit(0);
		}

		else {
			printf("Invalid command!\n");
		}
	}
}



int main() {
	Player players[50];
	int num_players = 0;
	load_players(players, &num_players);

	char init_command[20] = "guest"; //Default value as guest
	while (1) {
		printf("Please login, signup or enter as a guest: ");
		scanf("%s", &init_command);

		if (!(strcmp(init_command, "login"))) {
			int player_index = login(players, num_players);
			if (player_index != -1) {
				printf("Welcome! Please choose what next to do:\n");
				main_menu(players, num_players, player_index);
			}
		}


		else if (!(strcmp(init_command, "signup"))) {
			add_player(players, &num_players);
			save_players(players, num_players);
			printf("Your are signed up!\n");
		}


		else if (!(strcmp(init_command, "guest"))) {
			int player_index = -1;
			printf("Welcome! Please choose what next to do:\n");
			main_menu(players, num_players, player_index);
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
