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

void start_game(Player players[], int player_index, int size) {

    if (size < 3) {
        printf("Size must be at least 3\n");
        return;
    }

    Player* current_player;
    Player guest_player = { "Guest", "", 0 };
    if (player_index == -1) {
        current_player = &guest_player;
    }
    else {
        current_player = &players[player_index];
    }

    int** board = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        board[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
        }
    }

    srand(time(0));
    int r1 = rand() % size;
    int c1 = rand() % size;
    int r2 = rand() % size;
    int c2 = rand() % size;
    while (r1 == r2 && c1 == c2) {
        r2 = rand() % size;
        c2 = rand() % size;
    }
    board[r1][c1] = 2;
    board[r2][c2] = 2;

    int won = 0;
    while (1) {
        printf("Current score: %d\n", current_player->score);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) {
                    printf("- ");
                }
                else {
                    printf("%d ", board[i][j]);
                }
            }
            printf("\n");
        }

        int valid_move_exists = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size - 1; j++) {
                if (board[i][j] == board[i][j + 1] || board[j][i] == board[j + 1][i]) {
                    valid_move_exists = 1;
                    break;
                }
            }
            if (valid_move_exists) break;
        }

        if (!valid_move_exists) {
            printf("You lost!\n");
            break;
        }

        char move[10];
        printf("Enter move: ");
        scanf("%s", move);

        int moved = 0;

        if (!(strcmp(move, "up"))) {
            for (int j = 0; j < size; j++) {
                for (int i = 1; i < size; i++) {
                    if (board[i][j] != 0) {
                        int k = i - 1;
                        while (k >= 0 && board[k][j] == 0) k--;
                        if (k == -1 || board[k][j] != board[i][j]) k++;
                        if (k != i) moved = 1;
                        if (k >= 0 && board[k][j] == board[i][j]) {
                            players[player_index].score += board[k][j] * 2;
                            current_player->score += board[k][j] * 2;
                        }
                        board[k][j] += board[i][j];
                        board[i][j] = 0;
                    }
                }
            }
        }
        else if (!(strcmp(move, "down"))) {
            for (int j = 0; j < size; j++) {
                for (int i = size - 2; i >= 0; i--) {
                    if (board[i][j] != 0) {
                        int k = i + 1;
                        while (k < size && board[k][j] == 0) k++;
                        if (k == size || board[k][j] != board[i][j]) k--;
                        if (k != i) moved = 1;
                        if (k < size && board[k][j] == board[i][j]) {
                            players[player_index].score += board[k][j] * 2;
                            current_player->score += board[k][j] * 2;
                        }
                        board[k][j] += board[i][j];
                        board[i][j] = 0;
                    }
                }
            }
        }
        else if (!(strcmp(move, "left"))) {
            for (int i = 0; i < size; i++) {
                for (int j = 1; j < size; j++) {
                    if (board[i][j] != 0) {
                        int k = j - 1;
                        while (k >= 0 && board[i][k] == 0) k--;
                        if (k == -1 || board[i][k] != board[i][j]) k++;
                        if (k != j) moved = 1;
                        if (k >= 0 && board[i][k] == board[i][j]) {
                            players[player_index].score += board[i][k] * 2;
                            current_player->score += board[k][j] * 2;
                        }
                        board[i][k] += board[i][j];
                        board[i][j] = 0;
                    }
                }
            }
        }
        else if (!(strcmp(move, "right"))) {
            for (int i = 0; i < size; i++) {
                for (int j = size - 2; j >= 0; j--) {
                    if (board[i][j] != 0) {
                        int k = j + 1;
                        while (k < size && board[i][k] == 0) k++;
                        if (k == size || board[i][k] != board[i][j]) k--;
                        if (k != j) moved = 1;
                        if (k < size && board[i][k] == board[i][j]) {
                            players[player_index].score += board[i][k] * 2;
                            current_player->score += board[k][j] * 2;
                        }
                        board[i][k] += board[i][j];
                        board[i][j] = 0;
                    }
                }
            }
        }

        else {
            printf("Invalid command!\n");
        }

        if (!moved) continue;

        int empty_cells = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) empty_cells++;
                if (board[i][j] == 2048 && !won) {
                    printf("You won!\n");
                    won = 1;
                }
            }
        }

        if (empty_cells == 0) {
            printf("You lost!\n");
            break;
        }

        int r = rand() % empty_cells;
        int new_random_number_generated = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == 0) {
                    if (r == 0 && !new_random_number_generated) {
                        board[i][j] = rand() % 2 ? 2 : 4;
                        new_random_number_generated = 1;
                        break;
                    }
                    r--;
                }
            }
            if (r < 0) break;
        }
    }

    // free dynamically allocated memory
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}


void main_menu(Player players[], int num_players, int player_index) {
	char init_command[20];
	while (1) {
		scanf("%s", init_command);

		if (!(strcmp(init_command, "start"))) {
			int size;
			scanf("%d", &size);

			start_game(players, player_index, size);
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
