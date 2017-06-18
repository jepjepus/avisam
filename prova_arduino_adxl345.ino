#include <Wire.h> // el nostre Wire

#include <VTerm.h> // biblioteca d'us de terminal en linia serie (per fer esborra pantalla, posiciona,...
#include <VT100.h> // terminal tipus VT100
/*
 
 */
// extern "C" permet incloure .h per a funcions C directament en l'arduino IDE
extern "C" {
#include "maquina.h"
}

#include <SoftwareSerial.h>
#include <string.h>

#define ADXL_ADDRESS 0x53 // adreça de l'accelerometre
#define ADXL_DATA 0x32 // adreça dels 6 bytes XYZ 

// adxl_read_sb: reads single byte from an ADXL register
byte adxl_llegeix_sb(byte registre)
{
    byte valor;
    Wire.beginTransmission(ADXL_ADDRESS); //seleccionem dispositiu 0x53
    Wire.write(registre); //registre d'on llegirem
    Wire.endTransmission();  
    Wire.requestFrom(ADXL_ADDRESS,1);
    return Wire.read(); //retornem el byte que llegim
}    

// adxl_llegeix_mb: llegeix n bytes des d'un registre i desa a buffer d'enters
// els bytes llegits son en ordre lsb msb lsb msb lsb msb
// retorna el nombre de bytes llegits i buffer queda amb els n/2 enters llegits
byte adxl_llegeix_mb(byte registre, int n, int * buffer) 
{
    Wire.beginTransmission(ADXL_ADDRESS); //seleccionem dispositiu ADXL_ADDRESS
    Wire.write(byte(registre)); //a partir del byte que llegim
    Wire.endTransmission();
    Wire.beginTransmission(ADXL_ADDRESS);
    Wire.requestFrom(ADXL_ADDRESS, n); //llegim n bytes des del registre 
    int i=0;
    while (i<n)
    {
     byte lsb, msb;
     lsb = Wire.read();
     msb = Wire.read();
     *buffer = (msb << 8) | lsb;
     buffer++;
     i+=2;
    }
    Wire.endTransmission();
    return i;
}


void adxl_envia_sb(byte registre, byte valor) // per enviar un sl byte a un registre
{
 Wire.beginTransmission(ADXL_ADDRESS); //seleccionem dispositiu
 Wire.write(registre); //enviem registre on escriure
 Wire.write(valor); //enviem valor
 Wire.endTransmission(); 
}

boolean adxl_present()
{
  // read Device ID. It should be 11100101 = 0xe5 if present
  return (adxl_llegeix_sb(0x00) == 0xe5);
  // if present returns true, else returns false
}


void adxl_configura() //configuracio inicial adxl
{
   adxl_envia_sb(0x2D,0x08); //POWER_CTL, bit D3 (measure)
}

VTerm term(&Serial);

void serie_inicia(void)
{
   // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

}
void wire_inicia(void)
{
   Wire.begin(WIRE_DISABLE_PULLUPS); //start Wire with pullups disabled 
}

void term_inicia(void)
{
  term.init();
  term.cls();
  term.show_cursor(false);  
}

void setup()  
{
  serie_inicia();
  term_inicia();
  wire_inicia();
  term.print("Iniciat bus I2C. ");
  term.print("Esperant l'ADXL...");
  while(!adxl_present()) 
  {
  }
   term.print(" ADXL trobat. Configurant ADXL...");
   delay(1500);
   term.cls();
   adxl_configura();
   init_cua();
}

void mostra_valors(int x, int y, int valor[3])
{
     int suma;
     char buf[40];
     term.position(x,y);
     suma = abs(valor[0])+abs(valor[1])+abs(valor[2]);
     sprintf(buf,"(x,y,z):(%5d,%5d,%5d) abssum:%5d",valor[0],valor[1],valor[2], suma);
     term.print(buf);
}

int valor[3]; //lectura xyz de l'adxl

void loop() // run over and over
{
  int i=0, estat;
  //while(i<50)
  while (-1)
  {
     adxl_llegeix_mb(ADXL_DATA, 6, valor); //llegim 3 int de l'adxl
     estat=maquina(valor[0],valor[1],valor[2]); // crida a maquina d'estats
     Serial.print(estat);
     Serial.print(" ");
     Serial.println(return_sum());
//     mostra_valors(i+1,0,valor); //mostrem a apantalla els valors llegits
     delay(20);
     i++;
  }
     delay(5000);
}



