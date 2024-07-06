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

char transmitMsgp[] = "123CHRP000000000010"; // first 3: transmitter ID. 4-7 is the command, 8-17 are 10 action groups. 18,19 are the check sum.

//------------- need the 6 input switches
const int Switch1 = 27;
const int Switch2 = 28;
const int Switch3 = 26;
const int Switch4 = 21;
const int Switch5 = 20;
const int Switch6 = 19;

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
        
       //Fire code
      }

      else {

      //chirp code
      }

  for (int i = 0; i <= 3; i++) {
     digitalWrite(ledpin, HIGH);
     delay(50);

    if (Switch1State == 1) {
      int state = radio.transmit("8003");// legacy compatibility
      //check if action group is on
      transmitMsg[8] = '1';
    }

    if (Switch2State == 1) {
      int state = radio.transmit("81423");// legacy compatibility
      int state = radio.transmit("8142");// legacy compatibility
     //check if action group is on
     transmitMsg[9] = '2';
      }
    }

    if (Switch3State == 1) {
     //check if action group is on
     transmitMsg[10] = '3';
    }

    if (Switch4State == 1) {
      //check if action group is on
      transmitMsg[10] = '4';
    }

    if (Switch5State == 1) {
      //check if action group is on
      transmitMsg[11] = '5';
    }

    if (Switch6State == 1) {
     //check if action group is on
     transmitMsg[12] = '6';
    }

    //build the check sum and add it to the transmit msg


  int state = radio.transmit(transmitMsg);//---------------------------------Transmit

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


     digitalWrite(ledpin, LOW);
     delay(50);
  }
// set the transmit msg back to start
transmitMsgp[] = "123CHRP000000000010";
  }//------------------------END of trigger statment


//delay(100);

}