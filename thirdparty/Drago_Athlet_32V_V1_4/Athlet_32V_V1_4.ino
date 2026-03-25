/*
this code is an aternative software for Bosch vaccuumcleaner, tested and working on the BCH6L2560 & BBH7360K model. 
Other Bosch models may work, but YOU must chek first if your hardware is supported !
We take no responsability at all for this code, use it on your own risk. 
Here you find basic schematics (uncomplete) and fotosâ€¦. https://www.mikrocontroller.net/topic/513088
Many thaks to Daniel F.(per48or) for programming with me Lena S.(lena_s) this piece of code to awaken this vaccuumclears again.
*/

// SET your Board to MiniCore ATmega328PB @ 8Mhz Internal, BOD=2.7V, no Bootloader
// https://github.com/MCUdude/MiniCore#how-to-install

#include <Wire.h>

#define SWVERSION 240619

/*
//values for 25.2V-model BCH6L2560:
#define TYPE            "BCH6L2560"
#define NUM_OF_CELLS     7  // for BCH6L2560: number of battery cells for cell monitoring          
#define VAC_PWM_LOW    100  // for BCH6L2560: PWM vac-motor for switch-level LOW
#define VAC_PWM_MID    155  // for BCH6L2560: PWM vac-motor for switch-level MID
#define VAC_PWM_HIGH   185  // for BCH6L2560: PWM vac-motor for switch-level HIGH
#define BRUSH_PWM_LOW  120  // for BCH6L2560: PWM brush for switch-level LOW
#define BRUSH_PWM_MID  130  // for BCH6L2560: PWM brush for switch-level MID
#define BRUSH_PWM_HIGH 170  // for BCH6L2560: PWM brush for switch-level HIGH
#define BRUSH_I_MAX     2.2 // for BCH6L2560: overcurrent limit for brush motor   [amperes]
#define VBAT_LOW       21.0 // for BCH6L2560: Battery Voltage level for LED LOW   (discharging) discharge voltage levels are lower due high current of motor
#define VBAT_MID       22.6 // for BCH6L2560: Battery Voltage level for LED MID   (discharging)
#define VBAT_HIGH      24.1 // for BCH6L2560: Battery Voltage level for LED HIGH  (discharging)
#define VBAT_CHG_LOW   22.4 // for BCH6L2560: Battery Voltage level for LED LOW   (charging 33%)
#define VBAT_CHG_MID   25.2 // for BCH6L2560: Battery Voltage level for LED MID   (charging 66%)
                            // no VBAT_CHG_HIGH, charging ends when current goes low
*/

//values for 25.2V-model Bosch Athlet 25,2 Pro_Perfom, Modelnummer: BBH6P25K:
/*
#define TYPE            "BBH6P25K"
#define NUM_OF_CELLS     7  // for BCH6L2560: number of battery cells for cell monitoring          
#define VAC_PWM_LOW    100  // for BCH6L2560: PWM vac-motor for switch-level LOW
#define VAC_PWM_MID    140  // for BCH6L2560: PWM vac-motor for switch-level MID
#define VAC_PWM_HIGH   200  // for BCH6L2560: PWM vac-motor for switch-level HIGH
#define BRUSH_PWM_LOW  120  // for BCH6L2560: PWM brush for switch-level LOW
#define BRUSH_PWM_MID  140  // for BCH6L2560: PWM brush for switch-level MID
#define BRUSH_PWM_HIGH 165  // for BCH6L2560: PWM brush for switch-level HIGH
#define BRUSH_I_MAX     2.2 // for BCH6L2560: overcurrent limit for brush motor   [amperes]
#define VBAT_LOW       21.0 // for BCH6L2560: Battery Voltage level for LED LOW   (discharging) discharge voltage levels are lower due high current of motor
#define VBAT_MID       22.6 // for BCH6L2560: Battery Voltage level for LED MID   (discharging)
#define VBAT_HIGH      24.1 // for BCH6L2560: Battery Voltage level for LED HIGH  (discharging)
#define VBAT_CHG_LOW   22.4 // for BCH6L2560: Battery Voltage level for LED LOW   (charging 33%)
#define VBAT_CHG_MID   25.2 // for BCH6L2560: Battery Voltage level for LED MID   (charging 66%)
                            // no VBAT_CHG_HIGH, charging ends when current goes low
*/


