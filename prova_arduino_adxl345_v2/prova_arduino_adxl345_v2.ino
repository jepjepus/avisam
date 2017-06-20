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

void term_inicia_2n(void)
{
  term.cls();
  term.position(0,0);
  term.print("avisam - detector de caigudes - epsem enginyeria de sistemes");
  term.position(2,0);
}

void setup()  
{
  serie_inicia();
  term_inicia(); // inicia terminal per primer cop
  wire_inicia();
  term.print("Iniciat bus I2C. ");
  term.print("Esperant l'ADXL...");
  while(!adxl_present()) 
  {
  }
   term.print(" ADXL trobat. Configurant ADXL...");
   delay(1500);
   term_inicia_2n(); // inicia terminal 2n cop, amb titol, etc.
   adxl_configura();
   init_cua();
}

// mostra_valors: funcio que rep un pas entre 0 i 79 i tres valors x y z
// sortida: dibuixa valors co a punt d'una forma de grafica.
// valors 10 bts amb signe (entre -512 i 511); es passen a 0..1023 i despres proporcionalment a 0..7 per poder-los pintar.

void mostra_valors(int pas, int valor[3])
{
    term.position( 8 - (valor[0]+512)/128, pas); term.print("x"); // pinta x 
    term.position(16 - (valor[1]+512)/128, pas); term.print("y"); // pinta y
    term.position(24 - (valor[2]+512)/128, pas); term.print("z"); // pinta z
}

void mostra_caiguda(void)
{
  term.position(16,10); term.print("CAIGUDA!");
}

int valor[3]; //lectura xyz de l'adxl

void loop() // run over and over
{
  int i=0, estat;
  while (-1)
  {
     adxl_llegeix_mb(ADXL_DATA, 6, valor); //llegim 3 int de l'adxl
     estat=maquina(valor[0],valor[1],valor[2]); // crida a maquina d'estats
     if (estat == 3 ) // caiguda!
     {
       mostra_caiguda(); // mostrem missatge caiguda
       delay(10000); // esperem 10 segons
       init_cua(); // reiniciem algorisme a repos
       i=0; //comencem a 0
       term_inicia_2n();
     }
     i=(i+1) % 80; // de 0 a 79
     if (i==0) term_inicia_2n();
     mostra_valors(i,valor);
     delay(20);

  }
}



