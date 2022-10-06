//----------------------------------------------------------------------------------------------------------------------//
// ---------------------------------------------Main Sequence-----------------------------------------------------------//   
//----------------------------------------------------------------------------------------------------------------------//
void flow()
{    
  //---------------------------------Touch monitoring and responce---------------------------------------------
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  bool pressed = false;
  switches.pushed = 0;
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    // scale from 0->1023 to tft.width
    //p.y = map(p.y, TS_MINX, TS_MAXX, tft.height(), 0);
    //p.x = map(p.x, TS_MINY, TS_MAXY, 0,tft.width());

    p.y = map(p.y, TS_MINX, TS_MAXX, 0,tft.height());
    p.x = map(p.x, TS_MINY, TS_MAXY, tft.width(), 0);
    
    delay(150);//eh...lets see if this is really needed the debounce is shitty even so
    pressed = true; switches.pushed = 1;
  }
// -------------------------------------------Pinout buttons-----------------------------------------------------
  for (uint8_t a=0; a<*pinNumberRoutingPointer; a++)
  {   
      if (buttonsPinout[a].contains(p.x, p.y) && pressed){buttonsPinout[a].press(true);}  // tell the button it is pressed   
     else{buttonsPinout[a].press(false);}  // tell the button it is NOT pressed   
      if (buttonsPinout[a].justReleased()){if (buttonStatus[a] == 1){buttonsPinout[a].drawButton();}}  // draw normal
      if (buttonsPinout[a].justPressed()){if (buttonStatus[a] == 0){buttonsPinout[a].drawButton(true);}}  // draw invert!
      if (buttonsPinout[a].isPressed()){buttonsPinout[a].drawButton(true);}  // draw invert!      
  }
// -------------------------------------------Menu buttons-----------------------------------------------------  
  for (uint8_t a = 0; a < 13; a++)
  {   
      if (buttonsMenus[a].contains(p.x, p.y) && pressed){ buttonsMenus[a].press(true);}  // tell the button it is pressed    
     else{buttonsMenus[a].press(false);}  // tell the button it is NOT pressed
      if (buttonsMenus[a].justReleased()){}// buttonsMenus[a].drawButton();  // draw normal !!!!!!!!!need to check if these functions are leaving invisible residue!!!!!!!!!!!!
      if (buttonsMenus[a].justPressed()){}// buttonsMenus[a].drawButton(true);  // draw invert! !!!!!!!!!need to check if these functions are leaving invisible residue!!!!!!!!!!!!   
      if (buttonsMenus[a].isPressed()){}// buttonsMenus[a].drawButton(true);  // draw invert!  !!!!!!!!!need to check if these functions are leaving invisible residue!!!!!!!!!!!!      
  }    
