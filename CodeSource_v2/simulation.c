//
//  simulation.c
//  ArchitectureProjet
//
//  Created by Justin Ruelland & Raphael Malak on 01/02/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//

#include "simulation.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>

//Prototype
void init_tableau(registre** tab);
void init_tabmemory(char tab[65536][9]);
int BintoInt(char* bin);
void InttoBin(int x, char* buffer);
void ConcatReg(registre* reg, char* valeur);
int ValueRegister(registre* reg);
void ChangeReg(registre* reg, int x);
void LigneToCommand(int* pvalop, int* pdest, int* psrc, int* pimm, int* psrc2, char* ligne);
int ValPC(registre** Tableau,int nblignes);
void getLigne(char* buffer, int PC, FILE* file);
void simulation(FILE* file, int nblignes);
int LoadMemory(FILE* file, int nblignes, char moze[65536][9]);

void Operation_hlt(registre** tab); // fonction de fin de programme permet de vider le tableau de registre cree

void Operation_mul(registre** tab, int* valligne, int* etat); // multiplication
void Operation_ldb(registre** tab, int* valligne, int* etat, char moze[65536][9]); // chargement sur 1 octet
void Operation_ldw(registre** tab, int* valligne, int* etat, char moze[65536][9]); // chargement sur l'octet de poids faible de la destination de l'octet en memoire pointe par les sources et chargement sur l'octet de poids fort de l'octet dans la memoire apres celui pointe par les sources
void Operation_stb(registre** tab, int* valligne, int* etat, char moze[65536][9], int MinM);
void Operation_stw(registre** tab, int* valligne, int* etat, char moze[65536][9], int MinM);
void Operation_jmp(registre** tab, int* valligne, int nblignes);
void Operation_jzs(registre** tab, int* valligne, int nblignes, int* etat);
void Operation_jzc(registre** tab, int* valligne, int nblignes, int* etat);
void Operation_jcs(registre** tab, int* valligne, int nblignes, int* etat);
void Operation_jcc(registre** tab, int* valligne, int nblignes, int* etat);
void Operation_jns(registre** tab, int* valligne, int nblignes, int* etat);
void Operation_jnc(registre** tab, int* valligne, int nblignes, int* etat);
void ClearR0(registre** tab);
//prototype malak
void Operation_out(registre** Tableau, int* tab, int* etat);
void Operation_add(registre** Tableau, int* tab, int* etat);
void Operation_sub(registre** Tableau, int* tab, int* etat);
void Operation_div(registre** Tableau, int* tab, int* etat);
void Operation_or(registre** Tableau, int* tab, int* etat);
void Operation_xor(registre** Tableau, int* tab, int* etat);
void Operation_and(registre** Tableau, int* tab, int* etat);
void Operation_in(registre** Tableau, int* tab, int* etat);
void Operation_shl(registre** tab, int* valligne, int* etat);
void Operation_shl(registre** Tableau, int* tab, int* etat);
void Operation_rnd(registre** Tableau, int* tab, int* etat);
//fin prototype malak
//malak
void ClearR0(registre** tab){
    for(int i = 0; i < 9; i++){
        tab[0]->octetpoidfort[i] = '0';
        tab[0]->octetpoidfaible[i] = '0';
    }
}

