//
//  convert.c
//  ArchitectureProjet
//
//  Created by Justin Ruelland on 03/02/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<math.h> //pour la fonction puissance pow(a,b) qui calcule a à la puissance b
#include"etiquette.h"
#include "convert.h"

void convert(char* s);
void binary(char const* hexa, int nblignes);



void convert(char* s){ //permet de convertir une chaîne de caractères contenant une valeur hexadécimal en binaire
  int i, j;
  char buffer[33];
  strcpy(buffer, s);
  for(i = 7; i > -1; i--){ //on rempli 4 caractères à la fois en commençant par la fin
    switch (buffer[i]){ //dépend de la valeur du caractère d'indice i, correspondant à un symbole hexadécimal qu'on convertit donc en binaire sur 4 bits
      case '0' : for(j = i * 4 ; j < (i + 1) * 4; j++){
                   s[j] = '0';
                 }
                 break;
      case '1' : for(j = i * 4 ; j < (i + 1) * 4; j++){
                   s[j] = '0';
                 }
                 s[((i + 1) * 4) - 1] = '1';
                 break;

      case '2' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '0';
                 break;

      case '3' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '1';
                 break;

      case '4' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '0';
                 break;

      case '5' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '1';
                 break;

      case '6' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '0';
                 break;

      case '7' : s[(i*4)] = '0';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '1';
                 break;

      case '8' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '0';
                 break;

      case '9' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '1';
                 break;

      case 'A' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '0';
                 break;

      case 'B' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '0';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '1';
                 break;

      case 'C' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '0';
                 break;

      case 'D' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '0';
                 s[(i*4) + 3] = '1';
                 break;

      case 'E' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '0';
                 break;

      case 'F' : s[(i*4)] = '1';
                 s[(i*4) + 1] = '1';
                 s[(i*4) + 2] = '1';
                 s[(i*4) + 3] = '1';
                 break;

      default : printf("error in hexa.txt");
               exit(0);
    }
  }
}

void binary(char const* hexa, int nblignes){ //converti les valeurs hexadécimales du fichier hexa.txt en binaire et les stock dans un fichier temporaire binaire.txt
  char s[33]; //va stocker les valeurs de chaque ligne en binaire sous la forme de chaînes de caractères
  int i;
  FILE* file = NULL;
  file = fopen(hexa, "r"); //on ouvre le fichier (ici nommée hexa.txt)
  FILE* bin= NULL;
  bin = fopen("binaire.txt", "w"); //fichier temporaire où on traduit hexa.txt en binaire ligne par ligne
  for(i = 0; i < nblignes; i++){
    fscanf(file, "%s", s);
    s[32] = '\0';
    convert(s);
    fprintf(bin, "%s\n", s);
  }
  fclose(file);
  fclose(bin);
}


