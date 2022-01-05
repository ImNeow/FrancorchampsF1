#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include "config.h"
#include "course.h"
#include "voiture.h"
#include "result.h"
#include "final.h"

int NBRVOITURE = 0;
int TEMPSCOURSE = 0;
int pilotes[NBRTOTALVOITURE];  //Numéros de voitures


int gettypeRace(char *argv[]);
void trierVoiture(Voiture *vdata,char *argv[]);

int main(int argc, char *argv[]){
    Voiture *car ;

    if (argc != 2){
        printf("Veuillez passer 1 seul paramètre!\n");
        exit(EXIT_FAILURE);
    }

    int course_id = gettypeRace(argv);

    if (course_id == -1){
        printf("Erreur, parametre invalide");
        exit(EXIT_FAILURE);
    }else{

        //Création de la mémoire partagée
        int shmid = shmget(IPC_PRIVATE, sizeof(Voiture) * NBRTOTALVOITURE, 0666 | IPC_CREAT);
        car = shmat(shmid, NULL, 0);

        //sémaphore
        int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
        sem_t *semaphore = shmat(shmid_sem, NULL, 0);
        sem_init(semaphore, 1,1);


        int i =0;
        pid_t pid;
        trierVoiture(car,argv);
        for (i ; i < NBRVOITURE; i++)
        {
            pid = fork();
            if (pid == 0) break;
        }

        if (pid == -1) {
            perror("Erreur lors du fork !");
            exit(EXIT_FAILURE);
        } else if(pid == 0) {

            if (course_id == 1 || course_id == 2 || course_id == 3 || course_id == 4 || course_id == 5 || course_id == 6) {
                course(&car[i],pilotes[i],TEMPSCOURSE);
            } else {
                final(&car[i],pilotes[i],semaphore);
            }
            exit(EXIT_SUCCESS);
        }
        else{
            bool courseFinished = false;
            int compt = 0;
            if(course_id<7){
                while(!courseFinished){
                    afficheResult(car,semaphore);
                    compt=0;
                    for(int j=0;j<NBRVOITURE;j++){
                        if(car[j].tempTotal>TEMPSCOURSE*1000 || car[j].status == 2){
                            compt++;
                        }
                        if(compt==NBRVOITURE-1){
                            courseFinished=true;
                        }
                    }
                    sleep(DELAY);
                }
                saveToFile(car,argv,NBRVOITURE);
            } else{
                while(!courseFinished){
                    afficheResultFinal(car,semaphore);
                    compt=0;
                    for(int j=0;j<NBRVOITURE;j++){
                        if(car[j].tour==FINALTOURS || car[j].status == 2){
                            compt++;
                        }
                        if(compt==NBRVOITURE-1){
                            courseFinished=true;
                            saveToFile(car,argv,NBRVOITURE);
                        }
                    }
                    sleep(DELAY);
                }
            }

        }



        shmdt(car); //Détachement de la mémoire partagée
        shmctl(shmid, IPC_RMID, NULL); //Suppression de la mémoire partagée

        sem_destroy(semaphore);
        return 0;
    }

}

int gettypeRace(char *argv[]) {

    if(strcmp(argv[1], "P1" ) == 0){
        NBRVOITURE = 20;
        TEMPSCOURSE = CHRONO_P1;
        return 1;

    }
    else if(strcmp(argv[1], "P2") == 0) {
        NBRVOITURE = 20;
        TEMPSCOURSE = CHRONO_P2;
        return 2;

    }
    else if(strcmp(argv[1], "P3") == 0) {
        NBRVOITURE = 20;
        TEMPSCOURSE = CHRONO_P3;
        return 3;

    }
    else if(strcmp(argv[1], "Q1") == 0) {
        NBRVOITURE = 20;
        TEMPSCOURSE = CHRONO_Q1;
        return 4;

    }
    else if(strcmp(argv[1], "Q2") == 0) {
        NBRVOITURE = 15;
        TEMPSCOURSE = CHRONO_Q2;
        return 5;

    }
    else if(strcmp(argv[1], "Q3")  == 0) {
        NBRVOITURE = 10;
        TEMPSCOURSE = CHRONO_Q3;
        return 6;

    }
    else if(strcmp(argv[1], "F")  == 0) {
        NBRVOITURE = 20;
        return 7;

    }
    return -1;
}

void trierVoiture(Voiture *vdata,char *argv[]){

    int file,pos[20];
    char *str = malloc(sizeof(char)*200);
    char *token;
    int i = 0;

    int pilote[NBRTOTALVOITURE] = {44,77,11,33,3,4,5,18,14,31,16,55,10,22,7,99,9,47,6,63};  //Numéros de voitures



    if(strcmp(argv[1], "Q2" ) == 0 || strcmp(argv[1], "Q3" ) == 0 ){

        if(strcmp(argv[1], "Q2" ) == 0){
            file = open("result/Q1",O_RDONLY);
        }
        else if(strcmp(argv[1], "Q3" ) == 0){
            file = open("result/Q2",O_RDONLY);
        } else{
            file = open("result/Q3",O_RDONLY);
        }
        read(file,str,sizeof(str)*256);
        close(file);

        token = strtok(str, "\n");

        while( token != NULL ) {
            pos[i] = atoi(token);
            token = strtok(NULL, "\n");
            i++;
        }

        for(int k=0;k<NBRVOITURE;k++){
            pilotes[k] = pos[k];
        }
    }else{
        for(int j=0;j<NBRVOITURE;j++){
            pilotes[j] = pilote[j];
        }
    }


}