void Operation_rnd(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd un entier aléatoire entre (la valeur du registre numéro rn) et (la valeur de l'opérande src2) -1
  int x = ValueRegister(Tableau[tab[2]]), y, i; //x prend la valeur du registre numéro tab[2]
  if(tab[3] == 0){                            //si l'opérande 2 est un registre
    y = ValueRegister(Tableau[tab[4]]);
  }
  else{                                   //si l'opérande 2 est une valeur immédiate
    y = tab[4];
  }
  if(x > y){ //dans ce cas on ne peut pas appliquer la fonction rand donc on échange les valeurs entre elles
    i = x;
    x = y;
    y = i;
  }
  i = y;
    y = (rand()%(i-x)) + x;  //nombre aléatoire entre x et y-1
  etat[1] = 0; //pas de débordement possible donc bit de retenue C mis à 0
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_shl(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd la valeur du registre numéro rn décalé de la valeur de l'opérande src2 à gauche si cette valeur est positive et à droite sinon
  char bin[17], bin_dest[17];
  int x = ValueRegister(Tableau[tab[2]]), y; //x prend la valeur du registre numéro tab[2]
  if(tab[3] == 0){                            //si l'opérande 2 est un registre
    y = ValueRegister(Tableau[tab[4]]);
  }
  else{                                   //si l'opérande 2 est une valeur immédiate
    y = tab[4];
  }
  InttoBin(x, bin);
  if(y < 0){  //on décale à droite
    for(int i = 0; i < abs(y) && i < 16; i++){
      bin_dest[i] = '0'; //les y bits de poids forts sont donc à 0
    }
    for(int i = abs(y); i < 16; i++){
      bin_dest[i] = bin[i-abs(y)];
    }
    bin_dest[16] = '\0';
  }
  else{ // decalage a gauche
      for(int i = 0; i < 16 && (i+y)< 16; i++){
          bin_dest[i] = bin[i + y];
      }
      if(y > 16){
          for(int i = 0; i < 16; i++){
              bin_dest[i] = '0';
          }
      }
      else {
      for(int i = (16-y); i < 16; i++){
          bin_dest[i] = '0';
      }
      }
      bin_dest[16] = '\0';
          
  }
  y = BintoInt(bin_dest);
  if(y > 32767){
    etat[1] = 1;  //bit de retenue C mis à 1
    y = 32767;  //arrondi supérieurement
  }
  else{
    if(y < -32768){
      etat[1] = 1;
      y = -32768; //arrondi inférieurement
    }
    else{
      etat[1] = 0; //pas de débordement donc bit de retenue C mis à 0
    }
  }
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

int LoadMemory(FILE* file, int nblignes, char moze[65536][9]){
    char bufferL[37];
    int max = 0;
    int cpt = 0;
    fseek(file, 0, SEEK_SET);
    while(fgets(bufferL, 35, file)!=NULL){
        if(cpt+3 > 65533){
            printf("CompilateurAssembleur : segmentation fault trop de lignes \n");
            exit(0);
        }
        for(int i = 0; i < 8; i++){
            moze[cpt][i] = bufferL[i];
            moze[cpt+1][i] = bufferL[i+8];
            moze[cpt+2][i] = bufferL[i+16];
            moze[cpt+3][i] = bufferL[i+24];
        }
        cpt = cpt + 4;
        max = cpt;
    }
    return max;
}

void Operation_in(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd une valeur saisie au clavier par l'utilisateur
  int y;
  printf("Saisissez un entier : ");
  scanf(" %d", &y);
    printf("\n");
  if(y > 32767){
    etat[1] = 1;  //bit de retenue C mis à 1
    y = 32767;  //arrondi supérieurement
  }
  else{
    if(y < -32768){
      etat[1] = 1;
      y = -32768; //arrondi inférieurement
    }
    else{
      etat[1] = 0; //pas de débordement donc bit de retenue C mis à 0
    }
  }
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_out(registre** Tableau, int* tab, int* etat){ //affiche à l'écran la valeur registre numéro rd
  int y = ValueRegister(Tableau[tab[1]]);
  printf("La valeur du registre r%d est : %d\n", tab[1], y);
    printf("\n");
  etat[1] = 0; //pas de débordement possible car on ne modifie pas la valeur contenue dans le registre numéro rd donc bit de retenue C mis à 0
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }

}

void Operation_add(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd la valeur du registre numéro rn plus la valeur de l'opérande src2
  int x = ValueRegister(Tableau[tab[2]]), y; //x prend la valeur du registre numéro tab[2]
  if(tab[3] == 0){                            //si l'opérande 2 est un registre
    y = x + ValueRegister(Tableau[tab[4]]);
  }
  else{                                   //si l'opérande 2 est une valeur immédiate
    y = x + tab[4];
  }
  if(y > 32767){
    etat[1] = 1;  //bit de retenue C mis à 1
    y = 32767;  //arrondi supérieurement
  }
  else{
    if(y < -32768){
      etat[1] = 1;
      y = -32768; //arrondi inférieurement
    }
    else{
      etat[1] = 0; //pas de débordement donc bit de retenue C mis à 0
    }
  }
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_sub(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd la valeur du registre numéro rn moins la valeur de l'opérande src2
  int x = ValueRegister(Tableau[tab[2]]), y; //x prend la valeur du registre numéro tab[2]
  if(tab[3] == 0){                            //si l'opérande 2 est un registre
    y = x - ValueRegister(Tableau[tab[4]]);
  }
  else{                                   //si l'opérande 2 est une valeur immédiate
    y = x - tab[4];
  }
  if(y > 32767){
    etat[1] = 1;  //bit de retenue C mis à 1
    y = 32767;  //arrondi supérieurement
  }
  else{
    if(y < -32768){
      etat[1] = 1;
      y = -32768; //arrondi inférieurement
    }
    else{
      etat[1] = 0; //pas de débordement donc bit de retenue C mis à 0
    }
  }
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_div(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd le résultat de la division entière entre la valeur du registre numéro rn et la valeur de l'opérande src2
  int x = ValueRegister(Tableau[tab[2]]), y; //x prend la valeur du registre numéro tab[2]
  if(tab[3] == 0){                            //si l'opérande 2 est un registre
    y = x / ValueRegister(Tableau[tab[4]]);
  }
  else{                                   //si l'opérande 2 est une valeur immédiate
    y = x / tab[4];
  }
  etat[1] = 0; //pas de débordement possible donc bit de retenue C mis à 0
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_or(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd le résultat de l'opération (la valeur bianire du registre numéro rn OU la valeur binaire de l'opérande src2)
  char bin1[17], bin2[17], bin_dest[17];
  int x = ValueRegister(Tableau[tab[2]]), y;
  InttoBin(x, bin1);
  if(tab[3] == 0){
    y = ValueRegister(Tableau[tab[4]]);
  }
  else{
      y = tab[4];
  }
  InttoBin(y, bin2);
  for(int i = 0; i < 16; i++){
    if(bin1[i] == '1' || bin2[i] == '1'){
      bin_dest[i] = '1';
    }
    else{
      bin_dest[i] = '0';
    }
  }
  bin_dest[16] = '\0';
  y = BintoInt(bin_dest);
  etat[1] = 0; //ne peut pas y avoir de retenue
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_xor(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd le résultat de l'opération (la valeur bianire du registre numéro rn OU ET (NON ET) la valeur binaire de l'opérande src2)
  char bin1[17], bin2[17], bin_dest[17];
  int x = ValueRegister(Tableau[tab[2]]), y;
  InttoBin(x, bin1);
  if(tab[3] == 0){
    y = ValueRegister(Tableau[tab[4]]);
  }
  else{
      y = tab[4];
  }
  InttoBin(y, bin2);
  for(int i = 0; i < 16; i++){
    if((bin1[i] == '1' && bin2[i] == '0') || (bin1[i] == '0' && bin2[i] == '1')){
      bin_dest[i] = '1';
    }
    else{
      bin_dest[i] = '0';
    }
  }
  bin_dest[16] = '\0';
  y = BintoInt(bin_dest);
  etat[1] = 0; //ne peut pas y avoir de retenue
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}

void Operation_and(registre** Tableau, int* tab, int* etat){ //met dans le registre numéro rd le résultat de l'opération (la valeur bianire du registre numéro rn ET la valeur binaire de l'opérande src2)
  char bin1[17], bin2[17], bin_dest[17];
  int x = ValueRegister(Tableau[tab[2]]), y;
  InttoBin(x, bin1);
  if(tab[3] == 0){
    y = ValueRegister(Tableau[tab[4]]);
  }
    else{
        y = tab[4];
    }
  InttoBin(y, bin2);
  for(int i = 0; i < 16; i++){
    if(bin1[i] == '1' && bin2[i] == '1'){
      bin_dest[i] = '1';
    }
    else{
      bin_dest[i] = '0';
    }
  }
  bin_dest[16] = '\0';
  y = BintoInt(bin_dest);
  etat[1] = 0; //ne peut pas y avoir de retenue
  if(y < 0){     //le bit de poids fort N est 1 si le résultat est négatif, 0 sinon
    etat[2] = 1;
  }
  else{
    etat[2] = 0;
  }
  if(y == 0){ //si le résultat de l'opération est nul, le bit Z est mis à 1, 0 sinon
    etat[0] = 1;
  }
  else{
    etat[0] = 0;
  }
  ChangeReg(Tableau[tab[1]], y);
}



//fin malak


//Registre PC a l'adresse 32 du tab








void Operation_hlt(registre** tab){
    for(int i = 0; i < 34; i++){
        free(tab[i]);
    }
}


void Operation_jnc(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[2] == 0){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jns(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[2] == 1){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jcc(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[1] == 0){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jcs(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[1] == 1){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jzc(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[0] == 0){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jzs(registre** tab, int* valligne, int nblignes, int* etat){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    if(etat[0] == 1){
        x = valeurS;
        ChangeReg(tab[32], x);
    }
    else{
        x = x +4;
        ChangeReg(tab[32], x);
    }
}

void Operation_jmp(registre** tab, int* valligne, int nblignes){
    int x = ValPC(tab, nblignes);
    int valeurS;
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    x = valeurS;
    ChangeReg(tab[32], x);
}

void Operation_stw(registre** tab, int* valligne, int* etat, char moze[65536][9], int MinM){
    int valeurS, valeurRd;
       
    valeurRd = ValueRegister(tab[valligne[1]]);
       
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
       }
    else{
        valeurS = valligne[4];
       }
    int NmMemoire = valeurS + valeurRd + 32768;
    if(NmMemoire < MinM){
        printf("CompilateurAssembleur : Segmentation fault vous cherchez a modifier la memoire ou sont stockees les instructions du programme veuillez vous referez a la documentation\n");
        exit(0);
    }
    if(NmMemoire < 0 || NmMemoire > 65535){ 
        printf("CompilateurAssembleur : Segmentation fault \n");
        exit(0);
       }
    for(int i = 0; i < 9; i++){
        moze[NmMemoire][i] = tab[valligne[2]]->octetpoidfaible[i]; // recopie de l'octet de poid faible de rn dans la case memoire d'adresse rd+S
       }
    
    for(int i = 0; i < 9; i++){
        moze[NmMemoire+1][i] = tab[valligne[2]]->octetpoidfort[i]; // recopie de l'octet de poid fort de rn dans la case memoire d'adresse rd+S+1
    }
    
    etat[1] = 0; // pas de retenu possible car aucun depassement n'est possible
    int resultat = 0;
    etat[0] = 1;
    for(int i = 0; i < 8; i++){
        resultat = tab[valligne[2]]->octetpoidfaible[i]; // verification que loctet passe dans rd+S est non nul
        if(resultat != 0){
            etat[0] = 0;
        }
    }
    for(int i = 0; i < 8; i++){
           resultat = tab[valligne[2]]->octetpoidfort[i]; // verification que loctet passe dans rd+S est non nul
           if(resultat != 0){
               etat[0] = 0;
           }
       }
    
    etat[2] = tab[valligne[2]]->octetpoidfort[0]; // recopie du bit de poids fort du resultat
    
}


void Operation_stb(registre** tab, int* valligne, int* etat, char moze[65536][9], int MinM){
    int valeurS, valeurRd;
    
    valeurRd = ValueRegister(tab[valligne[1]]);
    
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
    }
    else{
        valeurS = valligne[4];
    }
    int NmMemoire = valeurS + valeurRd + 32768;
    if(NmMemoire < MinM){
        printf("CompilateurAssembleur : Segmentation fault vous cherchez a modifier la memoire ou sont stockees les instructions du programme veuillez vous referez a la documentation\n");
        exit(0);
    }
    
    if(NmMemoire < 0 || NmMemoire > 65536){
        printf("CompilateurAssembleur : Segmentation fault \n");
        exit(0);
    }
    for(int i = 0; i < 9; i++){
        moze[NmMemoire][i] = tab[valligne[2]]->octetpoidfaible[i]; // recopie de l'octet de poid faible de rn dans la case memoire d'adresse rd+S
    }
    
    etat[1] = 0; // pas de retenu possible car aucun depassement n'est possible
    int resultat = 0;
    etat[0] = 1;
    for(int i = 0; i < 8; i++){
        resultat = tab[valligne[2]]->octetpoidfaible[i]; // verification que loctet passe dans rd+S est non nul
        if(resultat != 0){
            etat[0] = 0;
        }
    }
    
    etat[2] = tab[valligne[2]]->octetpoidfaible[0]; // recopie du bit de poids fort du resultat
    
    
}

void Operation_ldw(registre** tab, int* valligne, int* etat, char moze[65536][9]){
    int valeurRn, valeurS;
    
    valeurRn = ValueRegister(tab[valligne[2]]);
    
    if(valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
    }
    else{
        valeurS = valligne[4];
    }
    int NmMemoire = valeurS + valeurRn + 32768;
    if(NmMemoire < 0 || NmMemoire > 65535){
        printf("CompilateurAssembleur : Segmentation fault \n");
        exit(0);
    }
    char valpdfaible[9];
    for(int i = 0; i < 9; i++){
        valpdfaible[i] = moze[NmMemoire][i];
    }
    char valpdfort[9];
    for(int i = 0; i < 9; i++){
        valpdfort[i] = moze[NmMemoire+1][i];
    }
    char valFinale[17];
    for(int i = 0; i < 8; i++){
        valFinale[i] = valpdfort[i];
    }
    for(int i = 8; i <= 16; i++){
        valFinale[i] = valpdfaible[i-8];
    }
    ChangeReg(tab[valligne[1]], BintoInt(valFinale));
    int resultat = BintoInt(valFinale);
    if( resultat > 32767 || resultat < -32768){
        etat[1] = 1;
    }
    else {
        etat[1] = 0;
    }
    
    if (resultat == 0){
        etat[0] = 1;
    }
    else {
        etat[0] = 0;
    }
    
    if (resultat >= 0){
        etat[2] = 0;
    }
    
    else{
        etat[2] = 1;
    }
}

void Operation_ldb(registre** tab, int* valligne, int* etat, char moze[65536][9]){
    int valeurRn, valeurS;
    
    valeurRn = ValueRegister(tab[valligne[2]]);
    
    
    if (valligne[3] == 0){ //cas ou S est un registre
        valeurS = ValueRegister(tab[valligne[4]]);
    }
    else{
        valeurS = valligne[4];
    }
    int NmMemoire = valeurS + valeurRn + 32768;
    if(NmMemoire < 0 || NmMemoire > 65536){
        printf("CompilateurAssembleur : Segmentation fault \n");
        exit(0);
    }
    
    char valpdfaible[9];
    for(int i = 0; i < 9; i++){
        valpdfaible[i] = moze[NmMemoire][i];
    }
    char valFinale[17];
    for(int i = 0; i < 8; i++){
        valFinale[i] = valpdfaible[0];
    }
    for(int i = 8; i <= 16; i++){
        valFinale[i] = valpdfaible[i-8];
    }
    ChangeReg(tab[valligne[1]], BintoInt(valFinale));
    int resultat = BintoInt(valFinale);
    if( resultat > 32767 || resultat < -32768){
        etat[1] = 1;
    }
    else {
        etat[1] = 0;
    }
    
    if (resultat == 0){
        etat[0] = 1;
    }
    else {
        etat[0] = 0;
    }
    
    if (resultat >= 0){
        etat[2] = 0;
    }
    
    else{
        etat[2] = 1;
    }
}

void Operation_mul(registre** tab, int* valligne, int* etat){
    int valSfaible;
    int valRnfaible;
    int resultat;
    char rnpfaible[9];
    char Spfaible[9];
    int numeroRegistreRn = valligne[2];
    for(int i = 0; i <= 8; i++){
        rnpfaible[i] = tab[numeroRegistreRn]->octetpoidfaible[i];
    }
    char rnf[17];
    char Sf[17];
    rnf[0] = rnpfaible[0];
    
    for(int k = 1; k < 9; k++){
        rnf[k] = '0';
    }
    for(int k = 9; k <= 16; k++){
        rnf[k] = rnpfaible[k-8];
    }
    
    if (valligne[3] == 0){ //cas ou S est un registre
        int numeroRegistreS = valligne[4];
        for(int i = 0; i <= 8; i++){
            Spfaible[i] = tab[numeroRegistreS]->octetpoidfaible[i];
        }
        Sf[0] = Spfaible[0];
        for(int k = 1; k < 9; k++){
            Sf[k] = '0';
        }
        for(int k = 9; k <= 16; k++){
            Sf[k] = Spfaible[k-8];
        }
        
        valSfaible = BintoInt(Sf);
        valRnfaible = BintoInt(rnf);
        resultat = valRnfaible * valRnfaible;
        ChangeReg(tab[valligne[1]], resultat);
        
    }
    else{ //cas ou S est une valeur immediate
        char BinaireS[17];
        InttoBin(valligne[4], BinaireS);
        for(int i = 0; i < 9; i++){
            Spfaible[i] = BinaireS[i+8];
        }
        Sf[0] = Spfaible[0];
        for(int k = 1; k < 9; k++){
            Sf[k] = '0';
        }
        for(int k = 9; k <= 16; k++){
            Sf[k] = Spfaible[k-8];
        }
        valSfaible = BintoInt(Sf);
        valRnfaible = BintoInt(rnf);
        resultat = valRnfaible * valSfaible;
        ChangeReg(tab[valligne[1]], resultat);
    }
    if( resultat > 32767 || resultat < -32768){
        etat[1] = 1;
    }
    else {
        etat[1] = 0;
    }
    
    if (resultat == 0){
        etat[0] = 1;
    }
    else {
        etat[0] = 0;
    }
    
    if (resultat >= 0){
        etat[2] = 0;
    }
    
    else{
        etat[2] = 1;
    }
}

void init_tableau(registre** tab){  //initialise le tableau de registre
  for(int i = 0; i < 34; i++){
    tab[i]=malloc(sizeof(registre));
    for(int j = 0; j < 8; j++){
      tab[i]->octetpoidfaible[j]='0';
      tab[i]->octetpoidfort[j]='0';
    }
      tab[i]->octetpoidfort[8]='\0';
      tab[i]->octetpoidfaible[8]='\0';
  }
}

void init_tabmemory(char tab[65536][9]){ //initialise le tableau de case memoire
  for(int i = 0; i < 65536; i++){
    for(int j = 0; j < 8; j++){
      tab[i][j] = '0';
    }
      tab[i][8]='\0';
  }
}

int BintoInt(char* bin){ //renvoi la valeur passe en argument binaire sous forme d'un entier
  int valeur = 0, i;
  for(i = 1; i < 16 || bin[i] != '\0'; i++){
    if(bin[i] == '1'){
      valeur = valeur + pow(2, 16-i-1);
    }
  }
  if(bin[0] == '1'){  //cela veut dire que la valeur decimale est négative (car complément à 2)
    valeur = valeur - pow(2, 15);
  }
  return valeur;
}

void InttoBin(int x, char* buffer){ //passe la valeur decimale contenu dans x dans le buffer en argument sous forme binaire
  int i, y = x;
  if(x < 0){   // Dans ce cas, on fait 2^(16)-x et on fait ensuite le même procédé que si x est pair
    buffer[0] = '1';
    y = pow(2, 15) + y;
  }
  else{
    buffer[0] = '0';
  }
  for(i = 1; i < 16; i++){ //division euclidienne par 2
    if(y >= pow(2, 16-i-1)){
      buffer[i] = '1';
      y = y - pow(2 , 16-i-1);
    }
    else{
      buffer[i] = '0';
    }
  }
  buffer[16] = '\0';
}

void ConcatReg(registre* reg, char* valeur){ // concatene le contenue d'un registre
    for(int i = 0; i < 8; i++){
        valeur[i] = reg->octetpoidfort[i];
    }
    for(int j = 8; j < 17; j++){
        valeur[j] = reg->octetpoidfaible[j-8];
    }
}

int ValueRegister(registre* reg){ //renvoi la valeur decimal contenu dans un registre
  char PC[17];
  ConcatReg(reg, PC);
  int x = BintoInt(PC);
  return x;
}

void ChangeReg(registre* reg, int x){ //met la valeur de l'entier x dans le registre
    char xBin[17];
    InttoBin(x, xBin);
    for(int i = 0; i < 8; i++){
      reg->octetpoidfort[i] = xBin[i];
    }
    for(int j = 8; j < 16; j++){
      reg->octetpoidfaible[j-8] = xBin[j];
    }
}

void LigneToCommand(int* pvalop, int* pdest, int* psrc, int* pimm, int* psrc2, char* ligne){ //divise une ligne codée en binaire en 5 parties et traduit chacune en décimale
  char op[6], rd[6], rn[6], imm[2], src2[17];
  for(int i = 0; i < 32 || ligne[i] == '\0'; i++){
    switch (i) {
      case 0 :
      case 1 :
      case 2 :
      case 3 :
      case 4 : op[i] = ligne[i]; //les 5 bits de poids fort correspondant au numéro du code opératoire
               break;

      case 5 :
      case 6 :
      case 7 :
      case 8 :
      case 9 : rd[i-5] = ligne[i]; //les bits entre 6 et 10 correspondant au numéro du registre destination
               break;

      case 10 :
      case 11 :
      case 12 :
      case 13 :
        case 14 : rn[i-10] = ligne[i]; //les bits entre 11 et 15 correspondant au numéro du premier registre source
                break;

      case 15 : imm[0] = ligne[i]; //le 16ème bits qui vaut 1 si le 2ème opérande est une valeur immédiate, ou 0 si ce dernier est un registre
                break;

      default : src2[i-16] = ligne[i]; //les 16 derniers bits correspondant à la valeur du deuxième registre source, ou à celle de la valeur immédiate en complément à 2 (dépend du bit imm)
                break;
    }
  }
    char fop[17];
    char frd[17];
    char frn[17];
    char fimm[17];
    op[5] = '\0';
     rd[5] = '\0';
     rn[5] = '\0';
     imm[1] = '\0';
     src2[16] = '\0';
    for(int i = 0; i < 11; i++){ // passage des valeurs binaires sous 16 bits en complement à 2
        fop[i]='0';
        frd[i]='0';
        frn[i]='0';
    }
    for(int j = 0; j < 6; j++ ){
        fop[j+11]=op[j];
        frd[j+11]=rd[j];
        frn[j+11]=rn[j];
    }
    for(int i = 0; i < 17; i++){
        if(i > 14){
            fimm[i]=imm[i-15];
        }
        else{
            fimm[i]='0';
        }
    }
    
  *pvalop = BintoInt(fop);
  *pdest = BintoInt(frd);
  *psrc = BintoInt(frn);
  *pimm = BintoInt(fimm);
  *psrc2 = BintoInt(src2);
}

int ValPC(registre** Tableau,int nblignes){ //renvoi la valeur PC nblignes commence a 1 et valPC vaut la valeur /4
  int valPC = ValueRegister(Tableau[32]);
  if(valPC%4 != 0 || valPC >((nblignes-1)*4)){
    printf("CompilateurAssembleur : Segmentation fault \n");
    exit(0);
  }
  return valPC;
}

void getLigne(char* buffer, int vPC, FILE* file){ // permet d'attribuer au buffer la valeur de la ligne pointer par le registre PC
  int PC = (vPC)/4;
  fseek(file, 0, SEEK_SET);
  char trash[50];
  for(int i = 0; i < PC; i++){
    fgets(trash, 49, file);
  }
  if(fgets(buffer, 34, file) == NULL){
    printf("CompilateurAssembleur : Segmentation fault \n");
    exit(0);
  }
}

void simulation(FILE* file, int nblignes){
  registre* Tableau[34];
  init_tableau(Tableau);
  char moze[65536][9];
  init_tabmemory(moze);
    int MinMemory; //cet indice signifie que toutes les cases memoires inferieures sont non modifiables car elles contiennent les lignes de commandes
    MinMemory = LoadMemory(file, nblignes, moze);
    int Etat[3];
    Etat[0] = 0; //bit d'etat Z
    Etat[1] = 0; //bit d'etat C
    Etat[2] = 0; //bit d'etat N
    
  int PC=ValPC(Tableau, nblignes); //ligne qu'il faut prendre valeur du registre PC de 0 à nblgines-1
  //Variable de lecture
  
    int valop = 0;
  int* pvalop = &valop;
  int dest = 0;
  int* pdest = &dest;
  int src = 0;
  int* psrc = &src;
  int imm = 0;
  int* pimm = &imm;
  int src2 = 0;
  int* psrc2 = &src2;
   
  char ligne[35];
  //Fin de lecture
    int ValeurLigne[5];
    
  while(PC <= nblignes*4){
      PC=ValPC(Tableau, nblignes);
      getLigne(ligne, PC, file);
      LigneToCommand(pvalop, pdest, psrc, pimm, psrc2, ligne);
      // Passage dans le tableau valeur ligne des informations prise par LigneToCommand
      for(int i = 0; i < 5; i++){
          switch(i) {
              case 0 : ValeurLigne[i] = valop;
                  break;
              case 1 : ValeurLigne[i] = dest;
                  break;
              case 2 : ValeurLigne[i] = src;
                  break;
              case 3 : ValeurLigne[i] = imm;
                  break;
              case 4 : ValeurLigne[i] = src2;
                  break;
          }
      }
      // Fin de passage de valeur
      
      //Selection de l'operation a effectue
      switch(ValeurLigne[0]){
          case 0 : Operation_add(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 1 : Operation_sub(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 2 : Operation_mul(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 3 : Operation_div(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 4 : Operation_or(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 5: Operation_xor(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 6: Operation_and(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 7: Operation_shl(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 8: Operation_ldb(Tableau, ValeurLigne, Etat, moze);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 9: Operation_ldw(Tableau, ValeurLigne, Etat, moze);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 10: Operation_stb(Tableau, ValeurLigne, Etat, moze, MinMemory);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 11: Operation_stw(Tableau, ValeurLigne, Etat, moze, MinMemory);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
            break;
              
          case 16: Operation_jmp(Tableau, ValeurLigne, nblignes);
              ClearR0(Tableau);
              break;
              
          case 17: Operation_jzs(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 18: Operation_jzc(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 19: Operation_jcs(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 20: Operation_jcc(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 21: Operation_jns(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 22: Operation_jnc(Tableau, ValeurLigne, nblignes, Etat);
              ClearR0(Tableau);
              break;
              
          case 28: Operation_in(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 29: Operation_out(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 30: Operation_rnd(Tableau, ValeurLigne, Etat);
              PC = PC + 4;
              ChangeReg(Tableau[32], PC);
              ClearR0(Tableau);
              break;
              
          case 31: PC = nblignes*4 + 3;
              //permet de sortir du while
              printf("ExecuteurAssembleur : Fin de l execution\n");
              ClearR0(Tableau);
              break;
              
            default:
              printf("Erreur argument operation ligne : %d", PC+1);
              exit(0);
              break;
      }
    
  }
    printf("Felicitations Monsieur pour cette execution reussie ! \n\n");
    Operation_hlt(Tableau); //vide le tableau de registre
}

