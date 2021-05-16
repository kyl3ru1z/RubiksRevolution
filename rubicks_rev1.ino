#include "pitches.h"

const int SPEAKER = A1;

byte LIGHT[] = {3, 5, 7, 9, 11, 13};
byte BUTTON[] = {2, 4, 6, 8, 10, 12};

int buttonState0;
int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;
int buttonState5;

int flash;
int pastFlashValue; 
int score = 0;
int correctButton;
bool timeLose = false;
int ranTone;

unsigned long startTime;
unsigned long endTime = 62000;
unsigned long timeShrink = 2000;
unsigned long delayTime = 195;

// Tones 
int introNotes[] = 
{
  NOTE_D4, NOTE_D4, NOTE_E5, NOTE_G6, NOTE_E5, NOTE_A6, 0, NOTE_B5,
  0, NOTE_C7, NOTE_B5, NOTE_G5, NOTE_D3, NOTE_E4, 0, NOTE_D4, 0
};

int introDuration[] = 
{
  200, 200, 200, 200, 200, 200, 400, 200, 
  200, 200, 200, 200, 200, 200, 400, 200, 
};

int lostWoodsNotes[] = 
{
  NOTE_F4, NOTE_A3, NOTE_B4, NOTE_F4, NOTE_A3, NOTE_B4, 
  NOTE_F4, NOTE_A3, NOTE_B4, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_D5, 
  NOTE_B4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, NOTE_G4, NOTE_E4, 0
};

int lostWoodsNoteDuration[] = 
{
  200, 200, 400, 200, 200, 400,
  200, 200, 200, 200, 400, 200, 200, 
  200, 200, 1000, 200,
  200, 200, 1200,
  
};

void setup() 
{
  randomSeed(analogRead(0)); 
  Serial.begin(9600);
              
  for (byte i = 0; i < 7; i++) // Initilizing LED
  {
    pinMode(LIGHT[i], OUTPUT);
  }

  lightState(true);
  
  ranTone = random(2);
  switch(ranTone) 
  {
    case 0:
      for (int i = 0; i < 17; i++) // Plays Intro melody
      {
        tone(SPEAKER, introNotes[i], introDuration[i]);
        if (i == 16) // stops melody from a long pause at the end.
        {
          noTone(SPEAKER);
          break;
        } 
        delay(introDuration[i]);
      }
      break;
    case 1:
      for (int i = 0; i < 21; i++)
      {
        tone(SPEAKER, lostWoodsNotes[i], lostWoodsNoteDuration[i]);
        if (i == 20) 
        {
          noTone(SPEAKER); 
          break;
        }
        delay(lostWoodsNoteDuration[i]);
      }
      break;  
  }


  resetButtonStates();

  while (buttonState0 == LOW && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW && buttonState5 == LOW) 
  {
    readButtonStates();
  }

  playCorrectButtonTone();
  lightState(false);
  delay(500);
}

