#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 1
#define FONA_TX 0
#define FONA_RST 4
#define LED_PIN 13

// this is a large buffer for replies
char replybuffer[255];

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void modKeyPress(uint8_t mod, uint8_t key);
String getInstruction(String msg, int index);
uint8_t extendedCommands(String key);
uint8_t translateKey(String key);
uint8_t translateMod(String mod);
void mouseControl(String msg);
void parse(String msg);

void setup()
{
  Serial1.begin(115200);
  if (!fona.begin(Serial1))
  {
    while(1);
  }

  // Check we have a sim card by IMEI
  char imei[15] = {0};
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0)
  {
    //Unlock the sim if defined
#if defined(SIM_PIN)
    fona.unlockSIM(SIM_PIN);
#endif
  }
  else
  {
    // No IMEI? No SIM? lock up here.
    while(1);
  }

  //Turn our LED on if we've defined a pin to show we're good to go.
#if defined(LED_PIN)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
#endif
}

//Loop is simply reading, parsing, and deleting all sms
//Using the 'R' case from adafruit's FonaTEST sketch
void loop()
{
  int8_t smsnum = fona.getNumSMS();
  uint16_t smslen;
  for (int8_t smsn=1; smsn<=smsnum; smsn++)
  {
    if (!fona.readSMS(smsn, replybuffer, 250, &smslen)) break;
    // if the length is zero, its a special case where the index number is higher
    // so increase the max we'll look at!
    if (smslen == 0)
    {
      smsnum++;
      continue;
    }
    parse(replybuffer);
    fona.deleteSMS(smsn);
  }
  delay(200);
}

void modKeyPress(uint8_t mod, uint8_t key)
{
  // Press and hold our modifier
  Keyboard.set_modifier(mod);
  Keyboard.send_now();
  delay(100);
  Keyboard.set_key1(key);
  Keyboard.send_now();
  delay(200);

  // Release everything
  Keyboard.set_modifier(0);
  Keyboard.set_key1(0);
  Keyboard.send_now();
  //delay(100);
}

// Simple inefficent tokenisation of a String
String getInstruction(String msg, int index)
{
  int count = 0, i = 0, j = 0;
  while(msg[i] == ' ') ++i;
  for (; i < msg.length(); ++i)
  {
    if(msg[i] == ' ')
    {
      if(count++ == index)
      {
        return msg.substring(j,i);
      }
      while(msg[i] == ' ') ++i;
      j = i;
    }
  }
  if(j == 0 && count == 0) return msg;
  else if(count == index) return msg.substring(j,i);
  return "";
}

// Translation of extended commands from Duckyscript ref. https://www.pjrc.com/teensy/td_keyboard.html
uint8_t extendedCommands(String key)
{
  if      (key == "ENTER") return KEY_ENTER;
  else if (key == "ESC" || key == "ESCAPE") return KEY_ESC;
  else if (key == "BACKSPACE") return KEY_BACKSPACE;
  else if (key == "TAB") return KEY_TAB;
  else if (key == "SPACE") return KEY_SPACE;
  else if (key == "MENU" || key == "APP") return KEY_MENU;
  else if (key == "CAPS_LOCK") return KEY_CAPS_LOCK;
  else if (key == "F1") return KEY_F1;
  else if (key == "F2") return KEY_F2;
  else if (key == "F3") return KEY_F3;
  else if (key == "F4") return KEY_F4;
  else if (key == "F5") return KEY_F5;
  else if (key == "F6") return KEY_F6;
  else if (key == "F7") return KEY_F7;
  else if (key == "F8") return KEY_F8;
  else if (key == "F9") return KEY_F9;
  else if (key == "F10") return KEY_F10;
  else if (key == "F11") return KEY_F11;
  else if (key == "F12") return KEY_F12;
  else if (key == "PRINTSCREEN") return KEY_PRINTSCREEN;
  else if (key == "SCROLLLOCK") return KEY_SCROLL_LOCK;
  else if (key == "PAUSE" || key == "BREAK") return KEY_PAUSE;
  else if (key == "INSERT") return KEY_INSERT;
  else if (key == "HOME") return KEY_HOME;
  else if (key == "PAGE_UP") return KEY_PAGE_UP;
  else if (key == "DELETE") return KEY_DELETE;
  else if (key == "END") return KEY_END;
  else if (key == "PAGE_DOWN") return KEY_PAGE_DOWN;
  else if (key == "RIGHT" || key == "RIGHTARROW" ) return KEY_RIGHT;
  else if (key == "LEFT" || key == "LEFTARROW") return KEY_LEFT;
  else if (key == "DOWN" || key == "DOWNARROW") return KEY_DOWN;
  else if (key == "UP" || key == "UPARROW") return KEY_UP;
  else if (key == "NUM_LOCK") return KEY_NUM_LOCK;
  else return 0;
}