//values for 32.4V-model BBH7360K:
#define TYPE            "BBH7360K"
#define NUM_OF_CELLS     9  // for BBH7360K: number of battery cells for cell monitoring
#define VAC_PWM_LOW      90 // for BBH7360K: PWM vac-motor for switch-level LOW  Motor-Voltage 11V
#define VAC_PWM_MID     115 // for BBH7360K: PWM vac-motor for switch-level MID  Motor-Voltage 16V
#define VAC_PWM_HIGH    160 // for BBH7360K: PWM vac-motor for switch-level HIGH Motor-Voltage 22V
#define BRUSH_PWM_LOW   110 // for BBH7360K: PWM brush for switch-level LOW      Motor-Voltage 18V
#define BRUSH_PWM_MID   130 // for BBH7360K: PWM brush for switch-level MID      Motor-Voltage 19V
#define BRUSH_PWM_HIGH  155 // for BBH7360K: PWM brush for switch-level HIGH     Motor-Voltage 22V
#define BRUSH_I_MAX     2.1 // for BBH7360K: overcurrent limit for brush motor   [amperes]
#define VBAT_LOW       27.0 // for BBH7360K: Battery Voltage level for LED LOW   (discharging) discharge voltage levels are lower due high current of motor
#define VBAT_MID       29.0 // for BBH7360K: Battery Voltage level for LED MID   (discharging)
#define VBAT_HIGH      31.0 // for BBH7360K: Battery Voltage level for LED HIGH  (discharging)
#define VBAT_CHG_LOW   32.0 // for BBH7360K: Battery Voltage level for LED LOW   (charging 33%)
#define VBAT_CHG_MID   34.3 // for BBH7360K: Battery Voltage level for LED MID   (charging 66%) 
                            // no VBAT_CHG_HIGH, charging ends when current goes low


                      
//Pins Arduino
#define LED_HIGH 11
#define LED_MID  12
#define LED_LOW  13
#define LED_BLAU 20
#define LED_ROT  21
#define POWER     6 // 12V for FETs
#define VAC       9 // motor for vaccuum PWM (max18V)
#define BRUSH    10 // Motor for brush PWM
#define ECO       3 // on switch pos 1
#define NORM      4 // on switch pos 2
#define TURBO     5 // on switch pos 3
#define PRESSURE  8 // pressure sensor
#define IBAT     A3 // IBAT from BQ77PL900 monitors charge (=positive) und brush (=negative) currents
#define VBAT     A7 // VBAT from BQ77PL900 monitors batt voltages
#define BQWRITE  16 // EEPROM BQ write (enable = HIGH)

//Variables
float BAT=35;
float UCELL=0;
float UCELLMIN=0;
float UCELLMAX=0;
float I_CHG_BRUSH=0;
int IBAT_ZERO=0;
int t_off=0;
int t_meas=10; 
byte VAC_PWM_SET=0;
byte BRUSH_PWM_SET=0;
byte VAC_PWM_ACT=0;
byte BRUSH_PWM_ACT=0;
byte TEMP=0;
boolean charging=LOW;
boolean chargemode=LOW;
boolean brush_OC=LOW;
boolean toogle=LOW;


