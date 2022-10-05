//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------SD card functions--------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------//
//---------------------------------Writes the config variables to the SD card-------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void writeConfig()
{
  myFile = SD.open(fname5, FILE_WRITE);   
  if (myFile) // if the file opened okay, write to it:
  {
    myFile.println(String(switches.config1)); //prints the Last IC config 1 to the SD card
    myFile.println(String(switches.config2)); //prints the Enable Inputs config 2 to the SD card
    myFile.println(String(switches.config3)); //prints the Enable Inputs config 3 to the SD card
    myFile.println(String(switches.config4)); //prints the Enable Inputs config 4 to the SD card
    myFile.println(String(speed)); //prints the Clock Speed to the SD card
    myFile.println(String(switches.config5)); //prints the Enable Inputs config 5 to the SD card
    myFile.println(String(macgyverTimingMechanism)); //prints the screensaver time to the SD card
  } 
  else 
  {    
    Serial.println("error opening Config.txt"); // if the file didn't open, prints an error:
  }           
  myFile.close(); 
}
//----------------------------------------------------------------------------------------------------------------------//
//---------------------------------Reads the config variables from the SD card------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void readConfig()
{
myFile = SD.open(fname5);// open the Config.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      switches.config1 = myFile.readStringUntil('\n').toInt();
      switches.config2 = myFile.readStringUntil('\n').toInt();
      switches.config3 = myFile.readStringUntil('\n').toInt();
      switches.config4 = myFile.readStringUntil('\n').toInt();
      speed = myFile.readStringUntil('\n').toInt();
      switches.config5 = myFile.readStringUntil('\n').toInt();
      macgyverTimingMechanism = myFile.readStringUntil('\n').toInt();           
      Serial.println("Succesfully read from Config.txt");
    }
    myFile.close(); 
    numberRouting.trim();                
  } 
  else 
  {             
    Serial.println("error reading Config.txt");// if the file didn't open, print an error:
  }   
  if(switches.config1 == 1){switches.keypadIC = 1;}//last IC keypad ON 
  if(switches.config1 == 0){switches.keypadIC = 0;}//last IC keypad OFF                        
  if(switches.config2 == 1){switches.pushingButtons = 1;}//auto pushbuttons ON 
  if(switches.config2 == 0){switches.pushingButtons = 0;}//auto pushbuttons OFF   
  if(switches.config3 == 1){switches.clearCSV = 1;}//clear CSV ON 
  if(switches.config3 == 0){switches.clearCSV = 0;}//clear CSV OFF  
  if(switches.config4 == 1){switches.diagram = 1;}//hold buttons ON 
  if(switches.config4 == 0){switches.diagram = 0;}//hold buttons OFF     
  if(switches.config5 == 1){switches.macgyver = 1;}//hold buttons ON 
  if(switches.config5 == 0){switches.macgyver = 0;}//hold buttons OFF    
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------Prints the config variables from the SD card on the serial monitor-----------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void readConfigFile()
{
myFile = SD.open(fname5);// open the Config.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      Serial.write(myFile.read());// read from the file until there's nothing else in it: 
    } 
    myFile.close();                
  } 
  else 
  {             
    Serial.println("error reading Config.txt");// if the file didn't open, print an error:
  }      
}
//----------------------------------------------------------------------------------------------------------------------//
//------------------------------------------SD card save last test functions--------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void saveState()
{
  myFile = SD.open(fname4, FILE_WRITE);                  
  if (myFile) // if the file opened okay, write to it:
  {
    myFile.println(numberRouting);
    myFile.println(String(screenStatus));
    myFile.println(numberofTests); 
    myFile.println(String(cycle)); 
    myFile.println(String(switches.diagram));        
  } 
  else 
  {         
    Serial.println("error opening State.txt"); // if the file didn't open, print an error:
  }
  myFile.close();             
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------Reads the saved state variables from the SD card-----------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void readSaveState()
{
myFile = SD.open(fname4);//open State.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      numberRouting = myFile.readStringUntil('\n');
      screenStatusSD = myFile.readStringUntil('\n').toInt();
      numberofTests = myFile.readStringUntil('\n');
      cycle = myFile.readStringUntil('\n').toInt();
      switches.diagram = myFile.readStringUntil('\n').toInt();
    }
    myFile.close(); 
    numberRouting.trim();                
  } 
  else 
  {             
    Serial.println("error reading State.txt");// if the file didn't open, print an error:
  }    
}
//----------------------------------------------------------------------------------------------------------------------//
//---------------------Prints the saved state variables from the SD card on the serial monitor--------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void readSavedFile()
{
myFile = SD.open(fname4);// open the State.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      Serial.write(myFile.read());// read from the file until there's nothing else in it: 
    } 
    myFile.close();                
  } 
  else 
  {             
    Serial.println("error reading State.txt");// if the file didn't open, print an error:
  }      
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------Prints the config variables from the SD card on the serial monitor-----------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void readICsInPinout()
{
String buffer;
myFile = SD.open(fname2);// open the Config.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      myFile.readStringUntil('$');      
      buffer.reserve(6); 
      buffer= myFile.readStringUntil('\n'); //chip id   
      Serial.print(buffer);Serial.print(" ");// read from the file until there's nothing else in it: 
    } 
    myFile.close();                
  } 
  else 
  {             
    Serial.println("error reading Pinout.txt");// if the file didn't open, print an error:
  }      
Serial.println(" ");
}
void readICsInDatabase()
{
String buffer;
myFile = SD.open(fname);// open the Config.txt
  if (myFile) 
  {                             
    while (myFile.available()) 
    {      
      myFile.readStringUntil('$');      
      buffer.reserve(6); 
      buffer= myFile.readStringUntil('\n'); //chip id   
      Serial.print(buffer);Serial.print(" ");// read from the file until there's nothing else in it: 
    } 
    myFile.close();                
  } 
  else 
  {             
    Serial.println("error reading Pinout.txt");// if the file didn't open, print an error:
  }      
}