void loop() 
{
  flash = random(6);
  while (flash == pastFlashValue) // keep re-rolling until a different flash is chosen.
  {
    flash = random(6);
  }
  pastFlashValue = flash; // storing flash value for re-rolling *changed recently*
  
  switch(flash) 
  {
    // turn on the specific light and assigns which is correct button to push
    case 0:
      digitalWrite(LIGHT[0], true);
      correctButton = BUTTON[0];  
      break;
    case 1:
      digitalWrite(LIGHT[1], true);
      correctButton = BUTTON[1];  
      break;
    case 2:
      digitalWrite(LIGHT[2], true);
      correctButton = BUTTON[2]; 
      break;
    case 3:
      digitalWrite(LIGHT[3], true);
      correctButton = BUTTON[3];  
      break;
    case 4:
      digitalWrite(LIGHT[4], true);
      correctButton = BUTTON[4];  
      break;
    case 5:
      digitalWrite(LIGHT[5], true);
      correctButton = BUTTON[5];  
      break;
  } 
  
  startTime = millis(); // timer has started after lights come on
  resetButtonStates();
  
  if (delayTime == 125) // Limits clock ticking progression
  {
    delayTime = 125;
  }
  else 
  {
    delayTime -= 5;
  }
 
  while (buttonState0 == LOW && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW && buttonState5 == LOW) // while button has not been pushed
  {
    tone(SPEAKER,1100,5); // Clock ticking 
    delay(delayTime);
    
    readButtonStates();

    if (millis() - startTime >= endTime) // if the user does not react to the button fast enough they lose
    {
      playWrongButtonTone();
      timeLose = true;
      giveScoreRating(score);
      youLose();
    }
  }
  
  delay(200); // debouncing 

  int buttonPressed = 0; // Clearing previous button value
  if (buttonState0 == HIGH) // If button was pushed assign it.
  {
    buttonPressed = BUTTON[0];
  }
  else if (buttonState1 == HIGH)
  {
    buttonPressed = BUTTON[1];
  }
  else if (buttonState2 == HIGH)
  {
    buttonPressed = BUTTON[2];
  }
  else if (buttonState3 == HIGH)
  {
    buttonPressed = BUTTON[3];
  }
  else if (buttonState4 == HIGH)
  {
    buttonPressed = BUTTON[4];
  }
  else if (buttonState5 == HIGH)
  {
    buttonPressed = BUTTON[5];
  }

  if (buttonPressed == correctButton && timeLose == false) 
  {
    score += 1;
    playCorrectButtonTone();

    if (endTime <= 4000) // Limits reaction time decrease
    {
      endTime = 1000; 
    }
    else 
    {
      endTime = endTime - timeShrink; // the time needed to press the next button will shrink
    }
    
    lightState(false);
  }
  else if (buttonPressed != correctButton && timeLose == false)
  {
    playWrongButtonTone();
    giveScoreRating(score);
    youLose();
  }
  timeLose = false;
}

void youLose () 
{ 
    resetButtonStates();
   
    while (buttonState0 == LOW && buttonState1 == LOW && buttonState2 == LOW && buttonState3 == LOW && buttonState4 == LOW && buttonState5 == LOW) // Since I'm waiting for a button push anyway just use button push
    { 
        lightState(true);
        readButtonStates();
        delay(50); // debouncing 
    }
    
    playCorrectButtonTone();
    softReset();
}

void softReset() 
{
  lightState(false);
  delayTime = 195;
  endTime = 62000;
  pastFlashValue = 0;
  score = 0;
  delay(500);
}

void giveScoreRating(int score) 
{
  int numBeeps = 0;
  if (score <= 10) 
  {
    numBeeps = 1;
  }
  else if (score <= 20) 
  {
    numBeeps = 2;
  }
  else if (score <= 30)
  {
    numBeeps = 3;
  }
  else if (score <= 40)
  {
    numBeeps = 4;
  }
  else if (score > 40)
  {
    numBeeps = 5;
  }

  for (byte i = 0; i < numBeeps; i++) 
  {
    tone(SPEAKER,NOTE_E5);
    delay(250);
    noTone(SPEAKER);
    delay(100);
  }
  noTone(SPEAKER);
}

void lightState (bool state) 
{
  for (byte i = 0; i < 7; i++) 
  {
    digitalWrite(LIGHT[i], state);
  }
}

void resetButtonStates() 
{
  buttonState0 = LOW;
  buttonState1 = LOW;
  buttonState2 = LOW;
  buttonState3 = LOW;
  buttonState4 = LOW;
  buttonState5 = LOW;
}

void readButtonStates()
{
  buttonState0 = digitalRead(BUTTON[0]);  
  buttonState1 = digitalRead(BUTTON[1]);
  buttonState2 = digitalRead(BUTTON[2]);
  buttonState3 = digitalRead(BUTTON[3]);  
  buttonState4 = digitalRead(BUTTON[4]);
  buttonState5 = digitalRead(BUTTON[5]);
}

void playCorrectButtonTone() 
{
  tone(SPEAKER,NOTE_A5);
  delay(70);
  tone(SPEAKER,NOTE_B5);
  delay(70);
  tone(SPEAKER,NOTE_C5);
  delay(70);
  tone(SPEAKER,NOTE_B5);
  delay(70);
  tone(SPEAKER,NOTE_C5);
  delay(70);
  tone(SPEAKER,NOTE_D5);
  delay(70);
  tone(SPEAKER,NOTE_E5);
  delay(250);
  noTone(SPEAKER);
}

void playWrongButtonTone()
{
  tone(SPEAKER,NOTE_G4);
  delay(250);
  tone(SPEAKER,NOTE_C4);
  delay(500);
  noTone(SPEAKER);
}