void setup() {
  Wire.begin();
  Serial.begin(115200);   // higher speed is better agaist timeloss for printing comments
  Serial.println("########################################");
  Serial.print("Starting:  ");
  Serial.print(TYPE);
  Serial.print(" v");
  Serial.println(SWVERSION);
    
  TCCR1B = TCCR1B & B11111000 | B00000001;   // Set PWM frequency for D9 & D10 to 16kHz

  pinMode(PRESSURE,INPUT_PULLUP);
  pinMode(ECO,INPUT);
  pinMode(NORM,INPUT);
  pinMode(TURBO,INPUT);
  
  //enables cell balancing and update eeprom registers, but dont write to eeprom
  I2C_8Bit_writeToModule(0x10, 2, 0b00000010);   //IGAIN=10, VGAIN=0.15, 0, 0, 0, 0, HOST=1, SHDN=0 ->switch to host-mode
  if (NUM_OF_CELLS==9){
    I2C_8Bit_writeToModule(0x10, 3, 0b01010011);   //CBAL10=0, CBAL9=1, TOUT=0, BAT=1, PACK=0, IACAL=0, IAEN=1, VAEN=1
    I2C_8Bit_writeToModule(0x10, 4, 0b11111111);   //CellBalance enabled for cells 9...1
  }else{
    I2C_8Bit_writeToModule(0x10, 3, 0b00010011);   //CBAL10=0, CBAL9=0, TOUT=0, BAT=1, PACK=0, IACAL=0, IAEN=1, VAEN=1
    I2C_8Bit_writeToModule(0x10, 4, 0b01111111);   //CellBalance only enabled for cells 7...1
  }
  I2C_8Bit_writeToModule(0x10, 6, 0b00000000);   //OV: 4.15V Hyst0.1 Delay0.5
  I2C_8Bit_writeToModule(0x10, 7, 0b00001111);   //UV: 2.90V Hyst0.2
  I2C_8Bit_writeFlag(0x10, 9, 7, 1);             //CBEN = 1: Automatic cell balancing on
  I2C_8Bit_writeToModule(0x10, 0xb, 0b01100010); //EEPROM read-writing
  /* this is just for test things, dont write to eeprom, its only allowed 3 times!
  Serial.println("EEPROM writing...");
  I2C_8Bit_writeToModule(0x10, 0xb, 0b01000001); //EEPROM writing
  delay(1);
  digitalWrite(BQWRITE,1);                       //EEPROM-Pin HIGH
  delay(100);
  I2C_8Bit_writeToModule(0x10, 0xb, 0b00000000); //EEPROM reading
  digitalWrite(BQWRITE,0);  //EEPROM-Pin LOW
  */
  // (Bosch balancing resistors are 2k. Resistors between 500 â„¦ and 1 kâ„¦ are recommended for effective cell balancing.)

  analogReference(DEFAULT);  // Vref=3.3 V
  delay(25);
  for (int x = 0; x < 10; x++) { IBAT_ZERO = IBAT_ZERO + (analogRead(IBAT));}
  delay(20);
  IBAT_ZERO = (IBAT_ZERO / 10);     //measure offset voltage for IBAT without load, it isnt exactly 1.2V
  Serial.print("IBAT_ZERO=");
  Serial.println(IBAT_ZERO);

  I2C_8Bit_writeFlag(0x10, 2, 1, 0);  //HOST = 0: switch to standalone-mode
  
  digitalWrite(POWER,1);  //switch on power for motors etc
} 



void loop() {

  if ((digitalRead(TURBO)==1)&&(digitalRead(NORM)==1)&&(digitalRead(ECO)==1)){  //switch is in off-position
    t_off++;
    if(t_off>1) {
      poweroff(); //switch off with delay 1.0s (debounce for switch)
      shutdwn();
      t_off=0;
    }
  }
  else{  //switch is in position LOW, MID od HIGH
    if (digitalRead(TURBO)==0) {VAC_PWM_SET=VAC_PWM_HIGH; BRUSH_PWM_SET=BRUSH_PWM_HIGH;} //switch is in position HIGH
    if (digitalRead( NORM)==0) {VAC_PWM_SET=VAC_PWM_MID;  BRUSH_PWM_SET=BRUSH_PWM_MID ;} //switch is in position MID
    if (digitalRead( ECO )==0) {VAC_PWM_SET=VAC_PWM_LOW;  BRUSH_PWM_SET=BRUSH_PWM_LOW ;} //switch is in position LOW
    t_off=0;
    pressmon();
   }

   do{
     if (t_meas++ >10) battmeas();  //measure voltage every 10th cycle (5s)
     if (brush_OC) {BRUSH_PWM_SET=0; BRUSH_PWM_ACT=0;}  //immediately disable brush for the next 5s in overcurrent-case
     battdisplay();
     if (charging) {
      poweroff();       //switch off power for brush and vac while charging for sure if switch was not in off-position
      chargemode=HIGH;  //remember a charging state, then dont switch on again, no matter what switch position
     }
     if (chargemode && !charging && t_meas==0) shutdwn();  //try to shutdown after charging every 5s (not possible while charger is plugged in)

     for (int x=0; x<50; x++) {                              // wait 500ms and do ramp function for VAC and BRUSH motors
      if(VAC_PWM_SET>VAC_PWM_ACT) VAC_PWM_ACT++;
      if(VAC_PWM_SET<VAC_PWM_ACT) VAC_PWM_ACT--;
      if(BRUSH_PWM_SET>BRUSH_PWM_ACT) BRUSH_PWM_ACT++;
      if(BRUSH_PWM_SET<BRUSH_PWM_ACT) BRUSH_PWM_ACT--;
      analogWrite(VAC,VAC_PWM_ACT);
      analogWrite(BRUSH,BRUSH_PWM_ACT);
      delay(10); 
     }

   } while(chargemode); //stay in loop while charging
   
}


