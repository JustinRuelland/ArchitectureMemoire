//
//  lecture.c
//  ArchitectureProjet
//
//  Created by Justin Ruelland & Raphael Malak on 15/01/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"etiquette.h"
#include<math.h>
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
// fin prototype

int lecturetotal(char* namefile){
    int nblignes = 0;
    FILE* file = fopen(namefile, "r");
    if(file == NULL){
        printf("Erreur de fichier \n");
        exit(0);
    }
    liste* list = creeListe();
    nblignes = lecture(file, list);
    fclose(file);
    file = fopen(namefile, "r");
    lecture2(file, nblignes, list);
    fclose(file);
    long* hexam = lecture3("fichierTemp.txt", nblignes);
    exit_file(hexam, nblignes);
    return nblignes;
}

// fonctions mallak()
void verif_lign_break(FILE* file, int i){ //i correspond au numéro de ligne pour dire à quelle ligne est l'erreur
  char c = fgetc(file);
  if(c == '\n' || c == '\r'){
    printf("Erreur car saut a la ligne a la ligne %d en plein instruction \n", i);
    exit(0);
  }
  fseek(file, -1, SEEK_CUR);
}

int in_list(char logique[30], char transfert[15], char sauts[27], char diverse[7], char* s, int i){
  if(strstr(logique, s) != NULL && (strlen(s) == 3 || strcmp("or", s) == 0)){
    return 0;
  }
  if(strstr(transfert, s) != NULL && strlen(s) == 3){
    return 1;
  }
  if(strstr(sauts, s) != NULL && strlen(s) == 3){
    return 2;
  }
  if(strcmp("in", s) == 0 || strcmp("out", s) == 0){
    return 3;
  }
  if(strstr(diverse, s) != NULL && strlen(s) == 3){
    return 4;
  }
  printf("Erreur ce mot n est pas reconnu dans le langage assembleur a la ligne %d\n", i+1);
  exit(0);
}

