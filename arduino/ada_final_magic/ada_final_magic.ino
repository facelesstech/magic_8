//#include <PCD8544.h>
//static PCD8544 lcd;

#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

#include <EEPROM.h>

#include <Wire.h>
#include "MMA7660.h"
MMA7660 acc;

char *magicTop[]={"As I see it,", "Ask again", "Better not", "Cannot predict", "Concentrate", "Dont count", "It is", "decidedly", "Most", "My reply", "My sources", "Outlook not", "Outlook", "Reply hazy,", "Signs point", "Very", "Without", "Yes", "Yes", "You may rely"};

char *magicBottom[]={"yes", "later", "tell you now", "now","and ask again", "on it", "certain", "so", "likely", "is no", "say no", "so good", "good", "try again", "to yes", "doubtful", "a doubt", " ", "definitely", "on it"};

//char *yesNoPick[]={"yes", "no", "maybe"};
char *yesNoPick[]={"yes", "no"};

char *dicePick[]={"1", "2", "3", "4", "5", "6"};

long randomPick;
long randomPickYesNo;
long randomPickDice;
int menu = 1;
int contrast = 0;
boolean readOnce = 1;
int apps = 0;
int magic = 0;
int yesNo = 0;
int dice = 0;

//short level = 45;
short level = 25;

// Button stuff
const int button = 10; // Button set to pin 5
long time_button = 0; // the last time the output pin was toggled
long debounce = 50; // the debounce time, increase if the output flickers
int reading_button; // the current reading from the input pin
int previous_button = HIGH; // the previous reading from the input pin

// eeprom stuff
int emptyGlass;
int fullGlass;
int addrLow = 0;
int addrHigh = 1;

void setup() {
  display.begin();
  // Add the smiley to position "0" of the ASCII table...
  acc.init();
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(button, INPUT); // Set the button as input
  digitalWrite(button, HIGH); // initiate the internal pull up resistor
  display.setRotation(2);

}


void loop() {
    static long cnt     = 0;
    static long cntout  = 0;
    float ax,ay,az;
    int8_t x, y, z;

    acc.getXYZ(&x,&y,&z);

    if(acc.getAcceleration(&ax,&ay,&az))
    {
    }
    else
    {
    }

  // Use a potentiometer to set the LCD contrast...
//   short level = map(analogRead(A0), 0, 1023, 0, 127);
//  lcd.setContrast(level);
//  display.setContrast(35);
  display.setContrast(level);
    
//    Serial.println("accleration of X/Y/Z: ");
//    Serial.print(ax);
//    Serial.print("x");
//    Serial.println(x);
//    Serial.println(" g");
    Serial.print("y");
    Serial.println(y);
//    Serial.println(" g");
//    Serial.print("z");
//    Serial.println(z);
//    Serial.println(" g");
//    Serial.println();

//  lcd.setCursor(0, 0);
//  lcd.print(x);
//
//  // Write the counter on the second line...
//  lcd.setCursor(0, 1);
//  lcd.print(ax);



  if (readOnce == 1)
  {

    level = EEPROM.read(addrHigh);
    Serial.print("Level");
    Serial.println(level);

    readOnce = 0;
  }


  if (menu == 1)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println("Magic 8");
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(12,30);
    display.write(27); // Left arrow
    display.println("Contrast");
    display.setCursor(25,20);
    display.write(24); // Up arrow
    display.println("Apps");
    display.display();

//    if (x <= -20)
    if (x >= 20)
    {
      menu = 0;
      contrast = 1;
    }

//    else if (y <= -20)
    else if (y >= 20)
    {
      display.clearDisplay();
      menu = 0;
      apps = 1;
    }
  }



//  else if (apps == 1)
  if (apps == 1)
  {
      display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("Apps");
    display.setTextSize(1);
    display.setCursor(0,20);
    display.write(27); // Left arrow
    display.println("Magic 8 ball");
    display.setCursor(78,30);
    display.write(26); // Right arrow
    display.setCursor(16,30);
    display.println(" Yes or No");
    display.setCursor(25,40);
    display.write(25); // Down arrow
    display.println("Dice");
    display.display();


    if (x >= 20)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Magic 8");
      display.display();
      apps = 0;
      magic = 1;
    }

    else if (x <= -20)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Yes No");
      display.display();

      apps = 0;
      yesNo = 1;
    }

    else if (y <= -25)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Dice");
      display.display();

      apps = 0;
      dice = 1;
    }
  }

  else if (dice == 1)
  {
      if (ax >= 1)
      {
        randomPickDice = random(sizeof(dicePick)/sizeof(char*));
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0,0);
        display.println("Dice");
        display.setCursor(0,30);
        display.setTextSize(1);
        display.println(dicePick[randomPickDice]);
        display.display();
      }
  }

  else if (yesNo == 1)
  {
      if (ax >= 1)
      {
        randomPickYesNo = random(sizeof(yesNoPick)/sizeof(char*));
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0,0);
        display.println("Yes No");
        display.setTextSize(1);
        display.setCursor(0,20);
        display.println(yesNoPick[randomPickYesNo]);
        display.display();
      }
  }

  else if (magic == 1)
  {

    if (ax >= 1)
    {
      randomPick = random(sizeof(magicTop)/sizeof(char*));
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0,0);
      display.println("Magic 8");
      display.setTextSize(1);
      display.setCursor(0,20);
      display.println(magicTop[randomPick]);
      display.setCursor(0,30);
      display.println(magicBottom[randomPick]);
      display.display();
    }
  }

  else if (contrast == 1)
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("contrast");
    display.setCursor(0,10);
    display.println(level);
    display.display();

   // ------------- Debound code button green start code -------------
    reading_button = digitalRead(button);

    if (reading_button == HIGH && previous_button == LOW && millis() - time_button > debounce) { 
      time_button = millis(); 
      // Do something here
      Serial.println("button press");
      menu = 1;
      contrast = 0;
      EEPROM.write(addrHigh, level);
    }
    previous_button = reading_button;
   
   // -------------- Debound code button green end code -------------

//    if (x <= -20)
    if (x >= 20)
    {
      level--;
      display.clearDisplay();
      display.setCursor(0,10);
      display.println(level);
      display.setCursor(0,20);
      display.println("down");
      display.setContrast(level);
      display.display();
      delay(500);
    }

//    else if(x >= 20)

    else if(x <= -20)
    {
      level++;
      display.clearDisplay();
      display.setCursor(0,10);
      display.println(level);
      display.setCursor(0,20);
      display.println("up");
      display.setContrast(level);
      display.display();
      delay(500);
    }
  }
}
