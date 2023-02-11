//
//  etiquette.c
//  ArchitectureProjet
//
//  Created by Justin Ruelland on 26/01/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//

#include "etiquette.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>

etiq* creeEtiq(int valeur, char* nom){
  etiq* p=malloc(sizeof(etiq));
  if(p == NULL){
    printf("exit of memory\n");
    exit(0);
  }
  strcpy(p->name, nom);
  p->val=valeur;
  p->next=NULL;
  return p;
}

liste* creeListe(void){
  liste* p = malloc(sizeof(liste));
  if(p == NULL){
    printf("exit of memory\n");
    exit(0);
  }
  p->head=NULL;
  p->len=0;
  return p;
}

void addListe(liste* list, etiq* etiquette){
    etiquette->next = list->head;
    list->head = etiquette;
    (list->len)++;
    return;
}

void suppliste(liste* list){
  etiq* p = list->head;
  while(list->head!=NULL){
    p = p->next;
    free(list->head);
    list->head=p;
  }
  free(list);
    return;
}

int findEtiq(liste* list, char* nom){
    etiq* p = list->head;
    for(int i = 0; i < list->len; i++){
        if(strcmp(nom, p->name) == 0){
            return p->val;
        }
        p=p->next;
    }
    return (-1);
}
