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
  DDRB |= (1 << PB6) | (1 << PB7);  // Set PB6(LED2) and PB7(LED1) as OUTPUT
  digitalWrite(PIN_PB3_LED3, LOW);
  digitalWrite(PIN_PB4_LED4, LOW);
  digitalWrite(PIN_PB5_LED5, LOW);
  digitalWrite(PIN_PB1_PWM_MAINMOTOR, LOW);
  digitalWrite(PIN_PB2_PWM_AUXMOTOR, LOW);
  digitalWrite(PIN_PD6_GATE_DRIVER_ENABLE, HIGH);
}

void loop() {
  #define waitingtime_ms 150
  
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
  digitalWrite(PIN_PB1_PWM_MAINMOTOR, HIGH);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB2_PWM_AUXMOTOR, HIGH);
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
  digitalWrite(PIN_PB1_PWM_MAINMOTOR, LOW);
  delay(waitingtime_ms);
  digitalWrite(PIN_PB2_PWM_AUXMOTOR, LOW);
  delay(waitingtime_ms);
}
