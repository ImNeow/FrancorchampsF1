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
        printf("%d\t|\t%d\t%d\t%d\t|\t%d\n",v[i].num,v[i].bestSector[0],v[i].bestSector[1],v[i].bestSector[2],v[i].bestLap);
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
    int nbr,tourActuel=0;
    int probaCrach = 500;
    int probaStand = 150;

    int pid = fork();
    if(pid==0){
        while(enCourse){
            sleep(1);

            if((nbr = rand()%probaCrach )== 1||nbr==2||nbr==3|| p_voiture[index].out == 1){//Crash Test
                p_voiture[index].out=1;
                tableS1[tourActuel] = 0;
                tableS2[tourActuel] = 0;
                tableS3[tourActuel] = 0;
                if(nbr>1){
                    tableS1[tourActuel] = rand()%(max-min)+min;
                }if(nbr>2){
                    tableS2[tourActuel] = rand()%(max-min)+min;
                }
                break;
            }
            else {
                tableS1[tourActuel] = rand()%(max-min)+min;
                tableS2[tourActuel] = rand()%(max-min)+min;
                if((p_voiture[index].stand == 0 && rand()%100 == 1)|| rand()%probaStand == 1) {//ARRET AU STAND
                    tableS3[tourActuel] = rand() % (max - min) + min + 30000;   //30 secondes d'arrèt au stand
                    p_voiture[index].stand = 1;
                }else{
                    p_voiture[index].stand = 0;
                    tableS3[tourActuel] = rand() % (max - min) + min;
                }
                //Récupération et Allocation des meilleurs scores
                if (tableS1[tourActuel] < p_voiture[index].bestSector[0] || p_voiture[index].bestSector[0]==0) {
                    p_voiture[index].bestSector[0] = tableS1[tourActuel];
                }
                if (tableS2[tourActuel] < p_voiture[index].bestSector[1] || p_voiture[index].bestSector[1]==0) {
                    p_voiture[index].bestSector[1] = tableS2[tourActuel];
                }
                if (tableS3[tourActuel] < p_voiture[index].bestSector[2] || p_voiture[index].bestSector[2]==0) {
                    p_voiture[index].bestSector[2] = tableS3[tourActuel];
                }

                //Récupération du meilleur temps de tour
                if (tableS1[tourActuel] + tableS2[tourActuel] + tableS3[tourActuel] < p_voiture[index].bestLap || p_voiture[index].bestLap == 0) {
                    p_voiture[index].bestLap = tableS1[tourActuel] + tableS2[tourActuel] + tableS3[tourActuel];
                }
                printf("N°%d \t: \t%d\t%d\t%d \t%d\n"
                    ,p_voiture[index].num,tableS1[tourActuel]
                    ,tableS2[tourActuel],tableS3[tourActuel]
                    ,tableS1[tourActuel]+tableS2[tourActuel]+tableS3[tourActuel]);
                tourActuel++;
            }
        }
    }
}

// resetVoiture set all Voiture from the array to 0
void resetVoiture(Voiture v[],int size){
  for (int i = 0; i < size; i++) {
    v[i] = (Voiture){0};
  }
}

// resetResult clear the result file
void resetResult(){
    fclose(fopen("result","w"));
}

///AFFICHAGE DES RESULTATS DANS UN FICHIER
void printLap(int numCar,int s1,int s2,int s3,  int out , int stand){
    if(out==1)
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\t %d\t\n"
            , numCar,(float)s1/1000
            ,(float)s2/1000,(float)s3/1000
            ,(float)0,out);
    else if(stand == 1)
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\t \t %d\n"
            , numCar,(float)s1/1000
            ,(float)s2/1000
            ,(float)s3/1000,(float)(s1+s2+s3)/1000
            ,stand);
    else
        printf("%d\t|\t %.4f\t %.4f\t %.4f\t %.4f\n"
            , numCar,(float)s1/1000
            ,(float)s2/1000
            ,(float)s3/1000
            ,(float)(s1+s2+s3)/1000);
}

