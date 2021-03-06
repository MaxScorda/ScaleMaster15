/*
  PIN 9: beep
  PIN 11: if 0 (gnd) play minor scale else major scale
  Instructions:
  Up/Down: change scale
  Rx/Lx: change 90/60/30 BPM +
          S: Mode by Pin 11
          A: Major and Minor mode
          C: continuos play with tone change
*/

#include <LCD5110_Graph.h>
#include <TimedAction.h>
#include"Config.h";

//boolean debug = false;
boolean debug = true;

// =========== PARAMETRI MODIFCABILI DI BOOT
//flag acceso/spento
boolean acceso = true; //all'inizio va girato al contrario


// =========== fine parametri BOOT

// forward declaration
// serve per compatibilita' per poter compilare con la 1.8 la TimedAction
// usare laddove servisse dichiarare prima
int setnoteSpeed(int RS, byte mult);
void tickNote();
void CCChange() ;
void controlBlink() ;
void screenSaver();
void Pauset() ;


// ======= scala =======
String score[] = {("C"), ("C#"), ("D"), ("D#"),
                  ("E"), ("F"), ("F#"), ("G"),
                  ("G#"), ("A"), ("A#"), ("B")
                 };
String scoreN [] = {("Do"), ("Do#"), ("Re"), ("Re#"),
                    ("Mi"), ("Fa"), ("Fa#"), ("So"),
                    ("So#"), ("La"), ("La#"), ("Si")
                   };

char scalamaggiore[] = {0, 2, 2, 1, 2, 2, 2 };
char scalaminore[] = {0, 2, 1, 2, 2, 1, 2 };
char scalapos = 10;
int tones[] = {262, 277, 294, 311,
               330, 349, 370, 392,
               415, 440, 466, 494
              };
int duration = 800;
// ==================


int PotAverage[12] = {  0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte OldPOTVal[12] = {  127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
byte lastx, lasty;


byte TastoNota[12];
byte TastoNotaOld[12];
byte lastread;

TimedAction SSTick = TimedAction(30000, screenSaver);  //Screensaver

//LCD
LCD5110 myGLCD(2, 3, 4, 6, 5);

extern uint8_t hard_logo[];
extern uint8_t fullSquare[];
extern uint8_t emptySquare[];
extern uint8_t SmallFont[];
extern uint8_t TinyFont[];
boolean statusblink = true;

//contatori
unsigned contick = 0;
//conteggio timer per screensaver (relativo)
unsigned long millStart;

//struttura

struct sInfo {
  bool PlayScala; //1: play major scale, 0:play minor digital 11
  byte howtoplay; //0-9:  30/60/90 bpm con s:suono M e m in base scelta, T:suono M e n, T: scala scelta su cambio scala
};
typedef struct sInfo SSInfo;
SSInfo strucInfo;


void setup() {
  if (debug)Serial.begin(9600);
  else Serial.begin(31250);
  Serial.flush() ;
  initLcd();
  //logo(2000);
  layout(true);
  inizializzaDigital();
  inizializzaVar();
  OnOff();
  //Prerequisiti
  SSTick.enable();
  goodRandomseed();
  millStart = millis();
}


void loop() {
  if ((millis() - millStart) > 20000) SSTick.check();
  CheckInput();
  LeggiPulsanti() ;
  ElaboraPulsantiAnalog();
  displayPotStatus();
  PlayControl();
  //Trasferisci vecchi valori nei nuovi
  for (int i = 0; i < 12; i++)TastoNotaOld[i] = TastoNota[i];

  myGLCD.update();
}