//----------------------------------------ScreenStatus Routines--------------------------------------------------  
  Serial.println("(" +  String(p.x) + " , " + String(p.y) + " , " + String(p.z) + ")");//uncomment this line to see the touch values on the serial monitor 
  //Serial.print("screenstatus: "); Serial.println(screenStatus); //uncomment this line to see in what screenStatus the program is 
  //Serial.println(freeMemory());//Uncomment this to see how much memory is left on the device while it´s working
  //Serial.println(millis());
  
 switch(screenStatus)//every time the program waits for input from the user excluding the keypad one of those cases are in effect 
  {
  case 0:switches.circumvent = 0;  screenStatus = 1; //screenStatus 0 is so it makes sure that it has changed status, circumvent is so you can get to the main menu from the keypad screen :/ 
  case 1://The main menu        
        screenSaver();           
        mainScreenDemoScene();//a little graphic functions for the home screen                           
        if (buttonsMenus[4].justPressed())//last test Screen
        {
          readSaveState();
          if(screenStatusSD == 32){screenStatus = 72;}
          if(screenStatusSD == 69){switches.saveStateSD = 1; switches.circumventTFT = 1; screenStatus = 69;}           
        }  
        if (buttonsMenus[0].justPressed()){screenStatus = 2;}  //IC search screen
        if (buttonsMenus[2].justPressed()){screenStatus = 3;}  //Pinout screen
        if (buttonsMenus[3].justPressed()){screenStatus = 32;} //IC test screen
        if (buttonsMenus[5].justPressed()){screenStatus = 80;} //Config screen
        if (buttonsMenus[1].justPressed())
        {  
          if(switches.fastStatus == 0){switches.fastMode = 1;buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, BLUE, BLACK, GREY, "Fast ON", 2); buttonsMenus[1].drawButton();}//fast mode  ON 
          if(switches.fastStatus == 1){switches.fastMode = 0;buttonsMenus[1].initButton(&tft, 120, 140, 200, 30, WHITE, GREY, BLACK, "Fast OFF", 2); buttonsMenus[1].drawButton();}//fast mode OFF             
          if(switches.lastfastStatus == 0){ switches.fastStatus = 1; } //flip the bit for toggle effect
          if(switches.lastfastStatus == 1){ switches.fastStatus = 0; } //flip the bit for toggle effect 
        }                  
        switches.lastfastStatus = switches.fastStatus; //Updates the last read state of a fast button                   
  break;
  case 2://Select number of pins on the IC for IC search
        screenSaver();      
        if (buttonsMenus[8].justPressed()){screenStatus = 0;} //Main menu
        if (buttonsMenus[3].justPressed()){pinCount = 14; screenStatus = 21;} //14pins
        if (buttonsMenus[4].justPressed()){pinCount = 16; screenStatus = 21;} //16pins
        if (buttonsMenus[6].justPressed()){pinCount = 20; screenStatus = 21;} //20pins
        if (buttonsMenus[7].justPressed()){pinCount = 24; screenStatus = 21;} //24pins
    break;
    case 3://Pinout from Main menu screen
    case 4://Pinout from IC search result screen
    case 5://Pinout from Test result screen
        screenSaver();  
        clockAndmuxButtons();//this function contains the clock and mux/demux button actions
// ---------------------Scrolling through input buttons looking for keypresses--------------------------------     
        for(uint8_t a=0; a<*pinNumberRoutingPointer; a++)//
        {                                                     
          if (buttonsPinout[a].justPressed() && buttonStatus[a] == 0) //Input selected ON
          {      
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], HIGH); buttonNonToggleStatus[a] = 1;     
          }              
          if (buttonsPinout[a].justPressed() && buttonStatus[a] == 1) //Input selected OFF
          { 
          pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonNonToggleStatus[a] = 0; 
          }        
          if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 0){ buttonStatus[a] = 1; } 
          if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 1){ buttonStatus[a] = 0; }
          
          lastbuttonStatus[a] = buttonStatus[a];//Updates the last read state of a button 
