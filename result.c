//
// Created by tommy on 17/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "voiture.h"

Voiture copyCar[NBRTOTALVOITURE];

int tri(const void *a, const void *b){
    const Voiture *p1 = (Voiture *) a;
    const Voiture *p2 = (Voiture *) b;

    if(p1->bestLap > p2->bestLap){
        return 1;
    } else if(p1->bestLap < p2->bestLap){
        return -1;
    } else {
        return 0;
    }
}

void afficheResult(Voiture *vdata){

    for (int i=0; i<NBRTOTALVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), tri);
    printf("\n\n\n");
    printf("|\tPos.\t|\tNumero\t|\tSecteur 1\t|\tSecteur 2\t|\tSecteur 3\t|\tBestLap\t\t|\tTemps en course\t|\tNombre de Tours\t\t|\tP\t|\tO\t|\n");

    for(int i=0; i<NBRTOTALVOITURE; i++){
        printf("|\t%d\t\t|%10d\t|\t%10f\t|\t%10f\t|\t%10f\t|\t%10f\t|\t%10d\t\t|\t%10d\t\t\t|\t%d\t|\t%d\t|\n",
               i+1, copyCar[i].num, (float)copyCar[i].secteur[0]/1000, (float)copyCar[i].secteur[1]/1000, (float)copyCar[i].secteur[2]/1000,(float)copyCar[i].bestLap/1000,copyCar[i].tempTotal/1000,copyCar[i].tour, copyCar[i].stand, copyCar[i].out);

    }
}


