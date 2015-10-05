//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <TrueRandom.h>


int song = 0;
const int silent = 0;
const int BRIGTHNESS= 60;
const int chipSelect = 10;
const int max_puns = 4585;
const int buttonPin = 2;
char message[233];
int previous = 0;
int pos = 0;
const int scrolling_speed = 300;
const int song_temp = 13;
int startup = 1;
unsigned long startMillis;
float v;
int i=0;
int random_nr = 0;
File myFile;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;


//Mario main theme melody
#define melodyPin 3
#define NOTE_E7  2637
#define NOTE_C7  2093
#define NOTE_G7  3136

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
};

int tempo[] = {
  song_temp, song_temp, song_temp, song_temp, 
  song_temp, song_temp, song_temp, song_temp,
  song_temp, song_temp, song_temp, song_temp,
  song_temp, song_temp, song_temp, song_temp, 
};

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void set_random_pun()
{  
  memset(&message[0], 0, sizeof(message));
  startup = 0;
  lcd.clear();
  lcd.setCursor(0, 1);
  pos = 0;
 
  unsigned long currentMillis = millis();
  unsigned long seed = (currentMillis-startMillis);

  lcd.setCursor(0, 1);
  lcd.write("Generating Pun..");
  for (int curser_position=0; curser_position <  16; ++curser_position){
   lcd.setCursor(curser_position, 0);
        lcd.write(1023);   
     delay(80);
        
   }  

  randomSeed(seed);
  random_nr = random(1,max_puns);
  String filename_string =  "pun";
  filename_string += random_nr;
  char filename[8];
  filename_string.toCharArray(filename, 8);
  myFile = SD.open(filename);
      
   if(myFile){
   int j = 0;
   while (myFile.available()) {
     message[j]=myFile.read();
     j++;
  }
  message[j-1] = 0;            
  lcd.clear();
  lcd.setCursor(0, 1);
  }
  else{
    Serial.write("error opening"); 
  }
  myFile.close();
}

void setup()
{
  analogWrite(10, BRIGTHNESS);
  pinMode(buttonPin,INPUT_PULLUP);
  startMillis = millis();
  pinMode(melodyPin, OUTPUT);//buzzer 
  lcd.begin(16, 2);
  lcd.print("Welcome,");
  lcd.setCursor(8, 1);
  lcd.print("Dominik!");
  if (silent != 1)
  sing(1);
  lcd.clear();
  analogWrite(10, BRIGTHNESS);
  if (!SD.begin(chipSelect)) {
   
    return;
  }
  lcd.print("Punmaster 2000");
  lcd.setCursor(0, 1);
  delay(1000);
}
 
 
void printLine(int refreshSeconds){
  if((millis()) % refreshSeconds == 0 && previous != (millis())){
    previous =  (millis());//Store the current time we entered for comparison on the next cycle
    lcd.setCursor(0, 0);//Set our draw position , set second param to 0 to use the top line
    char lcdTop[16];//Create a char array to store the text for the line
    char lcdBottom[16];//Create a char array to store the text for the line
    int copySize = 16; // What is the size of our screen , this could probably be moved outside the loop but its more dynamic like this        
    
    //Store the current position temporarily and invert its sign if its negative since we are going in reverse
    int tempPos = pos;
    if(tempPos < 0)
    {
        tempPos = -(tempPos);
    }
    //Build the lcd text by copying the required text out of our template message variable 
    memcpy(&lcdTop[0],&message[tempPos],copySize);
    memcpy(&lcdBottom[0],&message[tempPos+16],copySize);
    lcd.print(lcdTop);//Print it from position 0
    lcd.setCursor(0, 1);
    lcd.print(lcdBottom);//Print it from position 0
    if (pos == 0 && !startup)
      delay(1500);
    //Increase the current position and check if the position + 16 (screen size) would be larger than the message length , if it is go in reverse by inverting the sign.
    pos += 1;
    if(pos +31 >= strlen(message) && !startup)
    {
      delay(1000);
      pos = 0;
    }
  }
}


void loop()
{ 
  analogWrite(10, BRIGTHNESS);
  if (digitalRead(buttonPin) == LOW ) {
    if (silent != 1){
     tone(melodyPin, 1519, 100);
     delay(150);
     tone(melodyPin, 1700, 100);
     delay(150);
     tone(melodyPin, 1915, 100);
     }
     set_random_pun();
  }
  if(!startup)  
    printLine(scrolling_speed);

}



void sing(int s){      
   // iterate over the notes of the melody:
   song = s;
     Serial.println(" 'Mario Theme'");
     int size = sizeof(melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/tempo[thisNote];

       buzz(melodyPin, melody[thisNote],noteDuration);

       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
       buzz(melodyPin, 0,noteDuration);

    }
}


void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13,HIGH);
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13,LOW);
}