// ------------------------Scrolling through outputs, reading them and clock pulse------------------------------               
          if(digitalRead(pin[a]) != lastoutputState[a])//So the program doesn't write to the screen when there isn't anything new to write to the screen
          {
            switch(outputs[a])//This variable keeps track of which pins are outputs
            {                 
            case 1:
              switch(digitalRead(pin[a]))//digitalRead is very slow.... :( But not as slooowwww as writing to the screen ;)
              {                                                         
                case 0:rectTest(0,a);//graphical output LOW on the left side
                break;
                case 1:rectTest(1,a);//graphical output HIGH on the left side
                break;
              }                              
            break;     
            case 2:
              switch(digitalRead(pin[a]))//digitalRead is very slow.... :( But not as slooowwww as writing to the screen ;)
              {                                    
                case 0:rectTest(2,a);//graphical output LOW on the RIGHT side
                break;
                case 1:rectTest(3,a);//graphical output HIGH on the RIGHT side
                break;                
              }                
            break;       
            }
          }           
          lastoutputState[a] = digitalRead(pin[a]); //Updates the last read state of an output                                                                       
          if (buttonsMenus[0].justPressed()) //the menubuttons are inside the for loop so they can be read faster
          {           
            for (uint8_t i = 0; i < *pinNumberRoutingPointer; i++) {pinMode(pin[i], INPUT_PULLUP); digitalWrite(pin[i], LOW);} //Clear pin assignments  and return to main menu 
            screenStatus = 0;
          }        
        }//Buttons and output ends        
        switches.lastclockbuttonStatus = switches.clockbuttonStatus;//Updates the last read state of the clock button also used for demux!!!
        switches.lastmuxdemuxToggleStatus = switches.muxdemuxToggleStatus; 
  break;
  case 21: //aftermath of search for an IC
        if(switches.autosearchEnd == 1)
        {
          if (buttonsMenus[0].justPressed()){screenStatus = 0;} //Main menu     
          for(uint8_t a=1; a<numberofIcs+1; a++){if (buttonsMenus[a].justPressed()){numberRouting = numberAuto[a]; screenStatus = 4;}} //Pinout   
        }
  break;
  case 32://testing IC result screen
        screenSaver();  
        testCompleted();           
        if (buttonsMenus[0].justPressed()){screenStatus = 0;}//Main menu
        if (buttonsMenus[1].justPressed()){screenStatus = 5;} //Pinout from Manual search result screen
        if (buttonsMenus[5].justPressed()){repeatTest(numberRouting, numberofTests.toInt());} //Repeat last test
        if (buttonsMenus[6].justPressed()){screenStatus = 41;} //Pinout from Manual search result screen
  break;
  case 41: //Error screen
        screenSaver();  
        if (buttonsMenus[3].justPressed()){screenStatus = 42;} 
        if (buttonsMenus[4].justPressed()){screenStatus = 45;} //Repeat last test     
        if (buttonsMenus[0].justPressed()){screenStatus = 0;}  
  break;
  case 42: //Raw data screen     
        if (buttonsMenus[0].justPressed()){screenStatus = 0;}//Main menu  
  break;
  case 45: //repeat last test IC result screen from the error screen  
        screenSaver();         
        testCompleted();           
        if (buttonsMenus[0].justPressed()){screenStatus = 0;}//Main menu
        if (buttonsMenus[1].justPressed()){screenStatus = 5;} //Pinout from Manual search result screen
        if (buttonsMenus[5].justPressed()){repeatTest(numberRouting, numberofTests.toInt());} //Repeat last test
        if (buttonsMenus[6].justPressed()){screenStatus = 41;} //Pinout from Manual search result screen          
  break;       
  case 69: // truthtable screen
//----------------------------------------Inputs--------------------------------------------------------------------------- 
          screenSaver();                     
          for(uint8_t a=0; a<*pinNumberRoutingPointer; a++)//
          {               
            if (buttonsPinout[a].justPressed() && buttonStatus[a] == 0) //Input selected ON
            {
              pinMode(pin[a], OUTPUT); digitalWrite(pin[a], HIGH); clockCounter++;  rowsOfValuesCounter++; buttonNonToggleStatus[a] = 1; 
            }              
            if (buttonsPinout[a].justPressed() && buttonStatus[a] == 1) //Input selected OFF
            {
              pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); if(switches.fullCycle == 0){clockCounter++; rowsOfValuesCounter++;} buttonNonToggleStatus[a] = 0; 
            }        
            if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 0){ buttonStatus[a] = 1; } 
            if (buttonsPinout[a].justReleased() && lastbuttonStatus[a] == 1){ buttonStatus[a] = 0; }
            if(lastInputPushing[a] == 0 && inputs[a] == 1 && buttonStatus[a] == 0 && inputPushing[a] == 1 ){buttonsPinout[a].drawButton(true);}
            if(lastInputPushing[a] == 1 && inputs[a] == 1 && buttonStatus[a] == 0 && inputPushing[a] == 0 ){ buttonsPinout[a].drawButton();}
            lastInputPushing[a] = inputPushing[a] ;           
            lastbuttonStatus[a] = buttonStatus[a];//Updates the last read state of a button                                        
          }        
//----------------------------------------------------------------------------------------------------------------------------------              
//-------------------------------------------------Menu Buttons functions-----------------------------------------------------------         
//----------------------------------------------------Back to main menu-------------------------------------------------------------  
        if (buttonsMenus[0].justPressed()){screenStatus = 0;} //Main menu 
//-------------------------------------------------Starts clock---------------------------------------------------------------------          
        if(switches.clockmenuToggle == 1){if (buttonsMenus[1].justPressed()){switches.clockToggle = 1;}} //turns the clock ON
