#ifndef _PROJECT__H_
#define _PROJECT__H_

void addGame(char *name, float price);

void buyGame(char *name, int count);

void printDatabase();

void saveToFile(char *filename);

void loadFromFile(char *filename);

typedef struct{
    char *name;
    float price;
    float revenue;
} Game;

int main(void);

#endif //! _PROJECT__H_