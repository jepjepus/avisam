//
// Created by marc on 22/05/17.
//

#ifndef UNTITLED4_MAQUINA_H
#define UNTITLED4_MAQUINA_H

#define caiguda      96000
#define impacte      80000
#define f_max      1200000
#define f_min       700000

enum M_STATE_ {S_REPOS = 0, S_LLIURE = 1, S_IMPACTA = 2, S_KO = 3};

typedef enum M_STATE_ M_STATE;

M_STATE maquina (long int x, long int y, long int z);

void init_cua();

long int return_sum();

#endif //UNTITLED4_MAQUINA_H