long* lecture3(char const* v3, int nblignes){
  long* T = malloc(sizeof(long) * nblignes);
  char logique[30] = "add sub mul div or xor and shl"; //arrondi la valeur immédiate si elle depasse 2^15 - 1
  char transfert[15] = "ldb ldw stb stw"; // arrondi
  char sauts[27] = "jmp jzs jzc jcs jcc jns jnc"; // arrondi si positive et erreur si négative car pas de numéro de ligne négatif
  char diverse[7] = "rnd hlt";
  int d; //pour le résultat de la fonction in_list
  int i = 0; // numéro de ligne
  int j, reg1, reg2, reg3, imm, codeopp; //numéro des registres des 2 premiers arguments, puis soit le numéro du 3eme registre ou de la valeur immédiate, imm est la valeur du bit Imm et codeopp est la valeur du code opératoire
    imm = 0;
    codeopp = 0;
  FILE* file = NULL;
  file = fopen(v3, "r");
  verif_lign_break(file, i);
  char s[10], p[10], ch[500], arg3, error_arg; //arg3 est soit un # soit un r sinon erreur
  int b = fscanf(file, "%s", s);
  while(b != EOF){
    d = in_list(logique, transfert, sauts, diverse, s, i);
    fgets(ch, 500, file);
    switch(d){
      case 0 : if(sscanf(ch, " r%d, r%d, %s %c", &reg1, &reg2, p, &error_arg) == 4){
                  printf("Trop d arguments a la ligne %d\n", i+1);
                  exit(0);
               }
               if(sscanf(ch, " r%d, r%d, %c%d", &reg1, &reg2, &arg3, &reg3) != 4){ //si ça marche pas je regarde ensuite si c'est parce que la valeur reg3 entrée par l'utilisateur est une valeur immédiate en hexadécimal
                 if(sscanf(ch, " r%d, r%d, %ch%x", &reg1, &reg2, &arg3, &reg3) != 4){ //j'ai laissé arg3 qui prend le '#' pour ne pas avoir à rajouter du code après
                   printf("Erreur a la ligne %d\n", i+1);
                   exit(0);
                 }
               }
               if((arg3 != '#' && arg3 != 'r') || reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
                 printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                 exit(0);
               }
               if(arg3 == 'r'){
                 if(reg3 < 0 || reg3 > 31){
                   printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                   exit(0);
                 }
                 imm = 0;
               }
               if(arg3 == '#'){
                 imm = 1;
                 if(reg3 < -pow(2, 15)){
                   reg3 = -pow(2, 15);
                 }
                 if(reg3 > pow(2, 15) - 1){
                   reg3 = pow(2,15) - 1;
                 }
               }
               if(strcmp("add", s) == 0){
                 codeopp = 0;
               }
               if(strcmp("sub", s) == 0){
                 codeopp = 1;
               }
               if(strcmp("mul", s) == 0){
                 codeopp = 2;
               }
               if(strcmp("div", s) == 0){
                 codeopp = 3;
               }
               if(strcmp("or", s) == 0){
                 codeopp = 4;
               }
               if(strcmp("xor", s) == 0){
                 codeopp = 5;
               }
               if(strcmp("and", s) == 0){
                 codeopp = 6;
               }
               if(strcmp("shl", s) == 0){
                 codeopp = 7;
               }
               if(imm == 0){
                 *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + reg3;
                 break;
               }
               if(imm == 1){
                 *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + pow(2, 16) + reg3; //on rajoute 2^16 qui correspond au 16eme bit mis à 1 de la valeur immédiate
                 break;
               }
               break;
      case 1 : if(strcmp("ldb", s) == 0 || strcmp("ldw", s) == 0){
                 if(sscanf(ch, " r%d, (r%d)%s %c", &reg1, &reg2, p, &error_arg) == 4){
                    printf("Trop d arguments a la ligne %d\n", i+1);
                    exit(0);
                 }
                 if(sscanf(ch, " r%d, (r%d)%c%d", &reg1, &reg2, &arg3, &reg3) != 4){ //si ça marche pas je regarde ensuite si c'est parce que la valeur reg3 entrée par l'utilisateur est une valeur immédiate en hexadécimal
                   if(sscanf(ch, " r%d, (r%d)%ch%x", &reg1, &reg2, &arg3, &reg3) != 4){ //j'ai laissé arg3 qui prend le '#' pour ne pas avoir à rajouter du code après
                     printf("Erreur a la ligne %d\n", i+1);
                     exit(0);
                   }
                 }
                 if((arg3 != '#' && arg3 != 'r') || reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
                   printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                   exit(0);
                 }
                 if(arg3 == 'r'){
                   if(reg3 < 0 || reg3 > 31){
                     printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                     exit(0);
                   }
                   imm = 0;
                 }
                 if(arg3 == '#'){
                   imm = 1;
                   if(reg3 < -pow(2, 15)){
                     reg3 = -pow(2, 15);
                   }
                   if(reg3 > pow(2, 15) - 1){
                     reg3 = pow(2,15) - 1;
                   }
                 }
                 if(strcmp("ldb", s) == 0){
                   codeopp = 8;
                 }
                 if(strcmp("ldw", s) == 0){
                   codeopp = 9;
                 }
                 if(imm == 0){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + reg3;
                   break;
                 }
                 if(imm == 1){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + pow(2, 16) + reg3; //on rajoute 2^16 qui correspond au 16eme bit mis à 1 de la valeur immédiate
                   break;
                 }
               }
               if(strcmp("stb", s) == 0 || strcmp("stw", s) == 0){
                 if(sscanf(ch, " (r%d)%s, r%d %c", &reg1, p, &reg2, &error_arg) == 4){
                    printf("Trop d arguments a la ligne %d\n", i+1);
                    exit(0);
                 }
                 if(sscanf(ch, " (r%d)%c%d, r%d", &reg1, &arg3, &reg3, &reg2) != 4){ //si ça marche pas je regarde ensuite si c'est parce que la valeur reg3 entrée par l'utilisateur est une valeur immédiate en hexadécimal
                   if(sscanf(ch, " (r%d)%ch%x, r%d", &reg1, &arg3, &reg3, &reg2) != 4){ //j'ai laissé arg3 qui prend le '#' pour ne pas avoir à rajouter du code après
                     printf("Erreur a la ligne %d\n", i+1);
                     exit(0);
                   }
                 }
                 if((arg3 != '#' && arg3 != 'r') || reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
                   printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                   exit(0);
                 }
                 if(arg3 == 'r'){
                   if(reg3 < 0 || reg3 > 31){
                     printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                     exit(0);
                   }
                   imm = 0;
                 }
                 if(arg3 == '#'){
                   imm = 1;
                   if(reg3 < -pow(2, 15)){
                     reg3 = -pow(2, 15);
                   }
                   if(reg3 > pow(2, 15) - 1){
                     reg3 = pow(2,15) - 1;
                   }
                 }
                 if(strcmp("stb", s) == 0){
                   codeopp = 10;
                 }
                 if(strcmp("stw", s) == 0){
                   codeopp = 11;
                 }
                 if(imm == 0){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + reg3;
                   break;
                 }
                 if(imm == 1){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + pow(2, 16) + reg3; //on rajoute 2^16 qui correspond au 16eme bit mis à 1 de la valeur immédiate
                   break;
                 }
               }

      case 2 : if(sscanf(ch, " %s %c", p, &error_arg) == 2){
                  printf("Trop d arguments a la ligne %d\n", i+1);
                  exit(0);
               }
               if(sscanf(ch, " %c%d", &arg3, &reg3) != 2){ //si ça marche pas je regarde ensuite si c'est parce que la valeur reg3 entrée par l'utilisateur est une valeur immédiate en hexadécimal
                 if(sscanf(ch, " %ch%x", &arg3, &reg3) != 2){ //j'ai laissé arg3 qui prend le '#' pour ne pas avoir à rajouter du code après
                   printf("Erreur a la ligne %d\n", i+1);
                   exit(0);
                 }
               }
               if(arg3 != '#' && arg3 != 'r'){
                 printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                 exit(0);
               }
               if(arg3 == 'r'){
                 if(reg3 < 0 || reg3 > 31){
                   printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                   exit(0);
                 }
                 imm = 0;
               }
               if(arg3 == '#'){
                 imm = 1;
                 if(reg3 < -pow(2, 15)){ //on arrondi inférieurement
                   reg3 = -pow(2, 15);
                 }
                 if(reg3 > pow(2, 15) - 1){ //on arrondi supérieurement
                   reg3 = pow(2,15) - 1;
                 }
               }
               if(strcmp("jmp", s) == 0){
                 codeopp = 16;
               }
               if(strcmp("jzs", s) == 0){
                 codeopp = 17;
               }
               if(strcmp("jzc", s) == 0){
                 codeopp = 18;
               }
               if(strcmp("jcs", s) == 0){
                 codeopp = 19;
               }
               if(strcmp("jcc", s) == 0){
                 codeopp = 20;
               }
               if(strcmp("jns", s) == 0){
                 codeopp = 21;
               }
               if(strcmp("jnc", s) == 0){
                 codeopp = 22;
               }
               if(imm == 0){
                 *(T+i) = (codeopp * pow(2, 27)) + reg3;
                 break;
               }
               if(imm == 1){
                 *(T+i) = (codeopp * pow(2, 27)) + pow(2, 16) + reg3; //on rajoute 2^16 qui correspond au 16eme bit mis à 1 de la valeur immédiate
                 break;
               }

      case 3 : if(sscanf(ch, " r%d %c", &reg1, &error_arg) == 2){
                  printf("Trop d arguments a la ligne ou mauvaise syntaxe ligne %d\n", i+1);
                  exit(0);
               }
               if(sscanf(ch, " r%d", &reg1) != 1){
                 printf("Erreur a la ligne %d\n", i+1);
                 exit(0);
               }
               if(strcmp("in", s) == 0){
                 codeopp = 28;
               }
               if(strcmp("out", s) == 0){
                 codeopp = 29;
               }
               *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22));
               break;

      case 4 : if(strcmp("rnd", s) == 0){
                 if(sscanf(ch, " r%d, r%d, %s %c", &reg1, &reg2, p, &error_arg) == 4){
                   printf("Trop d arguments a la ligne %d\n", i+1);
                   exit(0);
                 }
                 if(sscanf(ch, " r%d, r%d, %c%d", &reg1, &reg2, &arg3, &reg3) != 4){ //si ça marche pas je regarde ensuite si c'est parce que la valeur reg3 entrée par l'utilisateur est une valeur immédiate en hexadécimal
                   if(sscanf(ch, " r%d, r%d, %ch%x", &reg1, &reg2, &arg3, &reg3) != 4){ //j'ai laissé arg3 qui prend le '#' pour ne pas avoir à rajouter du code après
                     printf("Erreur a la ligne %d\n", i+1);
                     exit(0);
                   }
                 }
                 if((arg3 != '#' && arg3 != 'r') || reg1 > 31 || reg1 < 0 || reg2 > 31 || reg2 < 0){
                   printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                   exit(0);
                 }
                 if(arg3 == 'r'){
                   if(reg3 < 0 || reg3 > 31){
                     printf("Erreur avec les valeurs des arguments ligne %d\n", i+1);
                     exit(0);
                   }
                   imm = 0;
                 }
                 if(arg3 == '#'){
                   imm = 1;
                   if(reg3 < -pow(2, 15)){
                     reg3 = -pow(2, 15);
                   }
                   if(reg3 > pow(2, 15) - 1){
                     reg3 = pow(2,15) - 1;
                   }
                 }
                 if(imm == 0){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + reg3;
                   break;
                 }
                 if(imm == 1){
                   *(T+i) = (codeopp * pow(2, 27)) + (reg1 * pow(2, 22)) + (reg2 * pow(2, 17)) + pow(2, 16) + reg3; //on rajoute 2^16 qui correspond au 16eme bit mis à 1 de la valeur immédiate
                   break;
                 }
                 codeopp = 30;
               }
               if(strcmp("hlt", s) == 0){
                 if(sscanf(ch, " %c", &error_arg) == 1){
                   printf("Trop d arguments a la ligne ou mauvaise syntaxe ligne %d\n", i+1);
                   exit(0);
                 }
                 codeopp = 31;
                 *(T+i) = codeopp * pow(2, 27);
                 break;
               }

      default : printf("Erreur fatal [0,4]?\n"); //impossible que cela se produise hormis si l'ordinateur qui  l execute est corrompu
                exit(0);
    }
    i++;
    b = fscanf(file, "%s", s);
      j = (int)strlen(ch);
    for(int k = j; k>0; k--){ //pour libérer l'espace de ch afin de pouvoir le réutiliser dans la boucle
      ch[k] = '\0';
    }
  }
  fclose(file);
  return T;
}

