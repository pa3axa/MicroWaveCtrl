

 
// 24Ghz Wavelab Sequencer PA3AXA

// Constants

const int DC12V = 8;
const int DC28V = 5;
const int DC6V = 10;
const int TXFWD = 11;

const int RL1 = 3;
const int RL2 = 2;
const int RL3 = 4;
const int TXS = 12;   // on board LED
const int HB = 13;    // on board LED

const char ADC7 = A7;

// Variables

int MODE = 2;  // 2 TX/RX Allowed, 1 RX Allowed, 0 Lipocut off 
int TXON = 648;
int LPCOFF = 547;
int adcr = 0;

// Use with millis()

uint32_t hb_time = 0; // time in msec
uint32_t hb = 2000;   // periode time in msec

boolean hbstate = LOW; 
 
void setup() { 

  digitalWrite(DC12V, LOW);   // do this first to make sure output is of
  pinMode (DC12V, OUTPUT);

  digitalWrite(DC28V, LOW);   // do this first to make sure output is of
  pinMode (DC28V, OUTPUT);

  digitalWrite(RL1, LOW);   // do this first to make sure output is of
  pinMode (RL1, OUTPUT);
  
  digitalWrite(RL2, LOW);   // do this first to make sure output is of
  pinMode (RL2, OUTPUT);
   
  digitalWrite(RL3, LOW);   // do this first to make sure output is of
  pinMode (RL3, OUTPUT);
  
  digitalWrite(TXS, LOW);   // do this first to make sure output is of
  pinMode (TXS, OUTPUT);

  digitalWrite(HB, LOW);   // do this first to make sure output is of
  pinMode (HB, OUTPUT);

  digitalWrite(TXFWD, LOW);   // do this first to make sure output is of
  pinMode (TXFWD, OUTPUT);

// Default PowerUp WaveLab

  digitalWrite(DC28V, LOW);   // SMA relais cntrl
  digitalWrite(DC12V, HIGH);  // IC402 PowerOn
  digitalWrite(DC6V, HIGH);   // wavelab PowerOn
  
 } 

void hbwdg() {
  
  if ( (millis() - hb_time) >= hb ){
        digitalWrite(HB,!digitalRead(HB));
            hb_time = millis();  
  }
}


void loop() {

adcr = analogRead(ADC7);  // read the input pin

  /* Determin mode of operation
  --------------------------------------
   TX mode selected > 18V
   ADC = 10B Max 5V, ~ 0,005V / digit
   15V = 525
   15,3 = 548
   18V = 643
   20V = 705
  
  */

  // Current TX mode selected Vin > 18V

 if ( (adcr - TXON) >= 0 ) {
    MODE = 2;
 }

 // Prevent TX when input voltage is
 // near TXON
 
 if ( (adcr - TXON) < 10 ) {
    MODE = 1;
 }

 // Prevent TX/RX when input voltage is
 // near LipoProtect

 if ( (adcr - LPCOFF) < 5 ) {
    MODE = 0;
 }

  // TX Mode Selected  Vin > 18V
  
  if  ( MODE == 2 ) {

  hb = 2000; // TX mode hartbeat
  
  digitalWrite(TXS, HIGH);
  digitalWrite(DC28V, HIGH);
  delay (50); 
  digitalWrite(TXFWD, HIGH);
  digitalWrite(RL3, HIGH);
  digitalWrite(DC12V, HIGH);
  digitalWrite(DC6V, HIGH);

  // Hold TX until threshold
  
  while ((adcr - TXON) >= 0 ){   
       hbwdg();
       adcr = analogRead(ADC7);
  }

}


  // RX mode selected < 18V

  else if  (MODE == 1 ) {

  hb = 1000; // RX mode hartbeat
    
  digitalWrite(TXFWD, LOW);
  delay (50);
  digitalWrite(RL3, LOW);
  digitalWrite(DC28V, LOW);
  digitalWrite(TXS, LOW);
  digitalWrite(DC12V, HIGH);
  digitalWrite(DC6V, HIGH);


  // Hold RX until Lipo threshold
  
  while ((adcr - LPCOFF) >= 0 ){   
       hbwdg();
       adcr = analogRead(ADC7);
  }

 }

  // Lipo Protect Cut Off < 15,3V
  // Needs Power Reset to Recover

  else if ( MODE == 0 ) { 

  hb = 250; // Protected mode hartbeat
  
  digitalWrite(TXFWD, LOW);
  
  delay (50);
  
  digitalWrite(RL3, LOW);
  digitalWrite(DC28V, LOW);
  digitalWrite(TXS, LOW);
  digitalWrite(DC12V, LOW);
  digitalWrite(DC6V, LOW);

  // End less loop to protect liPo
  
  while (true) {
    hbwdg();
  }

 }

hbwdg();
  
} // Loop End
