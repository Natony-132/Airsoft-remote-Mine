//Copyrite Nathan Julicher 2024
// intended for hand transmiter V2

#include <RadioLib.h>

#define NSS 8
#define DIO0 
#define RESET 12
#define DIO1 14
#define SCK 9
#define MISO 11
#define MOSI 10
#define BUSY 13

SX1262 radio = new Module(NSS, DIO1, RESET, BUSY);

const int flipFlop = 45;  //-------------This can be used as the fire/chirp switch 100k pull up?
const int Trigger = 46;
const int wakeupPin = 46;//------------------------This is the pin that will trigger leaving sleep mode, it is set to be the same as the trigger
#define ledpin 35
int ledState = 1;
int x = 0;

//------------- need the 6 input switches
const int Switch1 = 27;
const int Switch2 = 28;
const int Switch3 = 26;
const int Switch4 = 21;
const int Switch5 = 20;
const int Switch6 = 19;
//-------------- The 6 fire codes
const int Fire1 = 8003;
const int Fire2 = 81423;
const int Fire3 = 31646;
const int Fire4 = 41807;
const int Fire5 = 51006;
const int Fire6 = 61283;
//--------------The 6 Chirp codes
const int Chirp1 = 83090;
const int Chirp2 = 82514;
const int Chirp3 = 34607;
const int Chirp4 = 44813;
const int Chirp5 = 54092;
const int Chirp6 = 64226;
//------------------------------The 6 values that will be transmitted if the trigger is pressed. They should stay at 0 until transmission.
int Transmit1 = 0;
int Transmit2 = 0;
int Transmit3 = 0;
int Transmit4 = 0;
int Transmit5 = 0;
int Transmit6 = 0;

//---------------------------------------------------------------------------------------------------------------

void setup() {

pinMode(flipFlop, INPUT); 
pinMode(Trigger, INPUT);           //-----------------------------------------button input
pinMode(ledpin, OUTPUT);
pinMode(wakeupPin, INPUT);
//------------------------new switches
pinMode(Switch1, INPUT);
pinMode(Switch2, INPUT);
pinMode(Switch3, INPUT);
pinMode(Switch4, INPUT);
pinMode(Switch5, INPUT);
pinMode(Switch6, INPUT);
//--------------------------
esp_sleep_enable_ext0_wakeup((gpio_num_t) wakeupPin, HIGH);//------------Sleep setup

  Serial.begin(115200);

SPI.begin(SCK, MISO, MOSI, NSS);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin(915.1);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

}

//-----------------------------------------------------------------------main loop
void loop() {
      
  int TriggerState = digitalRead(Trigger);
  int flipFlopState = digitalRead(flipFlop);

  int Switch1State = digitalRead(Switch1);
  int Switch2State = digitalRead(Switch2);
  int Switch3State = digitalRead(Switch3);
  int Switch4State = digitalRead(Switch4);
  int Switch5State = digitalRead(Switch5);
  int Switch6State = digitalRead(Switch6);

Serial.println(x);
//--------------------------LED blink
 if (x >= 20){
    if (ledState == 1){
      digitalWrite(ledpin, LOW);
      ledState = 0;
      x = 0;
    }
    else{
      digitalWrite(ledpin, HIGH);
      ledState = 1;
      x = 15;
    }
  }
 x = x+1;
//-----------------------------End of LED code


  if (TriggerState == 1) {
      
      if (flipFlopState == 1) {
        
        Transmit1 = Fire1;
        Transmit2 = Fire2;
        Transmit3 = Fire3;
        Transmit4 = Fire4;
        Transmit5 = Fire5;
        Transmit6 = Fire6;
      }

      else {

        Transmit1 = Chirp1;
        Transmit2 = Chirp2;
        Transmit3 = Chirp3;
        Transmit4 = Chirp4;
        Transmit5 = Chirp5;
        Transmit6 = Chirp6;

      }

  for (int i = 0; i <= 3; i++) {
     digitalWrite(ledpin, HIGH);
     delay(50);

    if (Switch1State == 1) {
      int state = radio.transmit(Transmit1);//---------------------------------Transmit Group 1

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 1!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

    if (Switch2State == 1) {
      int state = radio.transmit(Transmit2);//---------------------------------Transmit Group 2

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 2!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

    if (Switch3State == 1) {
      int state = radio.transmit(Transmit3);//---------------------------------Transmit Group 3

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 3!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

    if (Switch4State == 1) {
      int state = radio.transmit(Transmit4);//---------------------------------Transmit Group 4

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 4!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

    if (Switch5State == 1) {
      int state = radio.transmit(Transmit5);//---------------------------------Transmit Group 5

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 5!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

    if (Switch6State == 1) {
      int state = radio.transmit(Transmit6);//---------------------------------Transmit Group 6

      if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F("success on 6!"));
        // print measured data rate
        Serial.print(F("[SX1262] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
      } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));
      } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
       // timeout occured while transmitting packet
      Serial.println(F("timeout!"));
      } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
      }
    }

     digitalWrite(ledpin, LOW);
     delay(50);
  }

        Transmit1 = 0;//--------------------Set the transmit codes back to 0
        Transmit2 = 0;
        Transmit3 = 0;
        Transmit4 = 0;
        Transmit5 = 0;
        Transmit6 = 0;

  }//------------------------END of trigger statment


//delay(100);

}