#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "config.h"
#include "course.h"
#include "voiture.h"
#include "result.h"

int NBRVOITURE = 0;
int TEMPSCOURSE = 0;

int gettypeRace(char *argv[]);

///FONCTION MAIN
int main(int argc, char *argv[]){
    Voiture *car ;
    int pilotes[NBRTOTALVOITURE] = {44,77,11,33,3,4,5,18,14,31,16,55,10,22,7,99,9,47,6,63};  //Numéros de voitures
    int nbrTour= 50;

    if (argc != 2){
        perror("Veuillez passer 1 seul paramètre!");
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

        /*Tentative d'ajouter de sémaphores
        int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
        sem_t *semaphore = shmat(shmid_sem, NULL, 0);
        sem_init(semaphore, 1,1);
        */

        int i =0;
        pid_t pid;
        for (i ; i < NBRTOTALVOITURE; i++)
        {
            pid = fork();
            if (pid == 0){
                break; // Sinon lance 400 processus ????

            }
        }

        if (pid == -1) {
            perror("Erreur lors du fork !");
            exit(EXIT_FAILURE);
        } else if(pid == 0) {

            if (course_id == 1 || course_id == 2 || course_id == 3 || course_id == 4 || course_id == 5 || course_id == 6) {
                course(&car[i],pilotes[i],TEMPSCOURSE);
            } else if (course_id == 7) {
                //final();
            }
            exit(EXIT_SUCCESS);
        }
        else{
            for(int j=0; j<nbrTour; j++){
                afficheResult(car);
                sleep(DELAY);
            }

        }



        shmdt(car); //Détachement de la mémoire partagée
        shmctl(shmid, IPC_RMID, NULL); //Suppression de la mémoire partagée
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
        //A completer
        return 7;

    }

    else {
        return -1;
    }
}
