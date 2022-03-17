#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <DHT.h>
 
BH1750 lightMeter;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) SDA = D2; SCL = D1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 14     // Digital pin connected to the DHT sensor as D5
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Its works for my 128x64 OLED 0,96" screen
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  Wire.begin();
  dht.begin();
  
  lightMeter.begin();
  lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE_2);
  

  display.setTextSize(1); //1-8; 1 is default, not needed
  display.setTextColor(WHITE); // Not relevant for one or two color OLED display but needed for the code
  display.setCursor(0,0); //0,0 to 120,57
  display.println("BH1750 test!");
  delay(2000);
  display.display();
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Fahrenheit (isFahrenheit = true; default C as false)
  float l = lightMeter.readLightLevel();
  
  //Convert to string and cut de second decimal numbers
  String temp = String(t,1); 
  String humi = String(h,1);
  String lux  = String(l,1);
  
  // Calculate lux position according to its number length
  int pad = (lux.length() * 6) + 19;

  // clear the display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setCursor(85,0);
  display.print(temp);
  display.drawCircle(115, 1, 1, WHITE); // degree sign
  display.setCursor(118,0); // repositioning the Celsius display
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("Humidity:   ");
  display.setCursor(85,10);
  display.print(humi);
  display.print(" ");
  display.print("%"); 

  // display light data
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Illuminance: ");
  display.setCursor((SCREEN_WIDTH - pad),20); 
  display.print(lux);
  display.print(" lx");
  
  display.display();
  delay(2000);
}
