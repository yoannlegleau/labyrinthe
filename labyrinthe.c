//
// Created by adzerake on 12/11/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "labyrinthe.h"
#include "fileCoordinate.h"

int isInGrid(int x,int y);
void generatWalls(void );
int getEmptyNeighbor(int x, int y);
void setupOutput(void );
void setupPlayer(void );
void nextRound(int difficulty  );
void drowPoint(void );
void generatPath(void );
void showPath(void);
void getPaths(void);

//DEPLETED
void ensuresDifficulty(int dif);

enum cell lab[SIZEH][SIZEL];
enum cell path[SIZEH][SIZEL];
int distance[SIZEH][SIZEL];
int pathIsDisplay = 0;
int difficulty = 7;

t_coordinate *player;
t_coordinate *outputLocation;

void lab_init(void) {
    srand(time(NULL));
    file_init();
    player = malloc(sizeof(t_coordinate));
    outputLocation = malloc(sizeof(t_coordinate));
    setupPlayer();
    nextRound(difficulty);
}

void nextRound(int diff){
    const int diffTolerance = 5;
    do {
        generatWalls();
        setupOutput();
    } while (lab[player->y][player->x] != Empty);
    showPath();
    //FIXME cause de gros ralantisement au changement de round
    // - optimiser les random car il y en a 1 seul par minute

    if(!(difficulty-diffTolerance<distance[player->y][player->x] && distance[player->y][player->x]<difficulty+diffTolerance)) {
        nextRound(difficulty);
    }
    printf("rond diff:%i\n",difficulty);
    difficulty = diff;
    drowPoint();
}

void setupOutput(void ){
    int x , y;
    do {
        x = rand()%SIZEL;
        y = rand()%SIZEH;
    }while (!isInGrid(x,y) || lab[y][x] != Empty || getEmptyNeighbor(x, y) != 1);
    lab[y][x] = Output;
    outputLocation->x = x;
    outputLocation->y = y;
    generatPath();
}

void setupPlayer(void ) {
    int x, y;
    do {
        x = rand()%SIZEL;
        y = rand()%SIZEH;
    }while (!isInGrid(x,y) || lab[y][x] != Empty );
    lab[y][x] = Player;
    player->x = x;
    player->y = y;
    showPath();
}

void ensuresDifficulty(int dif){

        int max = 0;
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(distance[i][j]>dif-3){
                    max = distance[i][j];
                    break;
                }
            }
            if(max>0) break;
        }
        if(max == 0){
            generatWalls();
            setupOutput();
            setupPlayer();
        }
    }


void doPlayerMove(enum playerMove move){
    int x = player->x, y = player->y;
    lab[y][x] = Empty;
    switch (move) {
        case Up:
            if(isInGrid(x,y-1) && lab[y-1][x] != Wall) y--;
            break;
        case Down:
            if(isInGrid(x,y+1) && lab[y+1][x] != Wall) y++;
            break;
        case Left:
            if(isInGrid(x-1,y) && lab[y][x-1] != Wall) x--;
            break;
        case Right:
            if(isInGrid(x+1,y) && lab[y][x+1] != Wall) x++;

            break;
    }
    lab[y][x] = Player;
    player->x = x;
    player->y = y;
    showPath();

    if(player->y == outputLocation->y && player->x == outputLocation->x) nextRound(difficulty+5);
}

int getEmptyNeighbor(int x,int y){
    int ret = 0;
    ret += (isInGrid(x,y+1)&& lab[y+1][x] == Empty);
    ret += (isInGrid(x,y-1)&& lab[y-1][x] == Empty);
    ret += (isInGrid(x+1,y)&& lab[y][x+1] == Empty);
    ret += (isInGrid(x-1,y)&& lab[y][x-1] == Empty);
    return ret;
}

void generatWalls(void ){
    int patern[SIZEH][SIZEL];
    int k = 1;
    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            if((i-1)%2 && (j-1)%2)
                patern[i][j] = k++;
            else {
                patern[i][j] = 0;
            }
        }
    }

    int x,y,find = 0;
    while (k>1){
        while (!find){
            x = rand()%SIZEL;
            y = rand()%SIZEH;
            find = ((isInGrid(x-1,y) && isInGrid(x+1,y) && patern[y][x-1] != 0 && patern[y][x+1] != 0 && patern[y][x-1] != patern[y][x+1])
                    ||(isInGrid(x,y-1) && isInGrid(x,y+1) && patern[y-1][x] != 0 && patern[y+1][x] != 0 && patern[y-1][x] != patern[y+1][x]));
        }
        find = 0;
        int max,min;
        if(isInGrid(x-1,y) && isInGrid(x+1,y) && patern[y][x-1] != 0 && patern[y][x+1] != 0 && patern[y][x-1] != patern[y][x+1]){
            if (patern[y][x - 1] > patern[y][x + 1]) {
                max = patern[y][x - 1];
                min = patern[y][x + 1];
            } else {
                max = patern[y][x + 1];
                min = patern[y][x - 1];
            }
        }
        if(isInGrid(x,y-1) && isInGrid(x,y+1) && patern[y-1][x] != 0 && patern[y+1][x] != 0 && patern[y-1][x] != patern[y+1][x]){
            if (patern[y - 1][x] > patern[y + 1][x]) {
                max = patern[y - 1][x];
                min = patern[y + 1][x];
            } else {
                max = patern[y + 1][x];
                min = patern[y - 1][x];
            }
        }
        patern[y][x] = min;
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(patern[i][j] == max)
                    patern[i][j] = min;
            }
        }

        k=1;
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(patern[i][j] > k){
                    k = patern[i][j];
                    break;
                }
            }
            if(k > 1){
                break;
            }
        }
    }

    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            if(patern[i][j] == 1)
                lab[i][j] = Empty;
            else
                lab[i][j] = Wall;
        }
    }
}

