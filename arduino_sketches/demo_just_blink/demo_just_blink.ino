/* This is an alternative Firmware for the Bosch Athlet VaccumCleaner.
   Here we just blink all possible outputs for demonstration.
   See readme.md for details.
*/

#define PIN_PB1_PWM_MAINMOTOR 9
#define PIN_PB2_PWM_AUXMOTOR 10
#define PIN_PD6_GATE_DRIVER_ENABLE 6
#define PIN_PB3_LED3 11
#define PIN_PB4_LED4 12
#define PIN_PB5_LED5 13
#define PIN_PD3_SWITCH_STEP2_ECO   3 /* switches are low-active. */
#define PIN_PD4_SWITCH_STEP3_HIGH  4
#define PIN_PD5_SWITCH_STEP4_TURBO 5
#define PIN_PB0_PRESSURESENSOR 8 /* applying pressure to the upper pipe pulls the pin low. */

//#define PIN_PB6_LED2 20 /* no standard pin mapping for these in the Arduino IDE. Pin20 to work needs MiniCore (328pb) installation. */
//#define PIN_PB7_LED1 21 /* no standard pin mapping for these in the Arduino IDE. Pin21 to work needs MiniCore (328pb) installation. */
/* most portable solution: use direct register access, to avoid the dependency from MiniCore installation */

void setLed1Red(uint8_t on) {
  if (on) {
    PORTB |=  (1 << PB7);   // PB7 HIGH
  } else {
    PORTB &= ~(1 << PB7);   // PB7 LOW
  }
}

void setLed2Blue(uint8_t on) {
  if (on) {
    PORTB |=  (1 << PB6);   // PB6 HIGH
  } else {
    PORTB &= ~(1 << PB6);   // PB6 LOW
  }
}

void setup() {
  pinMode(PIN_PB3_LED3, OUTPUT);
  pinMode(PIN_PB4_LED4, OUTPUT);
  pinMode(PIN_PB5_LED5, OUTPUT);
  pinMode(PIN_PB1_PWM_MAINMOTOR, OUTPUT);
  pinMode(PIN_PB2_PWM_AUXMOTOR, OUTPUT);
  pinMode(PIN_PD6_GATE_DRIVER_ENABLE, OUTPUT);
  pinMode(PIN_PD3_SWITCH_STEP2_ECO, INPUT);
  pinMode(PIN_PD4_SWITCH_STEP3_HIGH, INPUT);
  pinMode(PIN_PD5_SWITCH_STEP4_TURBO, INPUT);
  pinMode(PIN_PB0_PRESSURESENSOR, INPUT_PULLUP);
  DDRB |= (1 << PB6) | (1 << PB7);  // Set PB6(LED2) and PB7(LED1) as OUTPUT
  digitalWrite(PIN_PB3_LED3, LOW);
  digitalWrite(PIN_PB4_LED4, LOW);
  digitalWrite(PIN_PB5_LED5, LOW);
  digitalWrite(PIN_PB1_PWM_MAINMOTOR, LOW);
  digitalWrite(PIN_PB2_PWM_AUXMOTOR, LOW);
  digitalWrite(PIN_PD6_GATE_DRIVER_ENABLE, HIGH);
}

uint16_t getDelayDependingOnSwitch_ms(void) {
  if (digitalRead(PIN_PD3_SWITCH_STEP2_ECO)==0) return 300;
  if (digitalRead(PIN_PD4_SWITCH_STEP3_HIGH)==0) return 150;
  if (digitalRead(PIN_PD5_SWITCH_STEP4_TURBO)==0) return 50;
  if (digitalRead(PIN_PB0_PRESSURESENSOR)==0) return 10;

 return 500; /* 500ms default */
}

void loop() {
  #define waitingtime_ms getDelayDependingOnSwitch_ms()
  #ifdef TEST_MOTOR_RAMPS
    int16_t i;
    for (i=0; i<255; i+=1) { /* ramp up the main motor */
      if (i>255) i=255;
      analogWrite(PIN_PB1_PWM_MAINMOTOR, i);
      delay(5);
    }
    for (i=255; i>=0; i-=1) { /* ramp down the main motor */
      if (i<0) i=0;
      analogWrite(PIN_PB1_PWM_MAINMOTOR, i);
      delay(5);
    }
    for (i=0; i<255; i+=1) { /* ramp up the aux motor */
      if (i>255) i=255;
      analogWrite(PIN_PB2_PWM_AUXMOTOR, i);
      delay(5);
    }
    for (i=255; i>=0; i-=1) { /* ramp down the aux motor */
      if (i<0) i=0;
      analogWrite(PIN_PB2_PWM_AUXMOTOR, i);
      delay(5);
    }
  #endif
  setLed1Red(1);
  delay(waitingtime_ms);
  setLed2Blue(1);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB3_LED3, HIGH);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB4_LED4, HIGH);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB5_LED5, HIGH);
  delay(waitingtime_ms);

  setLed1Red(0);
  delay(waitingtime_ms);
  setLed2Blue(0);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB3_LED3, LOW);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB4_LED4, LOW);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB5_LED5, LOW);
  delay(waitingtime_ms);
}
