void MIDImessage(int command, int data1, int data2) {
  if ((acceso == true) || (command < 176)) {
    if (!debug) {
      Serial.write(command);//send command byte
      Serial.write(data1);//send data byte #1
      Serial.write(data2);//send data byte #2
    }
    else {
      Serial.println("Cmd: " + String(command) + F(" - Nota ") + String(data1) + F(" - Vel. ") + String(data2));
    }
    //  if ((luciArray[0] == false) && (data2 != 0) && (command < 176 )) {
    if ((data2 != 0) && (command < 176 )) {
      lcdprint(String(command, HEX) + F("-") + returnNote(command, data1) + F(".") + String(data2), 32, Dx);
      luciArray[0] = true; //midi led on
    }
  }
}

void MIDIhardreset() {
  if (!debug) {
    Serial.write(0xFF);
    Serial.flush() ;
  }
}


void MIDIsoftreset() {
  if (!debug) {
    //Serial.println(“MSC reset”);
    Serial.write(0xF0);
    Serial.write(0x7F);
    Serial.write(0x7F); //device id 7F = 127 all channels
    Serial.write(0x02);
    Serial.write(0x7F); // command format 7F = all devices
    Serial.write(0x0A); // action 0x0A= reset
    Serial.write(0xF7);
    //extra
    Serial.write(0x78);
    Serial.write(0x79);
    Serial.flush() ;
  }
}


void spengoNote() {
  //spengo tutte le note
  if (!debug) {
    for (int i = 0; i < endEle; i++) {
      if (endSt[i][0] != 255) {
        MIDImessage(NOTAOFF + endSt[i][1], endSt[i][0], 0);
      }
    }
  }
  else serprint(F("Spengo TUTTE le note"));
}


int SetChannel(int channelsplit) {
  //Channel control
  //mettere negativo per attivare la rotazione, altrimenti usa il canale fisso
  //se negativo, l'assoluto diventa il range di canali da rotazione (es. -3 -> rotazione canali 1,2,3)
  int result;
  if (channelsplit < 0) {
    channelsplit = abs(channelsplit);
    channelsplit = constrain(channelsplit, 0, 15);
    result = (contick % channelsplit) ;
  }
  else {
    channelsplit = constrain(channelsplit, 0, 15);
    result = channelsplit;
  }
  return result;
}

int setnoteSpeed(int RS, byte mult) {
  int result;
  if (RS == 0) result = beat2time(PotValue(9) * mult);
  else result = beat2time(RS);
  return result;
}


char volumePlus(int volume, float pervaria, char freqvaria) {
  // pervaria: percentuale di variazione volume
  // freqvaria: frequenza di variazione volume (<=1= sempre, 2=1 volta su 2, 3=1 volta su 3 ecc)
  int result;

  //normalizza
  if (freqvaria < 1) freqvaria = 1;
  if (pervaria > 100) pervaria = 100;
  if (pervaria < 0) pervaria = 0;

  result = volume;
  //applica solo se casualmente e' selezionato
  if (RandNum(1, freqvaria) == freqvaria) {
    // se volume e' 0 non applicare viariazioni
    if (volume > 0) {
      if (RandNum(0, 1) == 1) result = result + (result * (pervaria / 100));
      else result = result - (result * (pervaria / 100));
    }
    if (result < 0) result = 0;
    if (result > 127) result = 127;
  }
  return result;
}

char volumePlusPlus(int volume, float rangevaria, char freqvaria, char calcolo, char lennota) {
  // rangevaria: percentuale o range di variazione volume
  // freqvaria: frequenza di variazione volume (<=1= sempre, 2=1 volta su 2, 3=1 volta su 3 ecc)
  // calcolo: 1= percentuale fissa come in passato, 2=range sul quale agire
  // lennota: se diverso da 0 applica una regola proporzionale alla lunghezza (piano le corte, forte le lunghe)
  int result;
  byte volmax;
  char volmin;

  //normalizza
  volume = constrain(volume, 0, 127);
  result = volume;
  freqvaria = max(1, freqvaria);

  // se volume e' 0 non applicare viariazioni
  if ((volume > 0) && (RandNum(1, freqvaria) == freqvaria)) {
    //applica solo se casualmente e' selezionato
    switch (calcolo) {
      case 1: //percentuale fissa
        rangevaria = constrain(rangevaria, 0, 100);
        result = volume + (segnoRandom() * (volume * (rangevaria / 100)));
        break;
      case 2: //percentuale variabile
        // min e max =volume +-range/2
        volmin = volume - ((rangevaria * 2) / 3);
        volmax = volume + (rangevaria / 3);
        lennota = constrain(lennota, 0, 4);
        // se la nota e' definita sposta in basso o un alto il range di probabilita'
        if (lennota > 0) {
          volmin = volmin - ((5 - lennota) * 5);
          // volmax = volmax + (lennota * 2);
          volmin = constrain(volmin, 1, volmax);
          volmax = constrain(volmax, volmin, 127);
        }
        result = RandNum(volmin, volmax);
        break;
    }
  }
  result = constrain(result, 0, 127);
  return result;
}

