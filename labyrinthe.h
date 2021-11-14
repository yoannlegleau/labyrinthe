//
// Created by adzerake on 12/11/2021.
//

#ifndef LABYRINTHE_LABYRINTHE_H
#define LABYRINTHE_LABYRINTHE_H

#endif //LABYRINTHE_LABYRINTHE_H

enum cell {Empty,Wall,Clue,Output,Player};
enum playerMove {Up,Right,Down,Left};

#define SIZEL 31
#define SIZEH 17

void lab_init(void);
void lab_serialDraw(void);
void lab_run(void);
void lab_getGride(enum cell pCell[SIZEH][SIZEL]);
void switchDisplayPath();
void doPlayerMove(enum playerMove move);



