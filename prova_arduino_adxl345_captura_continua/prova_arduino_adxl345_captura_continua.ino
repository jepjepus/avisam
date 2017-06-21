#include <Wire.h> // incloem la nostra nova versio de Wire.h

//#include <SoftwareSerial.h>
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

void serie_inicia(void)
{
   // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

}
void wire_inicia(void) // iniciem biblioteca Wire.h
{
   Wire.begin(WIRE_DISABLE_PULLUPS); //start Wire with pullups disabled 
}

void setup()  // funcio d'inicialitzacio del programa
{
  serie_inicia(); // inicialitzacio linia serie
  wire_inicia();  // inicialitzacio i2c (biblioteca Wire.h)
  while(!adxl_present()) // surt quan l'adxl345 es troba realment al bus i2c
  {
  }
   delay(1500); // pausa
   adxl_configura(); // configura l'adxl
}

int valor[3]; //lectura xyz de l'adxl

void loop() // run over and over
{
  while (-1) //mentre cert = sempre
  {
     adxl_llegeix_mb(ADXL_DATA, 6, valor); //llegim 3 int de l'adxl
     Serial.print(valor[0]);Serial.print(" ");Serial.print(valor[1]);Serial.print(" ");Serial.println(valor[2]);
     delay(20); // espera de 20 ms => 1 s / 20 ms = 50 mostres per segon
  }
}



