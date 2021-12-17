#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "voiture.h"
#include "config.h"
#include "time.h"

void course(Voiture *v,int numVoiture,int tempsCourse){

    srand(getpid()*time(NULL)%100);

    v->num = numVoiture;
    v->status = 2;
    v->tour = 0;
    v->tempTotal = 0;

    while (v->tempTotal<tempsCourse) {

        v->secteur[0] = genererTemps();
        v->secteur[1] = genererTemps();
        v->secteur[2] = genererTemps();

        //Ajout du temps total
        v->tempTotal += ((int)(v->secteur[0])/1000)+((int)(v->secteur[1])/1000)+((int)(v->secteur[2])/1000);

        v->tour=v->tour+1;

        //Récupération et Allocation des meilleurs scores
        if (v->secteur[0] < v->bestSecteur[0] || v->bestSecteur[0]==0) {
            v->bestSecteur[0] = v->secteur[0];
        }
        if (v->secteur[1] < v->bestSecteur[1] || v->bestSecteur[1]==0) {
            v->bestSecteur[1] = v->secteur[1];
        }
        if (v->secteur[2] < v->bestSecteur[2] || v->bestSecteur[2]==0) {
            v->bestSecteur[2] = v->secteur[2];
        }

        //Récupération du meilleur temps de tour
        if (v->secteur[0] + v->secteur[1] + v->secteur[2] < v->bestLap || v->bestLap == 0) {
            v->bestLap = v->secteur[0] + v->secteur[1] + v->secteur[2];
        }

        sleep(DELAY);
    }
}

