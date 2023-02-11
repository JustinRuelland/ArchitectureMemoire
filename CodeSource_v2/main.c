//
//  main.c
//  ArchitectureProjet
//
//  Created by Justin Ruelland on 23/01/2020.
//  Copyright © 2020 Justin Ruelland & Raphaël Malak. All rights reserved.
//


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"simulation.h"
#include"etiquette.h"
#include"convert.h"

int main(int argc, char* argv[]) {
    int nblignes;
    int d;
    printf("Attente : \n");
    scanf("%d", &d);
    printf("Fin\n");
    nblignes = lecturetotal("entre.txt");
    binary("hexa.txt", nblignes-1);
    FILE* fileB = fopen("binaire.txt", "r");
    simulation(fileB, nblignes-1);
    fclose(fileB);
    printf("Fin execution \n");
    return 0;
}


