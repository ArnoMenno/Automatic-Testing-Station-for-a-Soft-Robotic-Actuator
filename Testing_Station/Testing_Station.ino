/* Testing Station - Software Implementation 
   AM, van der Vinne */

// Include the following libraries

#include "HX711.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

//Constants

const int FlexSensor_Pin = A0; // Analog pin 0 connected to flex sensor 
const float Vcc = 5; // 5 Volt input 
const float Resistor = 10000; // Resistor used to create voltage devider circuit
const float Flat_Resistance = 23800; // Resistance when flex sensor is at 0 degrees
const float Bend_Resistance = 40000; // Resistance when flex sensor is at 90 degrees
const int LoadCell_DAT_Pin = A2; // Analog pin 2 connected to load cell DAT pin
const int LoadCell_CLK_Pin = A3; // Analog pin 3 connected to load cell CLK pin
const int PressureSensor_Pin = A1; // Analog pin 1 connected to pressure sensor pin
const float V_Offset = Vcc / 2.01779;  // Offset voltage at 0 kPa
const float Sensitivity = Vcc / 2000; // 2.5 mV/kPa for the -100 kPa to 100 kPa range

//Variables

int Flex_ADC; // Analog value for flex sensor
float Flex_Voltage; // Voltage division value
float Flex_Resistance; // Resistance value
float Flex_Angle; // Bending angle of flex sensor in degrees
float Force_Measurement; // Force measurement of load cell
int Pressure_ADC; // Analog value for pressure sensor
float Pressure_Voltage; //Voltage division value
float Pressure; // Pressure measurement value


LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Set the LCD pins
HX711 scale;

void setup() {

  //LCD
  lcd.begin(16, 2); // Set LCD's columns and rows
  lcd.setCursor(0,0); // Set LCD cursor to first row and first column
  
  // Flex sensor
  pinMode(FlexSensor_Pin, INPUT); // Set pin A0 as an input

  // Load cell
  scale.begin(LoadCell_DAT_Pin, LoadCell_CLK_Pin); // Begin the scale         
  scale.set_scale(47.1); // this value is the calibration factor                     
  scale.tare(); // reset the scale to 0
}

void loop() {

 // Clear the LCD screen at the beginning of each loop
 lcd.clear();

 // Load cell code
 Force_Measurement = ((scale.get_units())/1000) *9.81; // Convert weight in grams to force in Newtons

 lcd.setCursor(0, 0);
 lcd.print("F:"); // Print force heading
 lcd.print(Force_Measurement, 2); // Print force measurement
 lcd.print("N"); // Print Newtons unit
 

 // Flex sensor code
 Flex_ADC = analogRead(FlexSensor_Pin); // Get ADC value
 Flex_Voltage = Flex_ADC * (Vcc / 1023); // Convert ADC value to voltage
 Flex_Resistance = Resistor * (Vcc / Flex_Voltage - 1); // Calculate the flex resistance
 Flex_Angle = map(Flex_Resistance, Flat_Resistance, Bend_Resistance, 0, 90); // Convert resistance into bending angle

 lcd.setCursor(9, 0);
 lcd.print(" B:"); // Print bending angle heading
 lcd.print(Flex_Angle, 0); // Print bending angle in degrees
 lcd.print("D"); // Print degrees unit
 
 // Pressure sensor code
 Pressure_ADC = analogRead(PressureSensor_Pin); // Get ADC value
 Pressure_Voltage = Pressure_ADC * (Vcc/ 1023); // Convert ADC value to voltage
 Pressure = (Pressure_Voltage - V_Offset) / Sensitivity; // Pressure value in kPa
 
 lcd.setCursor(0,1); // Set cursor to second row
 lcd.print("P:"); // Print pressure heading
 lcd.print(Pressure, 3); // Print pressure measurement
 lcd.print("kPa"); // Print kilo Pascal unit
 
 lcd.setCursor(0,0); // Set cursor back to force measurement position

 // Delay for a short period to control the display update rate
 delay(100);
}






