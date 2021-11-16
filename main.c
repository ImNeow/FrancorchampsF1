#include "lib.h"

///FONCTION MAIN
int main(){
    int pilotes[20] = {44,77,11,33,3,4,5,18,14,31,16,55,10,22,7,99,9,47,6,63};  //Numéros de voitures
    struct voiture * v = malloc(sizeof(Voiture)*20);	                                                    //Creation des voitures
    int voitureSize = 20;                                     //Récuperation du nombre de pilotes
    int i;

    //resetResult();//RESET DU FICHIER

    ///SHARED MEMORY
    int key;
    int shmid = shmget(key, sizeof(Voiture)*20, IPC_CREAT|0666);
    v=(Voiture*) shmat( shmid, 0, 0);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Allocation des numeros de voiture
    for(i=0;i<voitureSize;i++){
        v[i].num = pilotes[i];
    }

    lancerEssai(v);

    //Reset voiture
    printf("VOITURE    S1       S2       S3       TOUR       GAP    STAND \n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    for(i=0;i<voitureSize;i++){
        printf("%d\t|\t%d\t%d\t%d\t|\t%d\n",v[i].num,v[i].bestSecteur[0],v[i].bestSecteur[1],v[i].bestSecteur[2],v[i].bestLap);
    }
    //qsort(&v,sizeof(v)/sizeof(Voiture),sizeof(Voiture),(int (*) (const void *,const void *))&comp); // TRI
    return 0;
}

///SEANCE D'ESSAI (P1,P2,P3)
void lancerEssai(Voiture * v){
    int i;
    for(i=0;i<20;i++){

        essai(v,i,MINTIME,MAXTIME,i*getpid()*v[i].num,90);
    }
    sleep(21);
    enCourse = false;
}

void essai(Voiture *p_voiture,int index,int min, int max, int randint,int duree){
    srand(randint*time(NULL)%100);
    int nbrTour = rand()%(MAXTOURESSAI-MINTOURESSAI)+(MINTOURESSAI*duree/60);                            //Calcul de nombre de Tour
    int tableS1[nbrTour],tableS2[nbrTour],tableS3[nbrTour];    //Tableau de score
    int nbr,tourActuelle=0;
    int probaCrach = 500;
    int probaStand = 150;

    int pid = fork();
    if(pid==0){
        while(enCourse){
            sleep(1);

            if((nbr = rand()%probaCrach )== 1||nbr==2||nbr==3|| p_voiture[index].out == 1){//Crash Test
                p_voiture[index].out=1;
                tableS1[tourActuelle] = 0;
                tableS2[tourActuelle] = 0;
                tableS3[tourActuelle] = 0;
                if(nbr>1){
                    tableS1[tourActuelle] = rand()%(max-min)+min;
                }if(nbr>2){
                    tableS2[tourActuelle] = rand()%(max-min)+min;
                }
                break;
            }
            else {
                tableS1[tourActuelle] = rand()%(max-min)+min;
                tableS2[tourActuelle] = rand()%(max-min)+min;
                if((p_voiture[index].stand == 0 && rand()%100 == 1)|| rand()%probaStand == 1) {//ARRET AU STAND
                    tableS3[tourActuelle] = rand() % (max - min) + min + 30000;   //30 secondes d'arrèt au stand
                    p_voiture[index].stand = 1;
                }else{
                    p_voiture[index].stand = 0;
                    tableS3[tourActuelle] = rand() % (max - min) + min;
                }
                //Récupération et Allocation des meilleurs scores
                if (tableS1[tourActuelle] < p_voiture[index].bestSecteur[0] || p_voiture[index].bestSecteur[0]==0) {
                    p_voiture[index].bestSecteur[0] = tableS1[tourActuelle];
                }
                if (tableS2[tourActuelle] < p_voiture[index].bestSecteur[1] || p_voiture[index].bestSecteur[1]==0) {
                    p_voiture[index].bestSecteur[1] = tableS2[tourActuelle];
                }
                if (tableS3[tourActuelle] < p_voiture[index].bestSecteur[2] || p_voiture[index].bestSecteur[2]==0) {
                    p_voiture[index].bestSecteur[2] = tableS3[tourActuelle];
                }

                //Récupération du meilleur temps de tour
                if (tableS1[tourActuelle] + tableS2[tourActuelle] + tableS3[tourActuelle] < p_voiture[index].bestLap || p_voiture[index].bestLap == 0) {
                    p_voiture[index].bestLap = tableS1[tourActuelle] + tableS2[tourActuelle] + tableS3[tourActuelle];
                }
                printf("N°%d \t: \t%d\t%d\t%d \t%d\n",p_voiture[index].num,tableS1[tourActuelle],tableS2[tourActuelle],tableS3[tourActuelle],tableS1[tourActuelle]+tableS2[tourActuelle]+tableS3[tourActuelle]);
                tourActuelle++;
            }
        }
    }
}

// resetVoiture set all Voiture from the array to 0
void resetVoiture(Voiture v[],int size){
  for (int i = 0; i < size; i++) {
    v[i] = (Voiture){0};
  }
  /*
    int i,j;
    for(i=0;i<size;i++){
        for(j=0;j<3;j++){
            v[i].bestSecteur[j]=0;
        }
        v[i].bestLap = 0;
        v[i].out = 0;
        v[i].stand = 0;
    }
    */
}

// resetResult clear the result file
void resetResult(){
    fclose(fopen("result","w"));
}

///AFFICHAGE DES RESULTATS DANS UN FICHIER
void printLap(int numCar,int s1,int s2,int s3,  int out , int stand){
    if(out==1){
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\t %d\t\n"
            , numCar,(float)s1/1000,(float)s2/1000,(float)s3/1000,(float)0,out);
    }else if(stand == 1){
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\t \t %d\n"
            , numCar,(float)s1/1000,(float)s2/1000,(float)s3/1000,(float)(s1+s2+s3)/1000,stand);
    }else{
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\n"
            , numCar,(float)s1/1000,(float)s2/1000,(float)s3/1000,(float)(s1+s2+s3)/1000);
    }
}