//------------------------------------------Cycle input routine--------------------------------------------------------------------------     
        if(clockCounter != lastclockCounter)
        {
          getTouch(); //Serial.println("clockCounter != lastclockCounter");
          if(switches.status == 1 && cycle > 0){cycle = 0;}
        }
        if (buttonsMenus[2].justPressed() && clockCounter == lastclockCounter)
        {           
          lastclockCounter = 0; clockCounter = 0; lastOutputCounter = 0; rowsOfValuesCounter = 0; switches.clockToggle = 0;//clearing variables so they wont interfere with the next cycle input
          cycle = getIC(2).toInt();if(switches.circumvent==1)break;            
          tft.fillScreen(BLACK);//clear screen to erase the keypad screen
          if(switches.diagram == 0){table();}           
          anotherSpacer = 0; outputCounter = 0; 
          if(switches.config3 == 1){SD.remove(fname3);Serial.println("Clearing Table.csv");}
          truthtableButtons(); outputText(); writeTableInputs(); if(switches.clockmenuToggle == 1){writeOutputs();} if(switches.clockmenuToggle == 0){writeIOs();}   
          SD.remove(fname4);
          saveState();                       
          tft.fillRect(208, 260 , 70, 20, BLACK);
          tft.setCursor(210, 267);tft.setTextColor(YELLOW);
          tft.println(cycle);          
        }          
//---------------------------------------------Prints out on serial the truthtable that was generated-----------------------------------          
        if (buttonsMenus[6].justPressed())
        {
          myFile = SD.open(fname3);// open the Table.txt
          if (myFile) 
          {                             
            while (myFile.available()) 
            {
              Serial.write(myFile.read());// read from the file until there's nothing else in it:
            }                
          myFile.close(); screenStatus = 69; switches.clockToggle = 0;// close the file, make sure that the screenStatus remains 69 and the clock is OFF
          } 
          else 
          {             
            Serial.println("error opening Table.csv");// if the file didn't open, print an error:
          }            
        }       
//------------------------------------------Clears the table.csv file----------------------------------------------------------------         
        if (buttonsMenus[7].justPressed())
        {
          screenStatus = 69; switches.clockToggle = 0;
          SD.remove(fname3);Serial.println("Clearing Table.csv"); if(switches.clockmenuToggle == 1){writeOutputs();} if(switches.clockmenuToggle == 0){writeIOs();}           
        } 
//----------------------------------------------Switches between a diagram and truthtable-------------------------------------------------------          
        if (buttonsMenus[8].justPressed())
        { 
          tft.fillScreen(BLACK);//clear screen 
          anotherSpacer = 0; outputCounter = 0; switches.clockToggle = 0;
          truthtableButtons(); outputText(); writeTableInputs();              
          if(switches.statusDiagram == 0){switches.diagram = 1; buttonsMenus[8].initButton(&tft, 120, 270, 70, 30,  WHITE, BLACK, BLUE, "Table", 1); buttonsMenus[8].drawButton(); }//timing diagram ON
          if(switches.statusDiagram == 1){switches.diagram = 0; table(); buttonsMenus[8].initButton(&tft, 120, 270, 70, 30,  WHITE, BLACK, BLUE, "Diagram", 1); buttonsMenus[8].drawButton(); }//timing diagram OFF             
          if(switches.lastDiagram == 0){switches.statusDiagram = 1; } //flip the bit for toggle effect
          if(switches.lastDiagram == 1){switches.statusDiagram = 0; } //flip the bit for toggle effect 
        }
        switches.lastDiagram = switches.statusDiagram; //Updates the last read state of a fast button    
//--------------------------------------------Switches between different types of trigger--------------------------------------------------------          
        if(switches.clockmenuToggle == 0)
        {  
          if (buttonsMenus[9].justPressed() && !buttonsMenus[8].justPressed())
          {                
          if(switches.statusfullCycle == 0){switches.fullCycle = 1; buttonsMenus[9].initButton(&tft, 120, 305, 70, 30, BLACK, WHITE, BLUE, "Trig", 2); buttonsMenus[9].drawButton(); }//full cycle trigger
          if(switches.statusfullCycle == 1){switches.fullCycle = 0; buttonsMenus[9].initButton(&tft, 120, 305, 70, 30, BLACK, BLUE, WHITE, "Trig", 2); buttonsMenus[9].drawButton(); }//half cycle trigger    
          if(switches.lastfullCycle == 0){switches.statusfullCycle = 1;} //flip the bit for toggle effect
          if(switches.lastfullCycle == 1){switches.statusfullCycle = 0;} //flip the bit for toggle effect 
          }
          switches.lastfullCycle = switches.statusfullCycle; //Updates the last read state of a fast button   
        }
