//
// Created by marc on 22/05/17.
//

#include "maquina.h"
enum states {repos = 0, lliure = 1, impacta = 2, ko = 3};
#define N 5 
enum states state = repos;
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

int maquina(long int x, long int y, long int z)
{
    long int force;
    force = x*x+y*y+z*z;
    
    force = encua(force);
    if (blok <= N)
    {
      blok++;
      return force;  
    }
    
    switch (state)
    {
        case repos:
            if (force < caiguda)
            {
                //Si estic aqui vol dir que estic caient
                state = lliure;
                return 5;
            }
            return 6;
            break;
            
        case lliure:
            //Segueixo caient
            if (force < impacte )
            {
                //He picat terra
                state = impacta;
                return 0;
            }
            return 7;
            break;
            
        case impacta:
            if(force < f_max && force > f_min)
            {
                //He pasat massa poc temps al terra, falsa alarma
                state = ko;
                return 4;
            }
            return 8;  //no ha impactat
            break;
        case ko:
            // s.o.s, enviarem un avis VERMELL
	    state = ko;
            return 3; //
            
            break;
    }
   return -1; // no detecta res

}
