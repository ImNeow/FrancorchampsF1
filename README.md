# ProjetOS  
> Formule 1 projet pour l'EPHEC


## todo  
- [x] faire les secteurs, générer des temps de manière random entre 25 et 40sec par secteur
- [x] structure au sens C du terme d'une voiture
- [x] mémoire partagée
- [x] gestion des craches, arrêt au stand
- [x] le temps différent de chaque séance: P1: 1h30, P2:1h30, P3:1h
- [x] si crash arrive dans un autre secteur que le s1, les scores doivent être gardés en mémoire.
- [x] fork

- [ ] semaphore
- [ ] la qualif est une autre fonction que les essais: Q1:18min, Q2:15min, Q3:12min. La qualif élimine 5 voitures par séance, sauf Q3 qui n'élimine rien.
- [ ] Dans la fonction Qualif créer un tableau vide qui se remplit à chaque fin de session avec les 5 voitures les plus lentes éliminiées sauf en Q3 où le temps des 10 voitures tel quel est envoyé dans le tableau



## NOTE:  
En cas de crash, l'abandon est pour la séance, pas pour le week-end entier. Lors de l'abandon, la voiture reste classée
tableau de voitures, pouur pouvoir les faire tourner plusieurs tours. Au début on peut le faire de manière séquentielle, une voiture fait un toru, la deuxième fait le sien ensuite. Mettre en place le système de classement, en reprenant le meilleur s1, s2, s3 et le meilleur tour. Le meilleur s1 de la voiture peut venir d'un tour qui n'est pas son meilleur tour, même chose pour les autres secteurs.
 