//--------------------------------------------Goes back to pinout mode----------------------------------------------------------------------------          
        if (buttonsMenus[11].justPressed() && !buttonsMenus[7].justPressed())
        {
          switches.saveStateSD = 1; switches.circumventTFT = 0;
          tableRowCounter = 0; clockCounter = 0; lastclockCounter = 0; anotherSpacer = 0; outputCounter = 0; OutputIndex = 1; cycle = 0; rowsOfValuesCounter = 0; 
          switches.muxdemuxToggleStatus = 0;
          for(uint8_t i=0; i<24; i++)
          {           
          lastoutputState[i] = clearArray[0];   
          tablet.sortedOutputs[i] = 0;                
          tablet.refBuffer[i] = clearArray[0]; 
          tablet.sortReference[i] = "";
          *globalpinFunctionPointer[i] ="";
          }
          screenStatus = 3;               
        }                      
//-----------------------------------refreshes the table when it gets to the end of the table-------------------------------------------------                                                                            
        if(countingOutputs < 14)
        {          
          if(rowsOfValuesCounter == 9)
          {                                   
            tft.fillRect(60, 1 , 250, (outputCounter+1)*15, BLACK); rowsOfValuesCounter = 1; lastOutputCounter =-1; //truthtableData(); truthtableButtons();  
            if(switches.diagram == 0) {table();}
          }   
        }           
        if(countingOutputs >= 14)
        {  
          if(rowsOfValuesCounter == 7)
          {                                   
            tft.fillRect(60, 1 , 140, (outputCounter+1)*15, BLACK); rowsOfValuesCounter = 1; lastOutputCounter =-1; //truthtableData(); truthtableButtons();  
            if(switches.diagram == 0) {table();}
          }  
        }
//-----------------------------------Switches between diagram and truthtable function-------------------------------------------------------------         
        switch (switches.diagram)
        {
          case 0: truthTableValues();
          break;
          case 1: timingDiagram();
          break;             
        }   
