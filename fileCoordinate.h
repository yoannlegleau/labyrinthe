//
// Created by adzerake on 13/11/2021.
//

#ifndef LABYRINTHE_FILECOORDINATE_H
#define LABYRINTHE_FILECOORDINATE_H

#endif //LABYRINTHE_FILECOORDINATE_H

typedef struct coordinate{int x;int y;} t_coordinate;


t_coordinate coordinateFactory(int x,int y);
void file_init(void);
void file_add(t_coordinate v);
void fille_sub(t_coordinate* v);
int file_isEmpty(void);
void file_empty(void );
int file_equals(t_coordinate *v, t_coordinate *w);
