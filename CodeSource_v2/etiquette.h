//
//  etiquette.h
//  ArchitectureProjet
//
//  Created by Justin Ruelland on 26/01/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//

#ifndef etiquette_h
#define etiquette_h

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct etiquette{
  int val;
  char name[200];
  struct etiquette* next;
};
typedef struct etiquette etiq;

struct listeetiq{
  etiq* head;
  int len;
};
typedef struct listeetiq liste;

etiq* creeEtiq(int valeur, char* nom);
liste* creeListe(void);
void addListe(liste* list, etiq* etiquette);
void suppliste(liste* list);
int findEtiq(liste* list, char* nom); //renvoi la valeur de la ligne associee a letiquette passe en argument

void lecture2(FILE* file, int nblignes, liste* list);
void replaceEtiq(char* buffer, liste* list, int numligne);
void suppEtiq(char* buffer);
int lecture(FILE* file, liste* list);
int lecturetotal(char* namefile);
// fonction mallak()
void verif_lign_break(FILE* file, int i);
int in_list(char logique[30], char transfert[15], char sauts[27], char diverse[7], char* s, int i);
long* lecture3(char const* v3, int nblignes);
void exit_file(long* T, int i);
#endif /* etiquette_h */