//---------------------------------Stops generation of diagram/table when number of cycles have been completed---------------------------------------         
        if(clockCounter >= cycle){switches.clockToggle = 0; cycle = 0; automaticInputButtonPusher = 0;}                                 
        lastOutputCounter = cycle;                                                      
    break;
    case 72:
        screenSaver();  
        testCompleted();           
        if (buttonsMenus[0].justPressed()){screenStatus = 0;}//Main menu
        if (buttonsMenus[1].justPressed()){screenStatus = 5;} //Pinout from Manual search result screen
        if (buttonsMenus[5].justPressed()){repeatTest(numberRouting, numberofTests.toInt());} //Repeat last test
        if (buttonsMenus[6].justPressed()){screenStatus = 41;} //Pinout from Manual search result screen
    break; 
    case 73: screenStatus = 69;
    break;  
    case 80:       
        if (buttonsMenus[0].justPressed()){SD.remove(fname5); writeConfig(); screenStatus = 0;} //Main menu 
        if (buttonsMenus[6].justPressed()){screenStatus = 81;} //Clock speed select     
        if (buttonsMenus[4].justPressed())
        {  
          if(switches.config1Status == 0){switches.config1 = 1; buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "ON", 2);  buttonsMenus[4].drawButton(true); switches.keypadIC = 1;}//
          if(switches.config1Status == 1){switches.config1 = 0; buttonsMenus[4].initButton(&tft, 210, 100, 50, 20, WHITE, GREY, BLACK, "OFF", 2); buttonsMenus[4].drawButton(); switches.keypadIC = 0;}//          
          if(switches.lastconfig1Status == 0){ switches.config1Status = 1; } //flip the bit for toggle effect
          if(switches.lastconfig1Status == 1){ switches.config1Status = 0; } //flip the bit for toggle effect 
        }    
        switches.lastconfig1Status = switches.config1Status; //Updates the last read state of a config 1 button   
        if (buttonsMenus[5].justPressed())
        {  
          if(switches.config2Status == 0){switches.config2 = 1;buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "ON", 2);  buttonsMenus[5].drawButton(true); switches.pushingButtons = 1;}//
          if(switches.config2Status == 1){switches.config2 = 0;buttonsMenus[5].initButton(&tft, 210, 130, 50, 20, WHITE, GREY, BLACK, "OFF", 2); buttonsMenus[5].drawButton(); switches.pushingButtons = 0;}//           
          if(switches.lastconfig2Status == 0){ switches.config2Status = 1; } //flip the bit for toggle effect
          if(switches.lastconfig2Status == 1){ switches.config2Status = 0; } //flip the bit for toggle effect 
        }    
        switches.lastconfig2Status = switches.config2Status; //Updates the last read state of a config 1 button   
        if (buttonsMenus[7].justPressed())
        {  
          if(switches.config3Status == 0){switches.config3 = 1;buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "ON", 2);  buttonsMenus[7].drawButton(true); switches.clearCSV = 1;}//
          if(switches.config3Status == 1){switches.config3 = 0;buttonsMenus[7].initButton(&tft, 210, 160, 50, 20, WHITE, GREY, BLACK, "OFF", 2); buttonsMenus[7].drawButton(); switches.clearCSV = 0;}//            
          if(switches.lastconfig3Status == 0){ switches.config3Status = 1; } //flip the bit for toggle effect
          if(switches.lastconfig3Status == 1){ switches.config3Status = 0; } //flip the bit for toggle effect 
        }    
        switches.lastconfig3Status = switches.config3Status; //Updates the last read state of a config 1 button      
        if (buttonsMenus[8].justPressed())
        {  
          if(switches.config4Status == 0){switches.config4 = 1;buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "ON", 2);  buttonsMenus[8].drawButton(true); switches.diagram = 1; switches.statusDiagram = 1;}//
          if(switches.config4Status == 1){switches.config4 = 0;buttonsMenus[8].initButton(&tft, 210, 190, 50, 20, WHITE, GREY, BLACK, "OFF", 2); buttonsMenus[8].drawButton(); switches.diagram = 0; switches.statusDiagram = 0;}//            
          if(switches.lastconfig4Status == 0){ switches.config4Status = 1; } //flip the bit for toggle effect
          if(switches.lastconfig4Status == 1){ switches.config4Status = 0; } //flip the bit for toggle effect 
        }    
        switches.lastconfig4Status = switches.config4Status; //Updates the last read state of a config 1 button        
        if (buttonsMenus[9].justPressed())
        {  
          if(switches.config5Status == 0){switches.config5 = 1;buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "ON", 2); screenStatus = 82;}//
          if(switches.config5Status == 1){switches.config5 = 0;buttonsMenus[9].initButton(&tft, 210, 280, 50, 20, WHITE, GREY, BLACK, "OFF", 2); buttonsMenus[9].drawButton(); switches.macgyver = 0;}//            
          if(switches.lastconfig5Status == 0){ switches.config5Status = 1; } //flip the bit for toggle effect
          if(switches.lastconfig5Status == 1){ switches.config5Status = 0; } //flip the bit for toggle effect 
        }    
        switches.lastconfig5Status = switches.config5Status; //Updates the last read state of a config 1 button                     
    break;
    case 81: screenStatus = 80;
    break;
    case 82: screenStatus = 80;
    break;
    case 90: 
            if(previousScreenstatus == 69)
            {
              switches.saveStateSD = 1; switches.circumventTFT = 0;
              tableRowCounter = 0; clockCounter = 0; lastclockCounter = 0; anotherSpacer = 0; outputCounter = 0; OutputIndex = 1; cycle = 0; rowsOfValuesCounter = 0; 
              switches.muxdemuxToggleStatus = 0;
              for(uint8_t i=0; i<24; i++)
              {            
              tablet.sortedOutputs[i] = 0;                
              tablet.refBuffer[i] = clearArray[0]; 
              tablet.sortReference[i] = "";
              *globalpinFunctionPointer[i] ="";
              }
            } 
            screenStatus = previousScreenstatus; 
    break;
  }
  pinMode(XM, OUTPUT);//check if it should be right after line 44
  digitalWrite(XM, LOW);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);
