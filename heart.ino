#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

float avg = 540;
float reading = 0;

int average = avg;
int actual = reading;

float BPM=60;
float newBPM=60;

long oldTime=0;
bool pulseStart;



#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(60,15);
  display.println("Hoi!");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  reading = 0.2*analogRead(A0)+0.8*reading;  //ruisonderdrukking door signaal steeds met 20% aan te passen
  avg = 0.005*reading+0.995*avg;             //ruisonderdrukking door signaal steeds met 0,5% aan te passen
  if ((reading > avg + 2) & !pulseStart) {   //sla alleen start tijd op bij begin van de puls
    digitalWrite(LED_BUILTIN, LOW);          //ledje aan bij gedetecteerde puls
    pulseStart = 1;
    newBPM=(60000/(millis()-oldTime));
    if (newBPM>1.2*BPM){
      newBPM=1.2*BPM;
    }
    if (newBPM<0.8*BPM){
      newBPM=0.8*BPM;
    }
    BPM=0.2*newBPM+0.8*BPM; //ruisonderdrukking door signaal steeds met 20% aan te passen

    display.clearDisplay();
    display.setCursor(50,15);
    display.println(int(BPM));
    display.display();
    
    oldTime=millis();    

  } else if (reading <= avg) {
    digitalWrite(LED_BUILTIN, HIGH);       //ledje uit aan eind van de puls
    pulseStart = 0;                        //reset the puls zodat bij nieuwe puls de start weer gedetecteerd kan worden
  }
  
  actual = reading;
  if (actual > avg+10){                   //waardes een beetje binnen de perken houden ivm serial plotter
    actual = avg+10;
  }
  if (actual < avg -10){
    actual = avg -10;
  }
  average = avg;
  
//  Serial.print(int(10*BPM));            //waardes naar de serial plotter sturen
//  Serial.print(",");
  Serial.print(actual);
  Serial.print(",");
  Serial.println(average);
  
  delay(5);
}
