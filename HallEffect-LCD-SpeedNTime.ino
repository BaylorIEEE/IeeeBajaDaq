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
 *     2 - 5: sensor interrupts
 *         6: stopwatch button
 *    7 - 12: LCD
 * 
 * last update: 1/31/2018
 * 
 */
 
#include <LiquidCrystal.h>


// Initialize LCD pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Constants
const  double RADIUS = 2;                        // inches
const  double CIRCUMFRENCE = 2 * 3.14 * RADIUS;  // inches

// Variables
unsigned long timeold2 = 0;
unsigned long timeold3 = 0;
unsigned long timeold4 = 0;
unsigned long timeold5 = 0;
double WheelSpeed2 = 0;
double WheelSpeed3 = 0;
double WheelSpeed4 = 0;
double WheelSpeed5 = 0;
double long AvgSpeed;
double long stopWatch = 0;
double long stopWatch_begin = 0;


/*
 * Set up Function
 */
void setup() {
  // Set baudrate
  Serial.begin(115200);

  // Initialize button pin
  int buttonPin = 6;
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Initialize the intterrupt pins  (Pins 2&3 are already INT0&1, Pins 4&5 need extra work)
  int interrupt4 = 4;
  int interrupt5 = 5;
  pinMode(interrupt4, INPUT_PULLUP);
  pinMode(interrupt5, INPUT_PULLUP);
  attachInterrupt(0, magnet_detect2, RISING);                                   
  attachInterrupt(1, magnet_detect3, RISING);                                   
  attachInterrupt(digitalPinToInterrupt(interrupt4), magnet_detect4, RISING);   
  attachInterrupt(digitalPinToInterrupt(interrupt5), magnet_detect5, RISING);   

  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  // Print a og message to the LCD
  lcd.print(" ");
  delay(1000);
}


/*
 * Main Loop
 */
void loop() {

  // Calculate speed
  AvgSpeed = (WheelSpeed2+WheelSpeed3+WheelSpeed4+WheelSpeed5)/4    // inches per second

  // Calculate time elapsed
  if( digitalRead(buttonPin) == LOW ){
    stopWatch_begin = millis();
  }
  stopWatch = (millis() - stopWatch_begin) * 1000;
  
  // Send data to serial
  Serial.print("\nWheel speed is ");
  Serial.print(AvgSpeed);
  
  // Send data to LCD
  erase_screen();
  display_screen();
}


/*
 * Functions to display to LCD
 */
void erase_screen(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);
}
void display_screen(){
  lcd.print("Speed:");
  lcd.print(AvgSpeed);  // in/s
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(stopWatch/60);  // min
}


/* 
 *  Function called whenever a magnet/interrupt is detected by sensor
 */
void magnet_detect2(){
   Serial.println("DETECT!2");

   WheelSpeed2 = CIRCUMFRENCE * 1000 / (millis() - timeold2); // inches per second
   timeold2 = millis();
}
void magnet_detect3(){
   Serial.println("DETECT!3");
   
   WheelSpeed3 = CIRCUMFRENCE * 1000 / (millis() - timeold3); // inches per second
   timeold3 = millis();
}
void magnet_detect4(){
   Serial.println("DETECT!4");
   
   WheelSpeed4 = CIRCUMFRENCE * 1000 / (millis() - timeold4); // inches per second
   timeold4 = millis();
}
void magnet_detect5(){
   Serial.println("DETECT!5");
   
   WheelSpeed5 = CIRCUMFRENCE * 1000 / (millis() - timeold5); // inches per second
   timeold5 = millis();
}
