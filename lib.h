#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <semaphore.h>

// SETTINGS
int MINTIME = 25000;    //millisecondes
int MAXTIME = 40000;    //millisecondes
int MINTOURESSAI = 40;  //nombre de tour min    P1 & P2: 60-66
int MAXTOURESSAI = 47;  //nombre de tour max    P1 & P2 : 60-66
//int NBRTOURFINAL = 44;  //nombre de tour de la final(300-350)
bool enCourse = true;

// VOITURE struct
typedef struct voiture{
    int num;                //Numero de voiture
    int bestSector[3];     //Best temps pour chaque secteur (n° = index+1)
    int bestLap;            //Best Lap
    int stand;              //Arret au stand
    int out;                //Crash
}Voiture;

// function prototypes
void essai(Voiture *,int ,int , int , int ,int );
void lancerEssai(Voiture *);
void printLap(int ,int ,int , int , int , int );
void qualif();
void course();
void resetVoiture(Voiture [],int );
int comp(const Voiture *,const Voiture *);
void resetResult();
