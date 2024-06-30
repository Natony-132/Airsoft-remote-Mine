#include <esp_system.h>
#include <RadioLib.h>
#include <Deneyap_Servo.h>     
 Servo myservo;

uint32_t Freq = 0;

long servo_open_time = 0;
long servo_close_time = 0;

#define LED_OFF_TIME 200
#define LED_ON_TIME 2000
#define MAGIC_CODE 8142
#define SERVODELAY 500
#define LED_PIN 35

int TriggerPin = 46;                //-----------------------------------------button input
int TriggerLast = 0;

#define NSS 8 
//#define DIO0 
#define RESET 12
#define DIO1 14
#define SCK 9
#define MISO 11
#define MOSI 10
#define BUSY 13

SX1262 radio = new Module(NSS, DIO1, RESET, BUSY);



void setup() {
   Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT); // LED light

pinMode(TriggerPin, INPUT);           //-----------------------------------------button input
int TriggerLast = 0;

  myservo.attach(45);

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

  radio.setDio1Action(setFlag);

// start listening for LoRa packets
  Serial.print(F("[SX1262] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
 } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

}


// flag to indicate that a packet was received
volatile bool messageReceived = false;

// this function is called when a complete packet
// is received by the module

#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif

void setFlag(void) {
  // we got a packet, set the flag
  messageReceived = true;

}


enum states {
return_to_waiting,
message_received,
move_servo_open,
servo_wait,
move_servo_closed,
servo_delay,
led_off,
led_on,
sleepy
};

char *stateStrings[] = {
"return to waiting", "message received", "move servo open", "servo wait",
"move servo closed","servo delay", "led off", "led on", "sleepy"};


enum states the_state = return_to_waiting;
int LED_ON = false;
int led_timer_ms = 0;

void loop() { //---------------------------------------------------------------------------- LOOP

  enum states next_state = the_state;

int Trigger1 = digitalRead(TriggerPin);   //---------------------------------------------------trigger

  Serial.println(stateStrings[the_state]);

  switch (the_state) {
  default:
  case return_to_waiting:


  if (messageReceived) {
  next_state = message_received;
  }
  else if (LED_ON == true) {
  next_state = led_off;
  }
  else if (LED_ON == false) {
  next_state = led_on;
  }
 
//--------------------------------------------------------
  if (Trigger1 == 0){
 Serial.printf("%d\n", TriggerLast);
  if (TriggerLast == 1){
    TriggerLast = 0;
    next_state = move_servo_open;
    }
  }
  else{
  TriggerLast = 1;
  }
 Serial.printf("trigger Last %d\n", TriggerLast);
 Serial.printf("trigger %d\n", Trigger1);
 //---------------------------------------------------------

  
  break;
  case message_received: {
  //char buffer[16];
  //radio.read(buffer, sizeof(buffer));
  //int value = atoi(buffer);

  messageReceived = false;

  uint8_t str[16];
      int state = radio.readData(str,4);

      int fireCode = atoi((const char *)str);

      if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1262] Received packet!"));

      // print data of the packet
      Serial.print(F("[SX1262] Data:\t\t"));
      Serial.printf("%d\n", fireCode);

      // print RSSI (Received Signal Strength Indicator)
      Serial.print(F("[SX1262] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      // print SNR (Signal-to-Noise Ratio)
      Serial.print(F("[SX1262] SNR:\t\t"));
      Serial.print(radio.getSNR());
      Serial.println(F(" dB"));

      // print frequency error
      Serial.print(F("[SX1262] Frequency error:\t"));
      Serial.print(radio.getFrequencyError());
      Serial.println(F(" Hz"));
    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
        // packet was received, but is malformed
        Serial.println(F("CRC error!"));

      } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
      }

  if (fireCode == MAGIC_CODE) {
  next_state = move_servo_open;
  } else {
  next_state = return_to_waiting;
  }
  } break;
  case move_servo_open:
  myservo.write(8);
  next_state = servo_wait;
  servo_open_time = millis();
  digitalWrite(LED_PIN, HIGH);
  break;
  case move_servo_closed:
  myservo.write(33); 
   servo_close_time = millis();
  next_state = servo_delay;
  digitalWrite(LED_PIN, LOW);
  break;
  case servo_wait:
  if (millis() - servo_open_time > SERVODELAY) {
    
  next_state = move_servo_closed;
  }
  break;
  case servo_delay:
//for (int z = 0; z =< 100; z++){

  if (millis() - servo_close_time > SERVODELAY) {
     
  next_state = return_to_waiting;
  }
  break;
  case led_off:
  digitalWrite(LED_PIN, LOW);
  LED_ON = false;
  next_state = sleepy;
  led_timer_ms = LED_OFF_TIME;
  break;
  case led_on:
  digitalWrite(LED_PIN, HIGH);
  LED_ON = true;
  next_state = sleepy;
  led_timer_ms = LED_ON_TIME;
  break;
  case sleepy:

  esp_sleep_enable_timer_wakeup(led_timer_ms * 1000);

          // esp_sleep_enable_ext0_wakeup(GPIO_NUM_14,0);
  esp_light_sleep_start();
  next_state = return_to_waiting;
  break;
  }
  the_state = next_state;
}