void exit_file(long* T, int nblignes){
  int j;
  FILE* file = NULL;
  file = fopen("hexa.txt", "w");
  for(j=0; j < nblignes - 1; j++){
    fprintf(file, "%08lX\n", *T);
    T++;
  }
  fclose(file);
}
// fin fonction mallak()

void lecture2(FILE* file, int nblignes, liste* list){
    FILE* fOut = fopen("fichierTemp.txt", "w");
    char buffer[500];
    while(fgets(buffer, 499, file)!=NULL){
        suppEtiq(buffer);
        replaceEtiq(buffer, list, nblignes);
        fprintf(fOut, "%s",buffer);
    }
    fclose(fOut);
}


void suppEtiq(char* buffer){ // prend en argument un buffer d'une ligne et supprime l'etiquette en entête
    char buftemp[500]; // buffer temporaire de passage afin de reinitialiser plus tard le buffer pris en argument
    strcpy(buftemp, buffer); //copie du buffer dans un buffer locale temporaire afin de pouvoir copier la partie                           après l'etiquette du buffer dans lui même en évitant les mauvaises interactions
    unsigned long len = strlen(buffer);
    int pos = (-1); //variable associe a la position dans le buffer des : synonyme de declaration d'etiquette sur la ligne
    for(int i = 0; i <= len; i++){
        if(buffer[i] == ':'){ //presence d'une etiquette en entête de la ligne du buffer
            pos = i;
            break;
        }
    }
    if(pos < 0){      //aucun : n'a ete trouve sur la ligne donc aucune declaration d'etiquette
        return;
    }
    for(int k=0; k<500; k++){ // remise a zero du buffer pour eviter tout probleme d'interference avec sa valeur initiale
        buffer[k] = '\0';
    }
    int j = 0;  //indice d'ecriture dans le buffer
    pos++; //ecriture apres les deux points
    for(int i = pos; i <= len; i++){
        buffer[j] = buftemp[i]; // reecriture du buffer a partir de la suite de la declaration d'etiquette
        j++;
    }
    return;
}