int isInGrid(int x, int y) {
    return ((x >= 0 && x < SIZEL )&&(y >= 0 && y < SIZEH ));
}

void drowPoint(void) {
    lab[player->y][player->x] = Player;
    lab[outputLocation->y][outputLocation->x] = Output;
}

void generatPath(void) {
    t_coordinate *coordinate;
    coordinate = malloc(sizeof(t_coordinate));
    file_empty();

    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            distance[i][j] = 0;
        }
    }
    file_add(*outputLocation);
    distance[outputLocation->y][outputLocation->x]= 1;

    int value;
    while (!file_isEmpty()){
        fille_sub(coordinate);
        value = distance[coordinate->y][coordinate->x];
        int x = coordinate->x;
        int y = coordinate->y;
        if(isInGrid(x,y+1)&& lab[y+1][x] == Empty && distance[y+1][x] == 0){
            distance[y+1][x] = value+1;
            file_add(coordinateFactory(x,y+1));
        }
        if(isInGrid(x,y-1)&& lab[y-1][x] == Empty && distance[y-1][x] == 0){
            distance[y-1][x] = value+1;
            file_add(coordinateFactory(x,y-1));
        }
        if(isInGrid(x+1,y)&& lab[y][x+1] == Empty && distance[y][x+1] == 0){
            distance[y][x+1] = value+1;
            file_add(coordinateFactory(x+1,y));
        }
        if(isInGrid(x-1,y)&& lab[y][x-1] == Empty && distance[y][x-1] == 0){
            distance[y][x-1] = value+1;
            file_add(coordinateFactory(x-1,y));
        }
    }
    free(coordinate);
}

void getPaths(void ){
    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            path[i][j] = Empty;
        }
    }
    int x = player->x;
    int y = player->y;
    int value = distance[y][x];
    while (value > 1){
        if(isInGrid(x,y+1)&& lab[y+1][x] != Wall && distance[y+1][x] < value && distance[y+1][x] != 0){
            path[y+1][x] = Clue;
            y = y+1;
            value = distance[y][x];
        }
        if(isInGrid(x,y-1)&& lab[y-1][x] != Wall && distance[y-1][x] < value && distance[y-1][x] != 0){
            path[y-1][x] = Clue;
            y = y-1;
            value = distance[y][x];
        }
        if(isInGrid(x+1,y)&& lab[y][x+1] != Wall && distance[y][x+1] < value && distance[y][x+1] != 0){
            path[y][x+1] = Clue;
            x = x+1;
            value = distance[y][x];
        }
        if(isInGrid(x-1,y)&& lab[y][x-1] != Wall && distance[y][x-1] < value && distance[y][x-1] != 0){
            path[y][x-1] = Clue;
            x = x-1;
            value = distance[y][x];
        }
    }

}

void lab_serialDraw(void) {
    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            switch (lab[i][j]) {
                case Wall:
                    printf("#");
                    break;
                case Output:
                    printf("*");
                    break;
                default:
                    printf(" ");
            }
        }
        printf("\n");
    }
}

void lab_getGride(enum cell pCell[SIZEH][SIZEL]){
    for (int i = 0; i < SIZEH; i++) {
        for (int j = 0; j < SIZEL; j++) {
            pCell[i][j] = lab[i][j];
        }
    }
}

void showPath(void){
    if(pathIsDisplay){
        getPaths();
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(path[i][j] == Clue){
                    lab[i][j] = Clue;
                }
            }
        }
        drowPoint();
    }
}

void switchDisplayPath() {
    if(!pathIsDisplay){
        pathIsDisplay = 1;
        showPath();
    } else{
        for (int i = 0; i < SIZEH; i++) {
            for (int j = 0; j < SIZEL; j++) {
                if(lab[i][j] == Clue){
                    lab[i][j] = Empty;
                }
            }
        }
        pathIsDisplay = 0;
    }
}