//Functions for measurement, display and powermodes

void battmeas(){
  I2C_8Bit_writeToModule(0x10, 2, 0b00000010); //IGAIN=10, VGAIN=0.15, 0, 0, 0, 0, HOST=1, SHDN=0 ->switch to host-mode
  I2C_8Bit_writeToModule(0x10, 1, 0b00000110); //MOSFETs CHG=1, DSG=1                             ->manually switch on mosfets again

  //measure current for charge and brush
  
  analogReference(DEFAULT);  // Vref=3.3 V
  delay(20);   //stabilize voltages

  Serial.println("----- start of cyclic measurement -------");

  I_CHG_BRUSH = IBAT_ZERO - analogRead(IBAT);        //compensate offset voltage
  I_CHG_BRUSH = I_CHG_BRUSH*3.3/1023.0/10.0/0.005;   //IOUT=USENSE/10/RSENSE
  Serial.print("I_CHG_BRUSH=");
  Serial.print(I_CHG_BRUSH,2);

  if (I_CHG_BRUSH > 0.15) charging=HIGH; 
  if (I_CHG_BRUSH < 0.10) charging=LOW;               //Hysteresis

  Serial.print("A -> CHARGE:");
  Serial.println(charging);

  brush_OC = (I_CHG_BRUSH < (BRUSH_I_MAX*-1.0));

   //measure pack voltage

  analogReference(INTERNAL);  // Vref=1.1 V
  delay(5);   //stabilize voltages
  
  BAT = analogRead(VBAT)*1.1/1023.0*50.0;             //VBAT=VOUT*50
  Serial.print("U_PACK = ");
  Serial.print(BAT,2);
  Serial.println("V");

  //measure cells volatges
  //first switch off cell balancing for accurate readings
  I2C_8Bit_writeToModule(0x10, 3, 0b00000001);   //CBAL10=0, CBAL9=0, TOUT=0, BAT=0, PACK=0, IACAL=0, IAEN=0, VAEN=1
  I2C_8Bit_writeToModule(0x10, 4, 0b00000000);   //CBAL8..1 =0

  UCELLMIN=5.0;  //Reset min and max before reading
  UCELLMAX=0.0;

  for (int x = 0; x < NUM_OF_CELLS; x++) {
      I2C_8Bit_writeToModule(0x10, 05, x);   // select CELL_NUMBER x
      delay(5);
      UCELL = ((0.975 - analogRead(VBAT)*1.1/1023.0)/0.15);   //VCell = (0.975-VBAT)/0.15
      Serial.print("Ucell_");Serial.print(x+1);Serial.print("= ");
      Serial.print(UCELL, 3); Serial.println("V");
      if (UCELL < UCELLMIN) {UCELLMIN = UCELL;}
      if (UCELL > UCELLMAX) {UCELLMAX = UCELL;}
    }
  Serial.print("Ucmin= "); Serial.print(UCELLMIN,3);Serial.print("V / Ucmax= "); Serial.print(UCELLMAX,3);Serial.print("V / delta= ");Serial.print(UCELLMAX-UCELLMIN,3);Serial.println("V");
  
  //after reading switch cell balancing on
  if (NUM_OF_CELLS==9){
    I2C_8Bit_writeToModule(0x10, 3, 0b01010011);   //CBAL10=0, CBAL9=1, TOUT=0, BAT=1, PACK=0, IACAL=0, IAEN=1, VAEN=1
    I2C_8Bit_writeToModule(0x10, 4, 0b11111111);   //CellBalance enabled for cells 9...1
  }else{
    I2C_8Bit_writeToModule(0x10, 3, 0b00010011);   //CBAL10=0, CBAL9=0, TOUT=0, BAT=1, PACK=0, IACAL=0, IAEN=1, VAEN=1
    I2C_8Bit_writeToModule(0x10, 4, 0b01111111);   //CellBalance only enabled for cells 7...1
  }

  Serial.println("------ end of cyclic measurement --------");
  Serial.println("");
  
  I2C_8Bit_writeToModule(0x10, 2, 0b11000000);     //HOST = 0: switch to standalone-mode
  t_meas=0;
}