void replaceEtiq(char* buffer, liste* list, int numligne){ //remplace les etiquettes par la valeur de leur adresse
    char buftemp[50];
    char buftemp2[50];
    char buftemp3[50];
    char operation[]="jmp jzs jzc jcs jcc jns jnc";
    sscanf(buffer, "%s", buftemp);
    if( strstr(operation, buftemp) == NULL){ //l operande n'est pas un saut donc pas d'etiquette possible en argument
        return;
    }
    sscanf(buffer, "%s %s", buftemp2, buftemp3);
    if(buftemp3[0] == '#' || buftemp3[0] == 'r'){ // l argument pris est deja une adresse specifique ou un registre
        return;
    }
    int x = findEtiq(list, buftemp3); //renvoi la valeur associe a l'etiquette passe en argument
    if (x<0){
        printf("Erreur etiquette incorrect passe en argument de la ligne %d \n", numligne);
        exit(0); //ici l'etiquette a pour valeur renvoye -1 ce qui correspond dans la fonction findetiq que letiquette est absente de la liste des etiquettes cree a la premiere lecture du fichier d'entree
    }
    sprintf(buffer, "%s #%d \n", buftemp2, x); //le buffer est lave des erreurs possibles presente plus loin sur la ligne et retrouve un format concret avec l'operande de saut suivi de l'adresse precede d'un indicateur de valeur immediate #
    return;
}



