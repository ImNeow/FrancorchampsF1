

#ifndef PROJETOS_RESULT_H
#define PROJETOS_RESULT_H

void afficheResult(Voiture *v);
void afficheResultFinal(Voiture *v);
int tri(const void *a, const void *b);
int triFinal(const void *a, const void *b);
int getBestSecteur(int secteur);
int getBestLap();
int getDiff(int i);
void saveToFile(Voiture *v,char *argv[]);

#endif //PROJETOS_RESULT_H
