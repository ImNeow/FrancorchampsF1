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

int getBestSecteur(int secteur){
    int indexOfBestCar = 0;
    int best = 0;

    for(int i=0;i<NBRTOTALVOITURE;i++){
        if(secteur==1){
            if( copyCar[i].bestSecteur[0] < best || best == 0 ){
                best = copyCar[i].bestSecteur[0];
                indexOfBestCar = i;
            }
        }
        else if(secteur==2){
            if(copyCar[i].bestSecteur[1] < best || best == 0){
                best = copyCar[i].bestSecteur[1];
                indexOfBestCar = i;
            }
        }
        else if(secteur == 3){
            if(copyCar[i].bestSecteur[2] < best || best == 0  ){
                best = copyCar[i].bestSecteur[2];
                indexOfBestCar = i;
            }
        }

    }
    return indexOfBestCar;
}

int getBestLap(){
    int index = 0;
    int best = 0;

    for(int i=0; i < NBRTOTALVOITURE; i++){
        if( copyCar[i].bestLap < best || best == 0){
            best = copyCar[i].bestLap;
            index = i;
        }
    }

    return index;
}



void afficheResult(Voiture *vdata){

    for (int i=0; i<NBRTOTALVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), tri);
    printf("\n\n\n");
    printf("|\tPos.\t|\tNumero\t|\tSecteur 1\t|\tSecteur 2\t|\tSecteur 3\t|\tBestLap\t\t|\tTemps de course\t|\tTours\t|\tP\t|\tO\t|\n");

    for(int i=0; i<NBRTOTALVOITURE; i++){
        printf("|\t%d\t\t|%10d\t|\t%10f\t|\t%10f\t|\t%10f\t|\t%10f\t|\t%10d\t\t|\t%3d\t\t|\t%d\t|\t%d\t|\n",
               i+1, copyCar[i].num, (float)copyCar[i].secteur[0]/1000, (float)copyCar[i].secteur[1]/1000, (float)copyCar[i].secteur[2]/1000,(float)copyCar[i].bestLap/1000,copyCar[i].tempTotal/1000,copyCar[i].tour, copyCar[i].stand, copyCar[i].out);

    }

    printf("\n\tBest S1: %d [%.3f]\t", copyCar[getBestSecteur(1)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[0]/1000);
    printf("Best S2: %d [%.3f]\t", copyCar[getBestSecteur(2)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[1]/1000);
    printf("Best S3: %d [%.3f]\t", copyCar[getBestSecteur(3)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[2]/1000);
    printf("Meilleur tour: %d [%.3f]\n",copyCar[getBestLap()].num, (float)copyCar[getBestLap()].bestLap/1000);

}