int lecture(FILE* file, liste* list){
  int debut;
  char etiquette[50]={'\0'};
  char buffer[550]={'\0'};
  int ligne=0;
  int compteur1;
  while(fgets(buffer, 549, file)!=NULL){

    for(int i=0;buffer[i]!='\n' && buffer[i]!='\0'; i++){
      //presence d'une etiquette
      if(buffer[i]==':'){

        if(i==0){                                            //si l'étiquette commence au debut
          printf("Erreur commentaire declaré sans nom ligne : %d \n", ligne/4);
          exit(0);
        }

        debut=0;
        while(buffer[debut]=='\t' || buffer[debut]==' ' || debut==i){
          debut++;
        }
        if(debut==i){
          printf("Erreur commentaire declare sans nom ligne : %d \n", ligne/4);
          exit(0);
        }
        compteur1=0;
        for(int k=debut; k<i; k++){
          if(buffer[k]<48 ||( buffer[k]>57 && buffer[k]<65) || (buffer[k]>90 && buffer[k]<97 )|| buffer[k]>122){
              printf("Erreur commentaire mal declare ligne : %d \n",ligne/4);
            exit(0);
          }
          etiquette[compteur1]=buffer[k];
          compteur1++;
        }
        etiq* p=creeEtiq(ligne, etiquette);
        addListe(list, p);

      }
      //On est sur la presence d'une etiquette
    }
    ligne+=4;
    for(int p=0; p<50; p++){
      etiquette[p]='\0';
    }
      for(int p=0; p<550; p++){
          buffer[p]='\0';
      }

  }
    return (ligne/4)+1;
}


