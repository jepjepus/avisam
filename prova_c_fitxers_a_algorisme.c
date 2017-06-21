#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <unistd.h> //for usleep()

// a diferencia d'un programa principal arduino IDE no ens cal indicar
// extern "C" per aixo es comenta la clausula deixant nomes include:
// extern "C" {
#include "maquina/maquina.h"
//}

FILE * fitxer; // fitxer amb els valors x y z

FILE * obre_fitxer(char * filename) //obrim fitxer per llegir x y z
{
  FILE * f;
  f = fopen(filename,"r");
  return f;
}

// llegeix_xyz_fitxer: donat fitxer f i buffer buf, llegeix valors x y z
// del fitxer via fscanf. Retorna el nombre de valors llegits
// en cas d'exit, ha de retornar 3.
//
int llegeix_xyz_fitxer(FILE * f, int * buf)
{
  int n;
  n=fscanf(f,"%d %d %d\n", buf, buf+1, buf+2);
  return n;
}			 

// tanca_fitxer: donat el fitxer f, el tanca.
//retorna zero si s'ha tancat bé; altrament retorna EOF.
//
int tanca_fitxer(FILE * f)
{  
  return fclose(f);
}

// delay: fa un retard de mseconds miliseconds
// a diferencia de l'arduino IDE, al C s'usa usleep() i compta en microsegons
//
void delay(useconds_t mseconds)
{
  usleep(mseconds*1000);
}

// adxl_llegeix_mb: en aquesta prova llegeix xyz del fitxer
// el paràmetre és 6 ja que s'especifica el nombre de bytes
// aquí usem però 3 com a valor fix (3 enters = x y z)
// pel que no caldria el paràmetre n
// però el mantenim per variar el mínim respecte el programa de l'arduino
// retorna 3, el nombre de valors llegits.
int adxl_llegeix_mb( int n, int * buffer) 
{
  int llegits,buf[3];
  llegits=llegeix_xyz_fitxer(fitxer, buf); // llegim x y z del fitxer
  if (llegits==3) //hem llegit 3; altrament EOF (suposem)
    {
     int i=0;
    while (i<3)
    {
      *buffer = buf[i++];
      buffer++;
    }
    } 
  return llegits; //retornem 3 o EOF
}

int valor[3]; //lectura xyz

// bucle que va llegint valors del fitxer i els va passant a l'algorisme de
// deteccio de caiguda (maquina.c)
// acaba quan detecta EOF en el fitxer
//
void loop(void)
{
 int n=0;
 int i=0;
 M_STATE estat; //estat retornat per l'algorisme maquina
  while (n!=EOF)
  {
    n=adxl_llegeix_mb(6, valor); // llegim xyz del fitxer
     if (n==3)
       {
        estat=maquina(valor[0],valor[1],valor[2]); // crida a maquina d'estats
        printf("%d > %ld\n",estat,return_sum());
        delay(20); // 20 ms
        i++;
       }
  }
  tanca_fitxer(fitxer);
  printf("Fitxer tancat. Adéu");
    
}


// main: programa principal. Rep parametres de linia d'ordres
// sense parametres (o mes d'un parametre), mostra text d'ajuda
// amb un sol parametre (nom de fitxer), inicia algorisme sobre els valors
// x y z que va llegint en el fitxer
//
int main(int argc, char * argv[])
{
  switch(argc)
  {
    case 2: // entren un paràmetre a la línia d'ordres
         fitxer = obre_fitxer(argv[1]); //obrim fitxer amb dades per llegir
         init_cua();
         loop();
         break;
   default: // no entren paràmetre (o més d'un) a la línia d'ordres
      printf("\n\n"
	     "Anàlisi algorisme detecció caiguda.\n"
	     "Llegeix dades des d'un fitxer de mostres.\n"
	     "Indica si detecta caiguda o no.\n\n"
	     "Utilització:\n\n"
	     "%s fitxer_mostres\n\n",argv[0]);
  }
  return 0;
}
