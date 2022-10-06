//----------------------------------------------------------------------------------------------------------------------//
// ---------------------------------------------Screen Saver Routine----------------------------------------------------//  
//----------------------------------------------------------------------------------------------------------------------//   
void screenSaver()
{         
  previousScreenstatus = screenStatus;                          
  if(switches.pushed == 1){macgyverCounter = 0;}
  if(switches.macgyver == 1)
  {
    if(macgyverCounter >= macgyverTimingMechanism)
    {
      tft.fillScreen(BLACK);//"Clear" the screen             
      while(switches.status == 0)
      {            
        getTouch();//this is so the user can stop the test at any time
      }            
    if(switches.status == 1) macgyverCounter = 0; screenStatus = 90; switches.status = 0; //this is so the user can stop the test at any time
    }  
  }   
}           
//----------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------Clearing Routines----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void clearingRoutines()
{
  speed = map(speed, 10, 1, 2, 255);
  
  for(uint8_t i=0; i<*pinNumberRoutingPointer; i++) 
  {
    pinMode(pin[i], INPUT_PULLUP); //Remove Voltages from IC(sets all the arduino pins that are connected to the IC as Inputs and by that removing any voltages from the arduino connected to the IC)
    digitalWrite(pin[i], LOW); 

    clock[i] = clearArray[0];//Clear clock Array 
    inOut[i] = clearArray[0];//clear the Mux array
    outIn[i] = clearArray[0];//clear the deMux array
  }  
   for(uint8_t i=0; i<24; i++) 
  {
    previousTestPinFunction[i] = ' ';
    buttonStatus[i] = 0;
    lastbuttonStatus[i] = 0;
    buttonsMenus[i].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); //CLear the menubuttons otherwise it becomes a mangled mess :(   
    buttonsPinout[i]  = buttonsClearPinout[0]; 
    inputs[i] = clearArray[0]; 
    outputs[i] = clearArray[0]; 
    lastoutputState[i] = clearArray[0]; 
    tablet.sortedOutputs[i] = clearArray[0];  
    tablet.refBuffer[i] = clearArray[0]; 
    tablet.sortReference[i] = "";
    globalpinFunction[i] =  "";
  } 
  numberofTests = "";//Clear numbers of tests chosen before
  numberofIcs = 0; //clear the number of ICs that matched the autotest
  pinCount = 0; 
  clockCounter = 0; 
  lastclockCounter = 0; 
  OutputIndex = 1; 
  rowsOfValuesCounter = 0; 
  cycle = 0;
  tableRowCounter = 0; 
  switches.circumventTFT = 0;
  switches.autosearchEnd = 0; //make sure that screenStatus 21 buttons don´t interfere until search is over
  switches.endlessLoopSwitch = 0;   
  switches.clockToggle = 0; 
  switches.clockbuttonStatus = 1; 
  switches.clockmenuToggle = 0; //reset the clock button between modes
  switches.muxdemuxFlag = 0; 
  switches.muxdemuxToggle = 0; 
  switches.muxdemuxMenuToggle = 0; //reset the mux button between modes
  switches.saveStateSD = 0;  
}
//----------------------------------------------------------------------------------------------------------------------//
// ------------------------Scrolling through clock pins and enable/disableing them--------------------------------------//  
//----------------------------------------------------------------------------------------------------------------------//                 
void clockRoutine(bool state)
{
 // Serial.println("Im in the clock routine");
//-------------------------------------------Switching Input buttons----------------------------------------------------//  
   if(switches.pushingButtons == 1)
  {  
    for(int a=0; a<*pinNumberRoutingPointer; a++)
    {  
      if(*globalpinFunctionPointer[a] == "Input  ")
      {
        if(automaticInputButtonPusher >= *pinNumberRoutingPointer){automaticInputButtonPusher = 0;}
        //if(buttonStatus[automaticInputButtonPusher] == 1){pinMode(pin[automaticInputButtonPusher], OUTPUT); digitalWrite(pin[automaticInputButtonPusher], HIGH);}
        if(inputs[automaticInputButtonPusher] == 1 && buttonStatus[automaticInputButtonPusher] == 0)
        {                    
          pinMode(pin[automaticInputButtonPusher], OUTPUT);
          if(state == 0)
          {
            digitalWrite(pin[automaticInputButtonPusher], LOW); inputPushing[automaticInputButtonPusher] = 0;
          } 
          if(state == 1)
          {
            digitalWrite(pin[automaticInputButtonPusher], HIGH); inputPushing[automaticInputButtonPusher] = 1;
          }                            
        }  
      }
    } 
  }  
//-----------------------------------------------------Clock Routine----------------------------------------------------//  
  for(uint8_t a=0; a<*pinNumberRoutingPointer; a++)//
  { 
    if(clock[a] == 1)
    {                    
      pinMode(pin[a], OUTPUT);       
      if(state == 0){digitalWrite(pin[a], LOW);} 
      if(state == 1){digitalWrite(pin[a], HIGH); clockCounter++; rowsOfValuesCounter++;}                               
    }            
  }   
}  
void automateRoutine(bool state)
{   
  if(automaticInputButtonPusher >=*pinNumberRoutingPointer){automaticInputButtonPusher = 0;}
  if(inputs[automaticInputButtonPusher] == 1 && buttonStatus[automaticInputButtonPusher] == 0)
  {                          
    pinMode(pin[automaticInputButtonPusher], OUTPUT);
    if(state == 0)
    {
      digitalWrite(pin[automaticInputButtonPusher], LOW); inputPushing[automaticInputButtonPusher] = 0;
    }
    if(state == 1)
    {
      digitalWrite(pin[automaticInputButtonPusher], HIGH); clockCounter++; rowsOfValuesCounter++; inputPushing[automaticInputButtonPusher] = 1;
    }                            
  }            
}  
//----------------------This function checks the mode of a pin(whether it's an Input or an Output)----------------------//
//-----------------------------------------bastardization of post 6 by davekw7x-----------------------------------------//
//----------------- https://forum.arduino.cc/t/is-is-possible-to-detect-pin-mode-in-and-if/52904/6 ---------------------//

