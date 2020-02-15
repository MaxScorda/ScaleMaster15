#include <LCD5110_Graph.h>
#include <TimedAction.h>
#include"Config.h";

boolean debug = false;
//boolean debug = true;

// =========== PARAMETRI MODIFCABILI DI BOOT
//flag acceso/spento
boolean acceso = true; //all'inizio va girato al contrario
//se attivo sostituisce joystick ai potenziometri
boolean joystickComm = true;


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
int tones[] = { 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976 };
int duration = 800;
// ==================


int PotAverage[12] = {  0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte OldPOTVal[12] = {  127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
byte lastx, lasty;


byte TastoNota[12];
byte TastoNotaOld[12];

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


void setup() {
  if (debug)Serial.begin(9600);
  else Serial.begin(31250);
  Serial.flush() ;
  initLcd();
  //logo(2000);
  layout(true);
  inizializzaDigital();
  OnOff();
  //Prerequisiti
  SSTick.enable();
  goodRandomseed();
  millStart = millis();
}


void loop() {
  if ((millis() - millStart) > 20000) SSTick.check();
  LeggiPulsanti() ;
  ElaboraPulsantiAnalog();
  displayPotStatus();

  //Trasferisci vecchi valori nei nuovi
  for (int i = 0; i < 7; i++)TastoNotaOld[i] = TastoNota[i];

  myGLCD.update();
}
