//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------IC Identification function-------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean identificationCase(const String& test, int pins)//newCase Pins
{
  boolean result = true;
  int clkPin = -1;
  for (uint8_t i = 0; i < pins; i++)  
  {
    switch (test[i])
    {
      case 'V' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH); 
        break;
      case 'G' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW);
        break;
      //Setting Input Signals  
      case 'X' :
      case '0' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW); 
        break;
      case '1' : pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH); 
        break;
      case 'C' : clkPin = pin[i]; pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW); 
        break;  
    }
  }
  if (clkPin != -1)
  {
    //Clock Trigger
    pinMode(clkPin, INPUT_PULLUP);
    delay(10);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
  }
  //Reading Outputs
  for (uint8_t i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'H' : 
      digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP); 
      if (!digitalRead(pin[i])) 
        {
         result = false;
         storeErrorLines[i] = "L";
         storeErrorPlace[i] = 1;
        }
        break;
      case 'L' : 
      digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP); 
      if (digitalRead(pin[i])) 
      {
          result = false;
          storeErrorLines[i] = "H";
          storeErrorPlace[i] = 1;
        }
        break;
    }
  }
  return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------IC test function-------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean loopTestCase(const String& test, int pins)//newCase Pins
{
  boolean result = true;
  int clkPin = -1;
  for (uint8_t i = 0; i < pins; i++)  
  {
    switch (test[i])
    {
      case 'V' : if(previousTestPinFunction[i] != 'V'){pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH); previousTestPinFunction[i] = 'V';} 
        break;
      case 'G' : if(previousTestPinFunction[i] != 'G'){pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW); previousTestPinFunction[i] = 'G';}
        break;
      //Setting Input Signals  
      case 'X' :
      case '0' : if(previousTestPinFunction[i] != '0'){pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW); previousTestPinFunction[i] = '0';} 
        break;
      case '1' : if(previousTestPinFunction[i] != '1'){pinMode(pin[i], OUTPUT); digitalWrite(pin[i], HIGH); previousTestPinFunction[i] = '1';}
        break;
      case 'C' : clkPin = pin[i]; pinMode(pin[i], OUTPUT); digitalWrite(pin[i], LOW); 
        break;  
    }
  }
  if (clkPin != -1)
  {
    //Clock Trigger
    pinMode(clkPin, INPUT_PULLUP);
    delay(10);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
  }
  //Reading Outputs
  for (uint8_t i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'H' : 
      if(previousTestPinFunction[i] != 'H'){digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP); previousTestPinFunction[i] = 'H';} 
      if (!digitalRead(pin[i])) 
        {
         result = false;
         storeErrorLines[i] = "L";
         storeErrorPlace[i] = 1;
        }
        break;
      case 'L' : 
      if(previousTestPinFunction[i] != 'L'){digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP); previousTestPinFunction[i] = 'L';} 
      if (digitalRead(pin[i])) 
      {
          result = false;
          storeErrorLines[i] = "H";
          storeErrorPlace[i] = 1;
        }
        break;
    }
  }
  return result;
}