// Test function to display DDR register bits for Arduino pins
int getPinMode(int p) {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  const int MAX_DIGITAL_PIN_NUMBER = 69;
#else
  const int MAX_DIGITAL_PIN_NUMBER = 19;
#endif
  // Check valid pin number
  if (p > MAX_DIGITAL_PIN_NUMBER) {
    return -1;
  }
  // Convert designated Arduino pin to ATMega port and pin
  uint8_t pbit = digitalPinToBitMask(p);
  uint8_t pport = digitalPinToPort(p);

  // Read the ATmega DDR for this port
  volatile uint8_t *pmodereg = portModeRegister(pport);

  // Test the value of the bit for this pin and return
  // 0 if it is reset and 1 if it is set
  return ((*pmodereg & pbit) != 0);
}
void pinModeTester() {
  for (int pin = 0; pin < 69; pin++) {
    byte mode = getPinMode(pin);
    if (mode < 0) {
      Serial.print("Pin number ");
      Serial.print(pin);
      Serial.println(" is invalid.");
    } else {
      Serial.print("Mode of Pin ");
      Serial.print(pin);
      Serial.print(" is ");
      if (mode == INPUT) {
        Serial.println("INPUT.");
      } else {
        Serial.println("OUTPUT.");
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------When a little bit of touch is needed-----------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void getTouch() 
{
  switches.status = 0;  
  digitalWrite(13, HIGH);
  TSPoint q = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (q.z > MINPRESSURE && q.z < MAXPRESSURE) 
  {
      switches.status = 1;    
  }
}
//----------------------------------------------------------------------------------------------------------------------//
//---------------------------The function below monitors the free memory on the device----------------------------------//
//-------------------- https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory -------------------------//
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}