String returnNote(int command, int midival) {

  String ret;
  //se nota e non un comando stampa nota senno' il valore puro
  if (command < 176) {
    ret = score[midival % 12];
    ret += String(midival / 12, DEC);
  }
  else ret = String(midival, DEC);

  return ret;
}

byte ottavaRandom(byte tiponote) {
  //ritorna nota casuale su un'ottava con o senza i semitoni
  //Tiponote= Tutto / WhiteOnly
  byte ret;
  if (tiponote == Tutto) ret = RandNum(0, 11);
  else ret = whitekeys[RandNum(0, 6)];
  return ret;
}

byte generaNota(byte notada, byte notaa, byte algochoice) {
  // ora la funzione non fa nulla oltre il generare una nota dato un certo range
  // ma e' pronta per restituire note pesate
  // algochoice= funzione differenziata di generazione nota
  byte ret;
  switch (algochoice) {
    case 0: //random
      ret = RandNum(notada, notaa);
      break;
    default: //se non viene riconosciuto alcun valore allora random
      ret = RandNum(notada, notaa);
  }

  return ret;
}

//================ scale ===========================
byte notaW(char notaw) {
  // il sistema e' grezzo ma funziona in base al tasto nero, da' 2/3 di probabilita' ad ogni tasto bianco che accada e tutti i bianchi sono equiprobabili
  int ret;
  char cont = -1, idx = -1;
  char ottava, nota;
  char rnum;
  ottava = notaw / 12;
  nota = notaw % 12;
  //trova la nota nel vettore
  do {
    cont++;
    idx++;
    if (idx > 7) idx = 0;
  } while ((whitekeys[idx] != nota) && (cont < 7));
  // se nota non bianca
  if (cont >= 7) {
    rnum = RandNum(1, 3);
    // funziona ridotta ma funziona. Se si vuole riprodurre, distribuire la probabilita' di 1/3 sulle note bianche
    // 1 o 6
    if ((nota == 1) || (nota == 6)) {
      if (rnum == 1) notaw = notaw + 1;
      else notaw = notaw - 1;
    }
    else  {
      if (rnum == 1) notaw = notaw - 1;
      else notaw = notaw + 1;
    }
  }
  ret = notaw;
  return ret;
}

byte firstNotaW(char notaw, char notepiu) {
  // somma notepiu alla nota di partenza, sempre note bianche
  int ret;
  char cont = -1, idx = -1;
  char ottava, nota;
  char rnum;
  ottava = notaw / 12;
  nota = notaw % 12;
  //trova la nota nel vettore
  do {
    cont++;
    idx++;
    if (idx > 7) idx = 0;
  } while ((whitekeys[idx] != nota) && (cont < 7));
  // se nota non bianca
  if (cont >= 7) idx = 0; //non deve accadere, perche' si suppone passino solo note bianche
  idx = idx + notepiu;
  // se piu' di 6
  if (idx >= 7) {
    idx = idx % 7;
    ottava = ottava + 1;
  }
  // se meno di zero
  if (idx < 0) {
    idx = 7 + idx;
    ottava = ottava - 1;
  }
  ottava = constrain(ottava, 0, 9);
  notaw = (ottava * 12) + whitekeys[idx] ;
  ret = notaw;
  return ret;
}



byte getScala(char notaf, String  tiposcala) {
  //idx= nota da calcolare NB 0-6
  byte idx = scalapos;
  notaf = constrain(notaf, 0, 6);
  for (int i = 0; i < notaf + 1; i++)  {
    if (tiposcala == "M")     idx = idx +  scalamaggiore[i];
    else idx = idx +  scalaminore[i];
  }
  return idx;

}
