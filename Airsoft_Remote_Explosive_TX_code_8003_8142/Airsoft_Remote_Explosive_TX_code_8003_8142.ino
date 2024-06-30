//Copyrite Nathan Julicher 2024

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

int flipFlop = 45;
int Trigger = 46;
#define ledpin 35
int ledState = 1;
int x = 0;




void setup() {

pinMode(flipFlop, INPUT); 
pinMode(Trigger, INPUT);           //-----------------------------------------button input
pinMode(ledpin, OUTPUT);

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

  // some modules have an external RF switch
  // controlled via two pins (RX enable, TX enable)
  // to enable automatic control of the switch,
  // call the following method
  // RX enable:   4
  // TX enable:   5
  /*
    radio.setRfSwitchPins(4, 5);
  */
}

void loop() {
      
  int TriggerState = digitalRead(Trigger);
  int flipFlopState = digitalRead(flipFlop);
Serial.println(x);

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

  if (TriggerState == 1) {
      Serial.print(F("[SX1262] Transmitting packet ... "));

      if (flipFlopState == 1) {
        int state = radio.transmit("81423");

        if (state == RADIOLIB_ERR_NONE) {
                // the packet was successfully transmitted
                Serial.println(F("success!"));

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

            for (int i = 0; i <= 8; i++) {
                    digitalWrite(ledpin, HIGH);
                    delay(50);
                    digitalWrite(ledpin, LOW);
                    delay(50);
                  }

      }

      else {
        int state = radio.transmit("8003");

        if (state == RADIOLIB_ERR_NONE) {
          // the packet was successfully transmitted
          Serial.println(F("success!"));

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

        for (int i = 0; i <= 8; i++) {
          digitalWrite(ledpin, HIGH);
          delay(50);
          digitalWrite(ledpin, LOW);
          delay(50);
        }

      }

    
  }
delay(100);
}