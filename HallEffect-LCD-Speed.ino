/*
 * HallEffect-LCD-Speed
 * by Valentin Medina
 * 
 * This project functionally creates a spedometer, using a
 * Hall effect sensor in a stationary position next to a 
 * wheel with a magnet attached on the rim, being displayed
 * on an LCD screen.
 * 
 * UNO Pin Layout
 *    2 - 5: sensor interrupts
 *    7 - 12: LCD
 * 
 * last update: 12/19/2017
 * 
 */
 
#include <LiquidCrystal.h>

// Initialize LCD pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Constants
const  double RADIUS = 2;                        // inches
const  double CIRCUMFRENCE = 2 * 3.14 * RADIUS;  // inches

// Variables
unsigned long timeold;
volatile byte revolution;
double WheelSpeed;


void setup() {
  // Set baudrate
  Serial.begin(115200);
  
  // Initialize the intterrupt pins  (Pins 2&3 are already INT0&1, Pins 4&5 need extra work)
  int interrupt4 = 4;
  int interrupt5 = 5;
  pinMode(interrupt4, INPUT_PULLUP);
  pinMode(interrupt5, INPUT_PULLUP);
  attachInterrupt(0, magnet_detect2, RISING);                                   
  attachInterrupt(1, magnet_detect3, RISING);                                   
  attachInterrupt(digitalPinToInterrupt(interrupt4), magnet_detect4, RISING);   
  attachInterrupt(digitalPinToInterrupt(interrupt5), magnet_detect5, RISING);   
  
  // Initialize variable data to zero
  timeold = 0;
  revolution = 0;

  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // Print a og message to the LCD
  lcd.print(" ");
  delay(1000);
}

void loop() {
 
  // Handle when there has been a revolution
  if (revolution != 0) {
      // Calculate speed
      WheelSpeed = CIRCUMFRENCE * 1000 / (millis() - timeold); // inches per second
      timeold = millis();
      revolution = 0;
      
      // Send data to serial
      Serial.print("\nWheel speed is ");
      Serial.print(WheelSpeed);
      
      // Send data to LCD
      erase_screen();
      lcd.print("Speed:");
      lcd.print(WheelSpeed);
      lcd.print(" in/s");
   }

}

// Function to clear the screen and replace cursor
void erase_screen (){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);
}

// Function called whenever a magnet/interrupt is detected by sensor
void magnet_detect2(){
   revolution++;
   Serial.println("DETECT!!");
}
void magnet_detect3(){
   //revolution++;
   Serial.println("DETECT!!!");
}
void magnet_detect4(){
   //revolution++;
   Serial.println("DETECT!!!!");
}
void magnet_detect5(){
   //revolution++;
   Serial.println("DETECT!!!!!");
}
