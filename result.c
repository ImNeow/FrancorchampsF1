
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "config.h"
#include "voiture.h"

Voiture copyCar[NBRTOTALVOITURE];


int getBestSecteur(int secteur){
    int indexOfBestCar = 0;
    int best = 0;

    for(int i=0;i<NBRTOTALVOITURE;i++){
        if(secteur==1){
            if( copyCar[i].bestSecteur[0] != 0 &&(copyCar[i].bestSecteur[0] < best || best == 0 )){
                best = copyCar[i].bestSecteur[0];
                indexOfBestCar = i;
            }
        }
        else if(secteur==2){
            if( copyCar[i].bestSecteur[1] != 0 &&(copyCar[i].bestSecteur[1] < best || best == 0)){
                best = copyCar[i].bestSecteur[1];
                indexOfBestCar = i;
            }
        }
        else if(secteur == 3){
            if(copyCar[i].bestSecteur[2] != 0 &&(copyCar[i].bestSecteur[2] < best || best == 0  )){
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
        if( copyCar[i].bestLap != 0 &&(copyCar[i].bestLap < best || best == 0)){
            best = copyCar[i].bestLap;
            index = i;
        }
    }

    return index;
}

///////////////////////////////////////////////////////////////////////////////////////////Essaie et Qualifs
int tri(const void *a, const void *b){
    const Voiture *p1 = (Voiture *) a;
    const Voiture *p2 = (Voiture *) b;

    if(p1->bestLap==0){
        return 1;
    }
    else if(p2->bestLap==0){
        return -1;
    }
    else if(p1->bestLap > p2->bestLap){
        return 1;
    } else if(p1->bestLap < p2->bestLap){
        return -1;
    } else {
        return 0;
    }
}

int getDiff(int i){
    if (i != 0 ){
        return -(copyCar[i-1].bestLap - copyCar[i].bestLap);
    }else
    {
        return 0;
    }
}


void afficheResult(Voiture *vdata,sem_t *semaphore){
    float dif;
    system("clear");
    char *stand;
    char *out;

    sem_wait(semaphore);

    for (int i=0; i<NBRTOTALVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    sem_post(semaphore);

    qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), tri);

    ///CREATION DU TABLEAU
    for(int i=0;i<108;i++){
        printf("_");
    }
    printf("\n");
    printf("| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\n",5,"Pos.",10,"Numero",10,"Secteur 1",10,"Secteur 2",10,"Secteur 3",10,"BestLap",10,"Dif",6,"Tours",3,"P",3,"O");
    printf("|_______|____________|____________|____________|____________|____________|____________|________|_____|_____|\n");


    for(int i=0; i<NBRTOTALVOITURE; i++){
        dif = getDiff(i);
        if(copyCar[i].stand==1){
            stand="P";
        }else{
            stand=" ";
        }
        if(copyCar[i].out==1){
            out="O";
        }else{
            out=" ";
        }
        if(copyCar[i].num > 0 && copyCar[i].num<100){
            printf("| %*d | %*d | %*.4f | %*.4f | %*.4f | %*.4f | %*.4f | %*d | %*s | %*s |\n",
                   5,i+1,10,copyCar[i].num,10,(float)copyCar[i].secteur[0]/1000,10, (float)copyCar[i].secteur[1]/1000,10,(float)copyCar[i].secteur[2]/1000,10,(float)copyCar[i].bestLap/1000,10,dif/1000,6,copyCar[i].tour,3, stand,3, out);
        }

    }
    printf("|_______|____________|____________|____________|____________|____________|____________|________|_____|_____|\n");


    for(int i=0;i<45;i++){
        printf("_");
    }
    printf("\n| Meilleur tour |   Voiture %d\t [%.3f]   |\n",copyCar[getBestLap()].num, (float)copyCar[getBestLap()].bestLap/1000);
    printf("|_______________|___________________________|");
    printf("\n| Best S1  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(1)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[0]/1000);
    printf("| Best S2  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(2)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[1]/1000);
    printf("| Best S3  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(3)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[2]/1000);
    printf("|_______________|___________________________|\n");

}

///////////////////////////////////////////////////////////////////////////////////////////FINAL
int triFinal(const void *a, const void *b){
    const Voiture *p1 = (Voiture *) a;
    const Voiture *p2 = (Voiture *) b;

    if(p1->status==2){
        return 1;
    }
    else if(p2->status==2){
        return -1;
    }
    else if(p1->tempTotal > p2->tempTotal && p1->status!=2){
        return 1;
    }
    else if(p1->tempTotal < p2->tempTotal && p2->status!=2){
        return -1;
    }
    else{
        return 0;
    }

}


int getDiffFinal(int i){
    if(copyCar[i].out==1){
        return 0;
    }
    else if (i != 0 ){
        return -(copyCar[i-1].tempTotal - copyCar[i].tempTotal);
    }else
    {
        return 0;
    }
}


void afficheResultFinal(Voiture *vdata,sem_t *semaphore){
    float dif;
    char *stand;
    char *out;
    system("clear");

    sem_wait(semaphore);
    for (int i=0; i<NBRTOTALVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    sem_post(semaphore);

    qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), triFinal);

    ///CREATION DU TABLEAU
    for(int i=0;i<95;i++){
        printf("_");
    }
    printf("\n");
    printf("| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\n",5,"Pos.",10,"Numero",10,"Secteur 1",10,"Secteur 2",10,"Secteur 3",10,"Dif",6,"Tours",3,"P",3,"O");
    printf("|_______|____________|____________|____________|____________|____________|________|_____|_____|\n");

    for(int i=0; i<NBRTOTALVOITURE; i++){
        dif = getDiffFinal(i);

        if(copyCar[i].stand==1){
            stand="P";
        }else{
            stand=" ";
        }
        if(copyCar[i].out==1){
            out="O";
        }else{
            out=" ";
        }

        printf("| %*d | %*d | %*.4f | %*.4f | %*.4f | %*.4f | %*d | %*s | %*s |\n",
               5,i+1,10,copyCar[i].num,10,(float)copyCar[i].secteur[0]/1000,10, (float)copyCar[i].secteur[1]/1000,10,(float)copyCar[i].secteur[2]/1000,10,dif/1000,6,copyCar[i].tour,3, stand,3, out);

    }

    printf("|_______|____________|____________|____________|____________|____________|________|_____|_____|\n");


    printf("_____________________________________________");
    printf("\n| Meilleur tour |   Voiture %d\t [%.3f]   |\n",copyCar[getBestLap()].num, (float)copyCar[getBestLap()].bestLap/1000);
    printf("|_______________|___________________________|");
    printf("\n| Best S1  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(1)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[0]/1000);
    printf("| Best S2  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(2)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[1]/1000);
    printf("| Best S3  \t|   Voiture %d\t [%.3f]   |\n", copyCar[getBestSecteur(3)].num, (float)copyCar[getBestSecteur(1)].bestSecteur[2]/1000);
    printf("|_______________|___________________________|\n");
}



///////////////////////////////////////////////////////////////////////////////////////////Saving to FILE

void saveToFile(Voiture *vdata,char *argv[],int NBRVOITURE) {
    int file;
    for (int i=0; i<NBRTOTALVOITURE;i++) {
        copyCar[i] = vdata[i];
    }

    if (strcmp(argv[1],"P1")==0) {
        file = open("result/P1",O_WRONLY);
    }else if (strcmp(argv[1],"P2")==0) {
        file = open("result/P2",O_WRONLY);
    }else if (strcmp(argv[1],"P3")==0) {
        file = open("result/P3",O_WRONLY);
    }else if (strcmp(argv[1],"Q1")==0) {
        file = open("result/Q1",O_WRONLY);
    }else if (strcmp(argv[1],"Q2")==0) {
        file = open("result/Q2",O_WRONLY);
    }else if (strcmp(argv[1],"Q3")==0){
        file = open("result/Q3",O_WRONLY);
    }else{
        file = open("result/F",O_WRONLY);
        if (file<0) {
            perror("FILE ERROR ");
            return;
        }
        char buff[1024];

        qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), triFinal);

        for(int i =0 ; i<NBRVOITURE; i++){
            sprintf(buff,"N°%d\t|\t%d\n",i+1,copyCar[i].num);
            write(file,buff, strlen(buff));
        }

        sprintf(buff,"\n Meilleur tour | Voiture %d\t [%.3f]   \n",copyCar[getBestLap()].num, (float)copyCar[getBestLap()].bestLap/1000);
        write(file, buff, strlen(buff));

        close(file);
    }

    if (file<0) {
        perror("FILE ERROR ");
        return;
    }
    char buff[1024];

    qsort(copyCar, NBRTOTALVOITURE, sizeof(Voiture), tri);

    for(int i =0 ; i<NBRVOITURE; i++){
        sprintf(buff,"%d\n",copyCar[i].num);
        write(file,buff, strlen(buff));
    }

    close(file);
}
