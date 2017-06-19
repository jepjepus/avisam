EPSEM curs 1617, quadrimestre de primavera, enginyeria de sistemes

Marc Brunet
Hafid Karbouch
Alejandro Jabalquinto
Josep Lladonosa

#avisam
C and arduino IDE code to use an ADXL345 accelerometer in order to detect elder falls.

#avi-sa'm:
Codi C i d'arduino IDE per usar un acceleròmetre ADXL345 per detectar caigudes de persones grans.

Hi ha 3 programes (2 d'entorn arduino IDE, els .ino i 1 d'entorn C):

./prova_arduino_adxl345_captura_continua/prova_arduino_adxl345_captura_continua.ino: permet captura contínua valors x y z en el terminal.
./prova_arduino_adxl345.ino: connectat a terminal va mostrant estats de la màquina d'estats que inclou el cas de caiguda.
./prova_c_fitxers_a_algorisme.c: programa que permet provar l'algorisme de detecció de caiguda tot llegint els valors x y z des d'un fitxer.

Nota: els programes .ino han d'estar continguts en una carpeta amb el mateix nom que aquest programa; altrament, l'entorn es queixa.

Hi ha quatre carpetes addicionals:

./Wire

biblioteca arduino IDE modificada per tal d'admetre la inicialització del bus i2c sense resistències internes de pullup.
Habitualment aquestes resistències van lligades a +5 V i el nostre prototip treballa amb +3,3 V. La biblioteca Wire estàndard de l'arduino IDE
no contempla aquesta situació i aquesta, modificada i retrocompatible, admet activar(per defecte)/desactivar els pullups interns.

./vterm

que conté biblioteca vterm per tal d'usar controls de terminal vterm (VT100). S'instal·la a l'arduino IDE local a l'usuari.

./maquina

que conté maquina.h i maquina.c: formen l'algorisme de detecció de caiguda basat en diagrama d'estat i valors llindar.
A destacar que la carpeta ./prova_arduino_adxl345 conté enllaços simbòlics a aquests. Ho fem perquè l'arduino IDE pugui compilar-los
ja que no accepta que siguin fora de la carpeta del programa principal .ino.

./captures

que conté fitxers ascii amb línies amb lectures x y z.
S'han obtingut capturant al terminal l'execució de prova_arduino_adxl345_captura_continua.ino.
Aquest fitxer s'usa com a entrada del programa C prova_c_fitxers_a_algorisme.


Per compilar el programa c:
gcc ./prova_c_fitxers_a_algorisme.c maquina/maquina.c -o ./prova_c_fitxers_a_algorisme.out





