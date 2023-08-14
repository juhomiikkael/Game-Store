#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAP 10
#define GROW_CAP 2
int num_games = 0;
int capacity = INIT_CAP;
Game* all_games = NULL;
/**
 * @brief 
 *      Adds game to database and if already added prints error message.
 *      Name doesn't have character limits.
 * 
 * 
 * @param name Is the name of the game.
 * @param price Is the price of the game
 */
void addGame(char *name, float price) {
    int notExist = 1;
    if (price <= 0) {
        printf("Game price cannot be negative or zero.\n");
        return;
    }
    for (int i = 0; i < num_games; i++) {
        if (!strcmp(name, all_games[i].name)) {
            notExist = 0;
        }
    }
    if (notExist){
        if (num_games == capacity) {
            capacity *= GROW_CAP;
            all_games = (Game*)realloc(all_games, capacity * sizeof(Game*));
        }
        Game new_game;
        new_game.name = malloc(strlen(name) * sizeof(char) + 1);
        strcpy(new_game.name, name);
        new_game.name[strlen(name)] = '\0';
        new_game.revenue = 0.00;
        new_game.price = price;
        all_games[num_games] = new_game;
        num_games++;
        printf("SUCCESS\n");
    }
    else {
        printf("Game %s is already in the database.\n", name);
    }
}
/**
 * @brief 
 *      In this method it buys the game and changes the revenue of the game in the data base.
 *      If game doesn't exist in the database it gives out error message.
 * 
 * @param name Is the name of the game tat is being bought.
 * @param count Number of games that are being bought.
 */
void buyGame(char *name, int count) {
    int found = 0;
    if (count < 1) {
        printf("Number of bought items cannot be less than 1.\n");
        return;
    }
    for (int i = 0; i < num_games; i++) {
        if (!strcmp(all_games[i].name, name)) {
            all_games[i].revenue = all_games[i].revenue + count * all_games[i].price;
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Error: Doesn't exist in the database!\n");
    }
    else {
        printf("SUCCESS\n");
    }
}
/**
 * @brief 
 *      
 *      Prints the existing database
 */
void printDatabase() {
    int i, j;
    Game temp;
    // Sorting algorithm for revenue
    for (i = 0; i < num_games - 1; i++) {
        for (j = 0; j < num_games - i - 1; j++) {
            if (all_games[j].revenue < all_games[j+1].revenue) {
                temp = all_games[j];
                all_games[j] = all_games[j+1];
                all_games[j+1] = temp;
            }
        }
    }
    for (i = 0; i < num_games; i++) {
        printf("%s %.2f %.2f\n",all_games[i].name, all_games[i].price, all_games[i].revenue);
    }
    printf("SUCCESS\n");
    
}
/**
 * @brief 
 * 
 *      Saves the existing database to a file.
 * 
 * @param filename Is the name of the file where the database is saved.
 */
void saveToFile(char *filename) {
    FILE *fp;
    int i;
    fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: could not open file\n");
        return;
    }
    for (i = 0; i < num_games; i++) {
        fprintf(fp, "%s %.2f %.2f\n", all_games[i].name, all_games[i].price, all_games[i].revenue);
    }
    printf("SUCCESS\n");
    fclose(fp);
}

/**
 * @brief 
 * 
 *      Loads database from the file.
 * 
 * @param filename Name of the file that has
 */
void loadFromFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file %s for reading.\n", filename);
        return;
    }
    // Count the number of lines in the file.
    int num_lines = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            num_lines++;
        }
    }
    rewind(fp);
    // Allocate memory for the new games.
    Game *new_games = malloc(num_lines * sizeof(Game));
    if (!new_games) {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        return;
    }
    
    // Parse each line and create a new game object.
    int i = 0;
    char line[100];
    char name[50];
    float price, revenue;
    while (fgets(line, 100, fp)) {
        sscanf(line, "%s %f %f", name, &price, &revenue);
        new_games[i].name = malloc(strlen(name) + 1);
        strcpy(new_games[i].name, name);
        new_games[i].price = price;
        new_games[i].revenue = revenue;
        i++;
    }
    // Replace the current database with the new games.
    for (i = 0; i < num_games; i++) {
        free(all_games[i].name);
    }
    free(all_games);
    all_games = new_games;
    num_games = num_lines;
    capacity = num_lines;
    fclose(fp);
    printf("SUCCESS\n");
}
void quit() {
    printf("SUCCESS\n");
    for (int i = 0; i < num_games; i++) {
        free(all_games[i].name);
    }
    free(all_games);
}
/**
 * @brief 
 * 
 *      Handles all the commands in command line 
 *      A: adds game to database. Correct command is A <name> <price>
 *      B: buys game from database. Correct command is B <name> <count>
 *      L: prints whole database. Correct command is L
 *      W: saves the database to a file. Correct command is W <filename>
 *      O: loads data from a file. Correct command is O <filename>
 *      Q: exits from the program. Correct command is Q
 * 
 * @return int 
 */

int main(void) {
    int i = 10;
    all_games = malloc(capacity * sizeof(Game));
    if (!all_games) {
        printf("Error with memory allocation\n");
        exit(i);
    }
    char input[1024]; // maximum length of input
    char cmd;
    char name[100];
    float price;
    int count;
    while (i) {
        fgets(input, 1024, stdin);
        sscanf(input, " %c", &cmd);
        if (cmd == 'A') {
            if (sscanf(input, " %c %99s %f", &cmd, name, &price) != 3) {
                printf("A should be followed by exactly 2 arguments.\n");
                continue;
            }
            addGame(name, price);
        }
        else if (cmd == 'B') {
            if (sscanf(input, " %c %99s %d", &cmd, name, &count) != 3) {
                printf("B should be followed by exactly 2 arguments.\n");
                continue;
            }
            buyGame(name, count);
        }
        else if (cmd == 'L') {
            printDatabase();
        }
        else if (cmd == 'W') {
            char filename[100];
            if (sscanf(input, " %c %99s", &cmd, filename) != 2) {
                printf("W should be followed by exactly 1 arguments.\n");
                continue;
            }
            saveToFile(filename);
        }
        else if (cmd == 'O') {
            char filename[100];
            if (sscanf(input, " %c %99s", &cmd, filename) != 2) {
                printf("O should be followed by exactly 1 arguments.\n");
                continue;
            }
            loadFromFile(filename);
        }
        else if (cmd == 'Q') {
            quit();
            break;
        }
        else {
            printf("Invalid command %c\n", cmd);
        }
    }
    return 0;
}