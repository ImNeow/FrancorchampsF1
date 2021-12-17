//
// Created by tommy on 17/12/2021.
//

#include <stdlib.h>
#include "config.h"

int genererTemps(){

    int randomTime= rand() % (MAXTIMECOURSE + 1 - MINTIMECOURSE) + MINTIMECOURSE;

    return randomTime;

}