//-----------------------------------------Switch Case Routines----------------------------------------------------
  if (screenStatus != lastStatus)//so it only updates once per statechange(screenStatus)
  {   
  switch (screenStatus)
    {
    case 1://All roads lead to Rome.                                           
            //SD.remove(fname5); //!!!!!uncomment this line if the program doesn't load properly after uploading because the config file might be corrupt!!!!!//
            //readSavedFile();   //reads from the SD card relevent informations regarding previous test and prints it on the serial monitor       
            //readConfigFile();  //reads from the SD card the last configuration state and prints it on the serial monitor
            //readICsInPinout(); //prints out all the ICs in pinout.txt
            //readICsInDatabase(); //prints out all the ICs in database.txt
              clearingRoutines();//clearing routines so previous tests don't interfere with future tests.  
              readConfig();      //assigns config variables with saved config data from the SD card
              modeScreen();      //Main menu screen
      break;
    case 2:   autoScreen(); //Select number of pins for auto search
      break;      
    case 3:   switches.manualSwitch = 1; if(switches.saveStateSD == 0){numberRouting = getIC(0);} if(switches.circumvent==1)break; Test_init(pinCount); lastoutputState[0] = 4; switches.saveStateSD = 0;  //Manual pinout select
      break;
    case 4:   switches.manualSwitch = 0; Test_init(pinCount); SD.remove(fname4); saveState();  // Pinout after automatic search result
      break;
    case 5:   switches.manualSwitch = 1; Test_init(pinCount); SD.remove(fname4); saveState(); // Pinout after IC testing
      break;
    case 21:  autoSearch(pinCount); switches.autosearchEnd = 1; autoSearchResult();//Searches the SD card for the right chip
      break;      
    case 32:  numberRouting.reserve(6); numberRouting = getIC(0); if(switches.circumvent == 1)break; 
              numberofTests.reserve(8);numberofTests = getIC(1); if(switches.circumvent == 1)break;  //cycle = 1; switches.diagram = 0;             
              SD.remove(fname4); saveState();             
              repeatTest(numberRouting, numberofTests.toInt());//Loop test routine              
      break;
    case 41:  errorInfo();//error info screen    
      break;
    case 42:  rawData();//raw info screen    
      break;   
    case 45:  repeatTest(numberRouting, numberofTests.toInt());//Check if case 72 can be 45 instead
      break;   
    case 69:  
              if(switches.saveStateSD == 1){switches.manualSwitch = 1; switches.circumventTFT = 1; Test_init(pinCount);} //this gets executed if the last state saved was a truthtable
              clockCounter = 0; truthtableData(); if(switches.clockmenuToggle == 1){writeOutputs();} if(switches.clockmenuToggle == 0){writeIOs();} anotherSpacer = 0; outputCounter = 0; outputText(); 
              if(switches.diagram == 0){table();} truthtableButtons(); numberofTests = 1; //number of tests gets set here so there is a value to be written on the SD card files even though it isn't needed for this routine
      break;
    case 72:  repeatTest(numberRouting, numberofTests.toInt());//if last test that was saved to the SD card was a looptest it runs this routine
      break;  
    case 80:  configScreen();
      break;
    case 81:  speed = getIC(3).toInt(); speed = map(speed, 10, 1, 2, 255); if(switches.circumvent == 1)break;
      break;  
    case 82:  macgyverTimingMechanism = getIC(4).toInt(); macgyverTimingMechanism = map(macgyverTimingMechanism, 1, 2, 30000, 65000); if(switches.circumvent == 1){break;} macgyverCounter = 0; switches.macgyver = 1;
      break;   
    }
//---------------------------------------------Routines end------------------------------------------------------    
  lastStatus = screenStatus;
  } 
 lapsCounter++;//keeps track of how many times the main routine has been executed
 macgyverCounter++;//screensaver timer
}