void battdisplay(){     
  if (charging) {  //display blinking led-bar for battery-status depending of battery voltage and charge-levels
    if (BAT>VBAT_CHG_LOW || toogle) digitalWrite(LED_LOW, HIGH); else digitalWrite(LED_LOW, LOW);
    if (BAT>VBAT_CHG_LOW && ( BAT>VBAT_CHG_MID || toogle )) digitalWrite(LED_MID, HIGH); else digitalWrite(LED_MID, LOW); 
    if (BAT>VBAT_CHG_MID && toogle) digitalWrite(LED_HIGH, HIGH); else digitalWrite(LED_HIGH, LOW);   //no upper VBAT, charging ends when current goes low
  }
  else{ //display led-bar for battery-status depending of battery voltage and discharge-levels
    if (BAT>VBAT_HIGH) digitalWrite(LED_HIGH, HIGH); else digitalWrite(LED_HIGH, LOW);
    if (BAT>VBAT_MID) digitalWrite(LED_MID, HIGH); else digitalWrite(LED_MID, LOW);
    if (BAT>VBAT_LOW || toogle) digitalWrite(LED_LOW, HIGH); else digitalWrite(LED_LOW, LOW);
  } 
  toogle=!toogle;
}


void pressmon() {           //switches blue and red leds depending on pressure sensor status or brush-overcurrent
   if (digitalRead(PRESSURE)==0 || brush_OC){ 
      digitalWrite(LED_BLAU, LOW);  //led blue off
      digitalWrite(LED_ROT, HIGH);  //led red  on
   }
   else{ 
      digitalWrite(LED_BLAU, HIGH);  //led blue on
      digitalWrite(LED_ROT, LOW);    //led red  off
   }
}


void poweroff() {         //switch off power for brush and vac and status-leds
  analogWrite(VAC,0); 
  analogWrite(BRUSH,0);
  digitalWrite(POWER,0);
  digitalWrite(LED_BLAU, LOW);
  digitalWrite(LED_ROT, LOW);
}

void shutdwn(){  
  Serial.println("set battery-ic to shutdown-mode (energy saving), goodnight!");
  delay(100); //time to send serial data
  I2C_8Bit_writeToModule(0x10, 2, 0b00000011);
}



//include functions for easy I2C reading and writing (bytes and bits) imported from https://github.com/Wh1teRabbitHU/Arduino-I2C

void I2C_8Bit_writeFlag(uint8_t i2cAddr, uint8_t registerAddr, uint8_t pos, uint8_t value) {
  uint8_t registerValue = I2C_8Bit_readFromModule(i2cAddr, registerAddr);
  registerValue = I2C_8Bit_setBinary(registerValue, pos, value);
  I2C_8Bit_writeToModule(i2cAddr, registerAddr, registerValue);
}

uint8_t I2C_8Bit_setBinary(uint8_t binary, uint8_t pos, uint8_t flagVal) {
  if (flagVal == 1) {
    return binary | (1 << pos);
  }

  return binary & (~(1 << pos));
}

uint8_t I2C_8Bit_readFlag(uint8_t i2cAddr, uint8_t registerAddr, uint8_t pos) {
  uint8_t registerValue = I2C_8Bit_readFromModule(i2cAddr, registerAddr);

  return ((registerValue >> pos) & 1) ;
}

int8_t I2C_8Bit_readFromModule(uint8_t i2cAddr, uint8_t registerAddr) {
  uint8_t byteCount = 1;

  Wire.beginTransmission(i2cAddr);
  Wire.write(registerAddr);
  Wire.endTransmission();
  Wire.requestFrom(i2cAddr, byteCount);

  if (Wire.available()) {
    return Wire.read();
  }

  return 0;
}

void I2C_8Bit_writeToModule(uint8_t i2cAddr, uint8_t registerAddr, uint8_t data) {
  Wire.beginTransmission(i2cAddr);
  Wire.write(registerAddr);
  Wire.write(data);
  Wire.endTransmission();
}