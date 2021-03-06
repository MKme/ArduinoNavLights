/**
 Erics DIY Aircraft Navigation Lights Project
My Youtube Channel  : http://www.youtube.com/mkmeorg
Website http://mkme.org


_________________--------______________--------------________________----------------__________
 Update for Disgispark -  ERICS NOTEZ
 
 Ref Material on pins https://digistump.com/board/index.php?topic=142.0
 Install board manager in preferences in IDE: https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json
 Install drivers package: https://github.com/digistump/DigistumpArduino/releases
 Change programmer in IDE to "USBTinyISP"
 Hit program (dont plug in board untill te text asks you to in IDE dialog box
 Profit...
Works Aug 2019 on clone ebay digispark boards

  * March 2020 Stripped ou servo code and fixed pin assignments/notes.  Working on Nano talon
 * 
 

*/

// Landing light settings
#define LL_IRQ_NUMBER 0 // Interrupt number to use (0 = pin 2 on most boards)
#define LL_PIN_SERVO 2 // Servo input pin number - this needs to match whatever interrupt is used
#define LL_PIN_LIGHT 3 // Landing light output pin number
#define LL_SERVO_THRESHOLD 1500 // Servo signal threshold to turn on/off landing light (pulse width in microseconds, 1000 to 2000)
#define LL_SERVO_DEAD_ZONE 100 // Servo signal dead-zone size, eliminates flicker
#define LL_SERVO_REVERSED true   // Whether or not the servo channel is reversed

// Strobe settings
#define STB_PIN_LIGHT 4 // Pin number for strobe light output   // ------------------------------------------------------------------------Eric Note WORKS and labelled Pin P4 on digispark!
#define STB_PIN_LIGHTB 8 // Pin number for strobe light output
#define STB_BLINK_INTERVAL 2000000 // Blink interval for strobe light in microseconds

// Anti-collision beacon settings
#define ACB1_PIN_LIGHT 1 // Pin number for anti-collision beacon 1  //--------------------------------------------------- Eric Note on Digispark 1 is internal LED but labelled P2!!!! 
#define ACB2_PIN_LIGHT 6 // Pin number for anti-collision beacon 2
#define ACB_FADE_MIN 0 // Minimum fade level for beacon (0-255)
#define ACB_FADE_MAX 255 // Maximum fade level for beacon (0-255)
#define ACB_FADE_INTERVAL 5000 // Fade step interval, in microseconds (lower numbers = faster fade)



unsigned long lastFadeTime = 0;
unsigned long lastStrobeTime = 0;
int currentFade = ACB_FADE_MIN;
int fadeDirection = 1;

// Called on power on or reset
void setup()
{


  // Declare output pins
  pinMode(LL_PIN_LIGHT, OUTPUT);
  pinMode(STB_PIN_LIGHT, OUTPUT);
  pinMode(STB_PIN_LIGHTB, OUTPUT);
  pinMode(ACB1_PIN_LIGHT, OUTPUT);
  pinMode(ACB2_PIN_LIGHT, OUTPUT);
  
  
}

// Called continuouly
void loop()
{
  unsigned long currentTime = micros();

  
  // Check if it's time to fade the anti-collision lights
  if ((currentTime - lastFadeTime) > ACB_FADE_INTERVAL) {
    doFade();
    lastFadeTime = currentTime;
  }

  // Check if it's time to blink the strobes
  if ((currentTime - lastStrobeTime) > STB_BLINK_INTERVAL) {
    doStrobe();
    lastStrobeTime = currentTime; 
  }
}





// Fade anti-collision LEDs
void doFade()
{
  currentFade += fadeDirection;
  if (currentFade == ACB_FADE_MAX || currentFade == ACB_FADE_MIN) {
    // If we hit the fade limit, flash the high beacon, and flip the fade direction
    if (fadeDirection == 1) {
      analogWrite(ACB1_PIN_LIGHT, 255);

    } else {
      analogWrite(ACB2_PIN_LIGHT, 255);
    }
    delay(50); 
    fadeDirection *= -1; 
  }

  analogWrite(ACB1_PIN_LIGHT, currentFade);
  analogWrite(ACB2_PIN_LIGHT, ACB_FADE_MAX - currentFade + ACB_FADE_MIN);
}

// Strobe double-blink
void doStrobe()
{
  digitalWrite(STB_PIN_LIGHT, HIGH);
  digitalWrite(STB_PIN_LIGHTB, HIGH);
  delay(50);
  digitalWrite(STB_PIN_LIGHT, LOW);
  digitalWrite(STB_PIN_LIGHTB, LOW);
  delay(50);
  digitalWrite(STB_PIN_LIGHT, HIGH);
  digitalWrite(STB_PIN_LIGHTB, HIGH);
  delay(50);
  digitalWrite(STB_PIN_LIGHT, LOW);
  digitalWrite(STB_PIN_LIGHTB, LOW);
}
