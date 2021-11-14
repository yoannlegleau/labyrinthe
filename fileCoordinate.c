//
// Created by adzerake on 13/11/2021.
//

#include<stdlib.h>
#include "fileCoordinate.h"

typedef struct element {t_coordinate coo; struct element* under;} t_element;
t_element* top;
t_element* queue;

t_coordinate coordinateFactory(int x, int y) {
    t_coordinate *result;
    result = malloc(sizeof(t_coordinate));
    result->x = x;
    result->y = y;
    return *result;
}

void file_init(void) {
    top = NULL;
    queue = NULL;
}

void file_add(t_coordinate v) {
    t_element* nouv;

    nouv = malloc(sizeof(t_element));
    nouv->coo = v;
    nouv->under = NULL;
    if(file_isEmpty())
        top = nouv;
    else
        queue->under = nouv;
    queue = nouv;
}

void fille_sub(t_coordinate *v) {
    t_element* premier;

    if(!file_isEmpty()){
        premier = top;
        *v = premier->coo;
        top = premier->under;
        free(premier);
    }

}

int file_isEmpty(void) {
    return top == NULL;
}

void file_empty(void) {
    t_coordinate **tmp;
    while (!file_isEmpty()){
        fille_sub(*tmp);
    }
}

int file_equals(t_coordinate *v, t_coordinate *w) {
    return (v->y == w->y && v->x == w->x);
}
