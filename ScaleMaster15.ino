//Tasti
//5: spegnimento ritardato a fine sequenza (mod 128)

#include <LCD5110_Graph.h>
#include <TimedAction.h>
#include"Config.h";

boolean debug = false;
//boolean debug = true;

// =========== PARAMETRI MODIFCABILI DI BOOT
//protogestione hardware 5: LCD 5110  4:4884
char hardware = 4;
//flag acceso/spento
boolean acceso = true; //all'inizio va girato al contrario
//se attivo sostituisce joystick ai potenziometri
boolean joystickComm = true;
//status
//0: ""/N esegui note
//1: ""/C attiva CC
//2: ""/M Multichannel
//3: X entro tot tempo resetta
char FunStatus[4] = {'N', ' ', 'M', ' '}; //Stato funzione
char posidx = 0; //posizione status
char beatdiv = 16; //divisore battuta norm=16

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
// ==================


int PotAverage[12] = {  0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte OldPOTVal[12] = {  127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
byte lastx, lasty;

//vettore sequencer
byte vetSeq[16] ;
//vettore canali attivi MAX 6
boolean chanarray[6];

//indice strumento
char insidx = 1;

//vettore toni
byte whitekeys[] = {0, 2, 4, 5, 7, 9, 11};

int i = 0;

//variabile col tasto premuto equivalente al pot
char potaction = 11;
// valori min max dei pot PS: potevo renderlo costante ma preferisco inizializzarlo nell'input
int potminmax[12][2] = {{0, 127}, {0, 10}, {24, 83}, {0, 127},
  {0, 99}, {0, 99}, {0, 127}, {0, 127},
  {0, 127}, {10, 240}, {0, 9}, {0, 11}
};

boolean luciArray[6]; //array che simulano i led[1-4] e il MidiBlink[0]
boolean luciArrayback[6]; //backup

//lunghezza beat, se =0 allora leggi il potenziometro, altrimenti e' beat da usare
int realSpeed = 0;

////Matrici Attive
//Play stack
//0: nota
//1: canale
//2: volume
//3: durata (in tick)
//4: inizio (in tick)
//5: Tag
byte playEle = 32; //attenzione, che sia uguale alla dimensione
byte playSt[32][6];

//End Stack
//0: nota
//1: canale
//2: durata (in tick)
//3: Tag
byte endEle = 32; //attenzione, che sia uguale alla dimensione
byte endSt[32][4];

//Base Pattern
byte basePat[64];

//storage valori cc
byte ccVal[10][2];

byte TastoNota[12];
byte TastoNotaOld[12];

//spegni differito
boolean diffstop = false;
//conteggio timer per screensaver (relativo)
unsigned long millStart;
//conteggiol timer per note (assoluto)
unsigned long noteStart;


// Joystick
int x_position;
int y_position;
int x_direction;
int y_direction;
int x_speed;
int y_speed;

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



void setup() {
  if (debug)Serial.begin(9600);
  else Serial.begin(31250);
  Serial.flush() ;
  initLcd();
  //logo(2000);
  layout(true);
  displayButtonStatus(potaction); //ora necessario qui per via della nuova gestione tasti
  statusfun(10);

  statusPot(F("Init Btn"));
  inizializzaPulsanti();
  inizializzaDigital();

  azzeraStacks();
  OnOff();

  //Prerequisiti
  SSTick.enable();
  
  goodRandomseed();

  setDefault();
  carica();
  millStart = millis();
}


void loop() {

    if ((millis() - millStart) > 20000) SSTick.check();
  if (joystickComm == true) setfromJoystick();

  displayPotStatus();

  LeggiPulsanti();
  ElaboraPulsanti();

  //Trasferisci vecchi valori nei nuovi
  for (int i = 0; i < 12; i++)OldPOTVal[i] = PotAverage[i];
  for (int i = 0; i < 7; i++)TastoNotaOld[i] = TastoNota[i];

  myGLCD.update();
}
