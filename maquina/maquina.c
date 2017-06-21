//
// Created by marc on 22/05/17.
//

#include "maquina.h"
#define N 5 
M_STATE state = S_REPOS;
static int tiks = 0;
long int lis_force[N];
int i_cua = 0;
long int sum = 0;
int blok = 0;
void init_cua()
{
  int x = 0;
  while (x<N)
  {
    lis_force[x] = 0;
    x++;
  }
  state = S_REPOS;
  blok = 0;
  sum = 0;
  i_cua = 0;
}

static long int encua (long int val) 
{
  sum = sum + val - lis_force[i_cua];
  lis_force[i_cua] = val;
  i_cua = (i_cua + 1) % N;
  return sum;
}

long int return_sum()
{
  return sum;
}

M_STATE maquina(long int x, long int y, long int z)
{
    long int force;
    force = x*x+y*y+z*z;
    
    force = encua(force);
    if (blok <= N)
    {
      blok++;
      //return force;  
    }
    
    switch (state)
    {
        case S_REPOS:
            if (force < caiguda)
            {
                //Si estic aqui vol dir que estic caient
                state = S_LLIURE;
            }
            break;
            
        case S_LLIURE:
            //Segueixo caient
            if (force < impacte )
            {
                //He picat terra
                state = S_IMPACTA;
            }
            break;
            
        case S_IMPACTA:
            if(force < f_max && force > f_min)
            {
                //He pasat massa poc temps al terra, falsa alarma
                state = S_KO;
            }
            break;
        case S_KO:
	    state = S_KO;
            break;
    }
    return state;
}
