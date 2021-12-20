//
// Created by tommy on 17/12/2021.
//

#include <stdlib.h>
#include "config.h"

int genererTemps(int nbrTour){
    return rand() % (MAXTIMECOURSE + 1 - MINTIMECOURSE) + MINTIMECOURSE;
}

int crachTest(){
    if(rand()% PROBACRACH == 1) return 1;
    return 0;
}

int getRandomSecteur(){
    return rand()%3+1;
}

int stand(){
    return rand() % PROBASTAND+1;
}

int randomStandTime(){
    return rand() % (MAXSTANDTIME + 1 - MINSTANDTIME) + MINSTANDTIME;
}