// Translation of keys to their numerical values ref. https://www.pjrc.com/teensy/td_keyboard.html
uint8_t translateKey(String key)
{
  if (uint8_t val = extendedCommands(key)) return val;
  else if (key == "MINUS") return KEY_MINUS;
  else if (key == "EQUAL") return KEY_EQUAL;
  else if (key == "LEFT_BRACE") return KEY_LEFT_BRACE;
  else if (key == "RIGHT_BRACE") return KEY_RIGHT_BRACE;
  else if (key == "BACKSLASH") return KEY_BACKSLASH;
  else if (key == "SEMICOLON") return KEY_SEMICOLON;
  else if (key == "QUOTE") return KEY_QUOTE;
  else if (key == "TILDE") return KEY_TILDE;
  else if (key == "COMMA") return KEY_COMMA;
  else if (key == "PERIOD") return KEY_PERIOD;
  else if (key == "SLASH") return KEY_SLASH;
  else
  {
    switch(toupper(key[0]))
    {
      case 'A': return KEY_A;
      case 'B': return KEY_B;
      case 'C': return KEY_C;
      case 'D': return KEY_D;
      case 'E': return KEY_E;
      case 'F': return KEY_F;
      case 'G': return KEY_G;
      case 'H': return KEY_H;
      case 'I': return KEY_I;
      case 'J': return KEY_J;
      case 'K': return KEY_K;
      case 'L': return KEY_L;
      case 'M': return KEY_M;
      case 'N': return KEY_N;
      case 'O': return KEY_O;
      case 'P': return KEY_P;
      case 'Q': return KEY_Q;
      case 'R': return KEY_R;
      case 'S': return KEY_S;
      case 'T': return KEY_T;
      case 'U': return KEY_U;
      case 'V': return KEY_V;
      case 'W': return KEY_W;
      case 'X': return KEY_X;
      case 'Y': return KEY_Y;
      case 'Z': return KEY_Z;
      case '1': return KEY_1;
      case '2': return KEY_2;
      case '3': return KEY_3;
      case '4': return KEY_4;
      case '5': return KEY_5;
      case '6': return KEY_6;
      case '7': return KEY_7;
      case '8': return KEY_8;
      case '9': return KEY_9;
      case '0': return KEY_0;
    }
  }
  return 0;
}

// Translation of modifier keys ref. https://www.pjrc.com/teensy/td_keyboard.html
uint8_t translateMod(String mod)
{
  if(mod == "CONTROL" || mod == "CTRL") return MODIFIERKEY_CTRL;
  else if(mod == "GUI" || mod == "WINDOWS") return MODIFIERKEY_GUI;
  else if(mod == "ALT") return MODIFIERKEY_ALT;
  else if(mod == "SHIFT") return MODIFIERKEY_SHIFT;
  return 0;
}

void mouseControl(String msg)
{
  String instruction = getInstruction(msg, 1);
  //Click a button
  if(instruction == "CLICK")
  {
    instruction = getInstruction(msg, 2);
    if(instruction == "MOUSE_RIGHT") Mouse.click(MOUSE_RIGHT);
    else if(instruction == "MOUSE_MIDDLE") Mouse.click(MOUSE_MIDDLE);
    else Mouse.click(MOUSE_LEFT);
  }
  // Move our mouse, takes a comma seperated X,Y offset
  else if(instruction == "MOVE")
  {
    int8_t x = 0, y = 0;
    instruction = getInstruction(msg, 2);
    for (int i = 0; i < instruction.length(); ++i)
    {
      if(instruction[i] == ',')
      {
         x = instruction.substring(0,i).toInt();
         y = instruction.substring(i+1,instruction.length()).toInt();
      }
    }
    if(x != 0 || y != 0) Mouse.move(x,y);
  }
  // Press/release toggle. Allows dragging when combined with move
  else if(instruction == "PRESS")
  {
    uint8_t button = MOUSE_LEFT;
    instruction = getInstruction(msg, 2);
    if(instruction == "MOUSE_RIGHT") button = MOUSE_RIGHT;
    else if(instruction == "MOUSE_MIDDLE") button = MOUSE_MIDDLE;
    (Mouse.isPressed(button)) ? Mouse.release(button) : Mouse.press(button);
  }
}

/*
  Based on Duckyscript with the addition of mouse movement allowed by the Teensy
  Should support all commands listed here: https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript
  With the exception of DEFAULT_DELAY

  Extra 'MOUSE' command allows clicking/toggle of buttons and pointer movement
  * MOUSE CLICK (MOUSE_LEFT|MOUSE_MIDDLE|MOUSE_RIGHT)
  * MOUSE PRESS (MOUSE_LEFT|MOUSE_MIDDLE|MOUSE_RIGHT)
  * MOUSE MOVE X,Y
*/
void parse(String msg)
{
  String instruction = getInstruction(msg, 0);
  uint8_t key = 0;
  if(instruction == "") return;
  else if(instruction == "STRING") Keyboard.print(msg.substring(instruction.length()+1,msg.length()));
  else if(instruction == "ENTER") Keyboard.println();
  else if((key = extendedCommands(instruction))) modKeyPress(0,key);
  else if((key = translateMod(instruction)))
  {
    uint8_t currkey = 0, i = 1;
    instruction = getInstruction(msg, i);
    while(instruction != "")
    {
      if((currkey = translateMod(instruction)) != 0)
      {
        key |= currkey;
      }
      else
      {
        modKeyPress(key, translateKey(instruction));
        return;
      }
      instruction = getInstruction(msg, ++i);
    }
    // Should only get here if we're being funny and only pressing modifiers
    modKeyPress(key,0);
  }
  else if(instruction == "MOUSE") mouseControl(msg);
  else if(instruction == "REM");
  else if(instruction == "DELAY") delay(getInstruction(msg, 1).toInt());
  else
  {
      Keyboard.print(msg);
  }
  return;
}

