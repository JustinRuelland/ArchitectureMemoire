//
//  simulation.h
//  ArchitectureProjet
//
//  Created by Justin Ruelland & Raphael Malak on 01/02/2020.
//  Copyright © 2020 Justin Ruelland. All rights reserved.
//

#ifndef simulation_h
#define simulation_h

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>

struct registre{
  char octetpoidfaible[9];
  char octetpoidfort[9];
};

typedef struct registre registre;



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
void Operation_hlt(registre** tab);
void Operation_mul(registre** tab, int* valligne, int* etat);
void Operation_ldb(registre** tab, int* valligne, int* etat, char moze[65536][9]);
void Operation_ldw(registre** tab, int* valligne, int* etat, char moze[65536][9]);
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

#endif
