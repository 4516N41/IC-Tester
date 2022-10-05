//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Keypad function--------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
//UI details
#define BUTTON_X 28 //34
#define BUTTON_Y 94 //90
#define BUTTON_W 36 //50
#define BUTTON_H 40 //46
#define BUTTON_SPACING_X 10 //13
#define BUTTON_SPACING_Y 10
//#define BUTTON_TEXTSIZE 2

// text box where numbers go
#define TEXT_X 5
#define TEXT_Y 5
#define TEXT_W 230
#define TEXT_H 55
#define TEXT_TSIZE 3
#define TEXT_TCOLOR GREEN
#define TEXT_LEN 7

const char buttonlabels[10][5] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

String getIC(byte keypadText)
{
  tft.fillScreen(BLACK);
 
  String number = "";
  unsigned long numberIC = 0;  
  //String numberLookup = "";
  int i = 0;
  boolean status = false;
  
  // create buttons
  for (uint8_t row = 0; row < 2; row++) 
  {
    for (uint8_t col = 0; col < 5; col++) 
    {
      buttonsMenus[col + row * 5].initButton(&tft, BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X), BUTTON_Y + row * (BUTTON_H + BUTTON_SPACING_Y), BUTTON_W, BUTTON_H, WHITE, BLACK, WHITE, buttonlabels[col + row * 5], 2);
      buttonsMenus[col + row * 5].drawButton();
    }
  }
  buttonsMenus[10].initButton(&tft, 70, BUTTON_Y + 2 * (BUTTON_H + BUTTON_SPACING_Y), 50, BUTTON_H, WHITE, WHITE, BLACK, "OK", 2);
  buttonsMenus[10].drawButton();
  buttonsMenus[11].initButton(&tft, 150, BUTTON_Y + 2 * (BUTTON_H + BUTTON_SPACING_Y), 80, BUTTON_H, WHITE, BLACK, WHITE, "Clear", 2);
  buttonsMenus[11].drawButton();
  buttonsMenus[12].initButton(&tft, 120, 290, 200, 40, WHITE, GREY, BLACK, "Main menu", 3);
  buttonsMenus[12].drawButton();
  // create 'text field'
  tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);
  tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
  tft.setTextColor(TEXT_TCOLOR, BLACK);
  tft.setTextSize(TEXT_TSIZE);
    
  if(keypadText == 0)
  { 
    tft.print(F("  IC number"));
    delay(500);
    tft.setTextColor(BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(F("  IC number"));
    if(switches.keypadIC == 1) {number = numberRouting;}
    tft.setTextColor(TEXT_TCOLOR, BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(number);
  }
  if(keypadText == 1)
  {
    tft.print(F(" Test count"));
    delay(500);
    tft.setTextColor(BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(F(" Test count"));
  }
  if(keypadText == 2)
  {
    tft.print(F(" Cycle count"));
    delay(500);
    tft.setTextColor(BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(F(" Cycle count"));
  }
  if(keypadText == 3)
  {
    tft.print(F("Speed 1-10"));
    delay(500);
    tft.setTextColor(BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(F("Speed 1-10"));

  }
  if(keypadText == 4)
  {
    tft.print(F("Time 1-2"));
    delay(500);
    tft.setTextColor(BLACK);
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.print(F("Time 1-2"));

  }
  while (!status)
  {
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    //Serial.println(keypadText);
    bool pressed = false;

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
      // scale from 0->1023 to tft.width
    p.y = map(p.y, TS_MINX, TS_MAXX, tft.height(), 0);
    p.x = map(p.x, TS_MINY, TS_MAXY, 0,tft.width());
   
      pressed = true;
    }

    // go thru all the buttons, checking if they were pressed
    for (uint8_t b = 0; b < 14; b++)
    {
      if (buttonsMenus[b].contains(p.x - 8, p.y - 8) && pressed)
      {
        buttonsMenus[b].press(true);  // tell the button it is pressed
      }
      else
      {
        buttonsMenus[b].press(false);  // tell the button it is NOT pressed
      }
    }

    // now we can ask the buttons if their state has changed
    for (uint8_t b = 0; b < 14; b++)
    {
      if (buttonsMenus[b].justReleased())
      {
        //Serial.print("Released: "); Serial.println(b);
        buttonsMenus[b].drawButton();  // draw normal
      }

      if (buttonsMenus[b].justPressed())
      {
        buttonsMenus[b].drawButton(true);  // draw invert!

        // if a numberpad button, append the relevant # to the textfield
        if (b < 10) {
          if (i < TEXT_LEN)
          {
            number += buttonlabels[b][0];
            i++;
          }
        }

        // clr button! delete char
        if (b == 11) {
          tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
          tft.setTextColor(BLACK);
          tft.setTextSize(TEXT_TSIZE);
          tft.print(number);
          number = "";
          i = 0;
        }
        
        // update the current text field
        //Serial.println(textfield);
        tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
        tft.setTextColor(TEXT_TCOLOR, BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(number);

        // we dont really check that the text field makes sense
        // just try to call
        if(keypadText == 0)
        { 
          numberIC = number.toInt();                    
          if (numberIC > 3999 && numberIC < 4600 && b == 10 || numberIC > 7399 && numberIC < 75000 && b == 10) 
          {
          for(uint8_t i=0; i<24; i++) 
            {
            buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
            }
            tft.fillRect(0, 220 , 240, 50, BLACK);
            status = true;
          }
        }          
         if(keypadText == 1 || keypadText == 2)
        {                   
          if (b == 10) 
          {
          for(uint8_t i=0; i<24; i++) 
            {
            buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
            }
            tft.fillRect(0, 220 , 240, 50, BLACK);
            status = true;
          }
        } 
         if(keypadText == 3)
        { 
          numberIC = number.toInt();                    
          if (b == 10 && numberIC > 0 && numberIC < 11) 
          {
          for(uint8_t i=0; i<24; i++) 
            {
            buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
            }
            tft.fillRect(0, 220 , 240, 50, BLACK);
            status = true;
          }
        } 
         if(keypadText == 4)
        { 
          numberIC = number.toInt();                    
          if (b == 10 && numberIC > 0 && numberIC < 3) 
          {
          for(uint8_t i=0; i<24; i++) 
            {
            buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
            }
            tft.fillRect(0, 220 , 240, 50, BLACK);
            status = true;
          }
        }              
        if (b == 12) 
        {   
          for(uint8_t i=0; i<24; i++) 
          {
          buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
          }     
          switches.circumvent = 1;
          screenStatus = 0;
          tft.fillRect(0, 220 , 240, 50, BLACK);
          status = true;
        }             
        //delay(100); // UI debouncing          
      }    
    }   
  }
  return number;
}