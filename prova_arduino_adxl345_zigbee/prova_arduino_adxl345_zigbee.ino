/*

prova_arduino_adxl345_v2

EPSEM curs 1718, quadrimestre de tardor, integracio de sistemes

Marc Brunet, Hafid Karbouch, Alejandro Jabalquinto, Josep Lladonosa

#avisam C and arduino IDE code to use an ADXL345 accelerometer in order to detect elder falls
and send warnings via a zigbee module.

#avi-sa'm: Codi C i d'arduino IDE per usar un acceleròmetre ADXL345 per detectar caigudes de persones grans
i enviar avisos per mitja d'un modul zigbee.

Aquesta versio detecta la caiguda i envia l'avis, via zigbee, al coordinador.

*/

#include <Wire.h> // el nostre Wire
#include <XBee.h> // xbee-zigbee library
#include <Printers.h>

// extern "C" permet incloure .h per a funcions C directament en l'arduino IDE
extern "C" {
#include "maquina.h"
}

#include <string.h>

#define ADXL_ADDRESS 0x53 // adreça de l'accelerometre
#define ADXL_DATA 0x32 // adreça dels 6 bytes XYZ 

XBee xbee = XBee(); // Create an XBee object at the top of your sketch


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

void serie_inicia(void)
{
   // Open serial communication towards zigbee module and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

}
void wire_inicia(void)
{
   Wire.begin(WIRE_DISABLE_PULLUPS); //start Wire with pullups disabled 
}


void xbee_inicia(void)
{
 xbee.setSerial(Serial); // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
}

void setup()  
{
  serie_inicia();
  wire_inicia();
  xbee_inicia();
  while(!adxl_present()) 
  {
  }
   delay(1500); // 1.5 s delay
   adxl_configura();
   init_cua();
}


void avisa_caiguda(void) // fall was detected. send it to coordinator
{
  //Serial.print() 
  // Create an array for holding the data you want to send.
  uint8_t payload[] = { 'C', 'a', 'u', '!' };

  // Specify the address of the remote XBee (this is the SH + SL)
  // mac router (18): XBeeAddress64(0x0013a200, 0x40b8f807);
  //XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x408b99f4);
  XBeeAddress64 addr64 = XBeeAddress64(0x0, 0x0);

  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload)); // Create a TX Request
  xbee.send(zbTx); // Send request
}

int valor[3]; //lectura xyz de l'adxl

void loop() // run over and over
{
  M_STATE estat; // estat obtingut de la maquina d'estats
  while (-1)
  {
     adxl_llegeix_mb(ADXL_DATA, 6, valor); //llegim 3 int de l'adxl
     estat=maquina(valor[0],valor[1],valor[2]); // crida a maquina d'estats
     if (estat == S_KO ) // caiguda!
     {
       avisa_caiguda(); // mostrem missatge caiguda
       // s.o.s, enviarem un avis VERMELL
       delay(10000); // esperem 10 segons
       init_cua(); // reiniciem algorisme a repos
     }
     delay(20); // 20 ms delay
  }
}



