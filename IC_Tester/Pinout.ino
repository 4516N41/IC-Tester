//----------------------------------------------------------------------------------------------------------------------//
//---------------------------------------------Pinout function----------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void Test_init(int pins)
{ 
  File dataFile = SD.open(fname2);//open the Pinouts.txt file
  chip newChip; 
  word nudge;
  word buttonHeight = 50; 
  word buttonYaxis = 290;
  buttonSize = 22;
  outputSize = 10;
  boxSize = 20;  
  horizontalNudge = 0;
  newChip.num.reserve(6);
  unsigned long testPosition = 0;
  unsigned long *testPositionPointer = &testPosition;
  tft.fillScreen(BLACK);  //"clear" the screen
  if(numberRouting.toInt() > 7399 && numberRouting.toInt() < 7500 || numberRouting.toInt() > 74000){dataFile.seek(33700);}// this line here speeds up the process of looking for the 7400 series,could cause problems if a IC of the cd4000 series is removed!! 
  if (dataFile)
  {   
    while (dataFile.available())
    {               
//------------------------------------------Looking for the right chip -------------------------------------------------//              
      dataFile.readStringUntil('$');   
      *testPositionPointer = dataFile.position(); //this variable gets set to start of each line of the test routines, here it´s set to the start of the test sequence        
      newChip.num = dataFile.readStringUntil('\n'); //chip id
      if(newChip.num.toInt() != numberRouting.toInt()){dataFile.seek(*testPositionPointer + 210);}//this "scrolls/jumps" through the Pinout lines       
//--------------------------------When the right chip has been identified routine---------------------------------------//
      if(newChip.num.toInt() == numberRouting.toInt()) //comparing IC number to the one chosen by the user
      {      
      numberofIcs = 1;   
      dataFile.find('\n'); // chip description, not needed here...yet?
      newChip.pins = dataFile.readStringUntil('\n').toInt(); //pin count   
      pinNumberRouting = newChip.pins; //so I can use the appropiate number of pins outside the scope              
      if(switches.manualSwitch == 0) {newChip.pins = pins;}//
      if(switches.manualSwitch == 1)//
        {                     
        switch(newChip.pins)
          {
            case 14:pin = PIN14;
            break;
            case 16:pin = PIN16;
            break;
            case 20:pin = PIN20;
            break;
            case 24:pin = PIN24;
            break;
          }      
        }         
//--------------------------------------------Graphics body routine-----------------------------------------------------//         
     if(switches.circumventTFT == 0)
     {
      if(newChip.pins == 14 || newChip.pins == 16){tft.drawRect(80, 10 , 80, 240, BLUE);}//chip outline if the IC is a 14 or 16 pin chip  
      if(newChip.pins == 20){tft.drawRect(80, 5 , 80, 270, BLUE);}//chip outline if the chip is a 20 pin IC  
      if(newChip.pins == 24){tft.drawRect(80, 10 , 80, 270, BLUE);}//chip outline if the chip is a 24 pin IC   
      tft.setTextColor(WHITE); tft.setTextSize(4);
      tft.setCursor(110, 60);  tft.println(newChip.num[0]);
      tft.setCursor(110, 90);  tft.println(newChip.num[1]);
      tft.setCursor(110, 120); tft.println(newChip.num[2]);
      tft.setCursor(110, 150); tft.println(newChip.num[3]);    
      if(newChip.num[4] != 0)//for those chips with 5 numbers in their ID
        {
          tft.setCursor(110, 180);
          tft.println(newChip.num[4]);
        } 
     }                 
      for (uint8_t i = 0; i < newChip.pins; i++) {outputs[i] = clearArray[0]; digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);}//Clear output Array   
 //-----------------Various graphical spacing and shifting depending on the number of pins on the chip------------------//
      switch(newChip.pins)
      {
        case 14: nudge = 30; pinSpacer = 30; shifterLeft = 9; shifterRight = 459;
        break;
        case 16: nudge = 20; pinSpacer = 28; shifterLeft = 1; shifterRight = 476;
        break;
        case 20: nudge = 13; pinSpacer = 26; shifterLeft = -3; shifterRight = 542; buttonHeight = 30; buttonYaxis = 305;
        break;
        case 24: nudge = 13; pinSpacer = 22; shifterLeft = 0; shifterRight = 550; buttonHeight = 30; buttonYaxis = 305; buttonSize = 18; boxSize = 16; outputSize = 8; horizontalNudge = 2;
        break;
      }              
//-----------------------------------------Graphics left side pins routine----------------------------------------------//        
      for(uint8_t a=0; a<newChip.pins/2; a++)
      {   
        newChip.pinFunction.reserve(9);    
        newChip.pinFunction = dataFile.readStringUntil('%'); 
        *globalpinFunctionPointer[a] = newChip.pinFunction;       
        if(newChip.pinFunction == "Input  ") {pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, NAVY, BLUE, WHITE, "", 1); if(switches.circumventTFT == 0){buttonsPinout[a].drawButton();}}//
        if(newChip.pinFunction == "Clock  ") {clock[a] = 1; buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, BLUE, PURPLE, WHITE, "C", 2);if(switches.circumventTFT == 0){ buttonsPinout[a].drawButton();}}//
        if(newChip.pinFunction == "Output ") {outputs[a] = 1; tablet.sortedOutputs[a] = 1; if(switches.circumventTFT == 0){tft.fillCircle(80, nudge+9, outputSize, DARKGREY);}}
        if(newChip.pinFunction == "GND    ") {pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); if(switches.circumventTFT == 0){tft.fillRect(70 + horizontalNudge, nudge , boxSize, boxSize, GREEN);}}//
        if(newChip.pinFunction == "VCC    ") {pinMode(pin[a], OUTPUT); digitalWrite(pin[a], HIGH); if(switches.circumventTFT == 0){tft.fillRect(70 + horizontalNudge, nudge , boxSize, boxSize, RED); }}//
        if(newChip.pinFunction == "NC     ") { if(switches.circumventTFT == 0){tft.fillRect(70 + horizontalNudge, nudge , boxSize, boxSize, NAVY);}}//        
        if(newChip.pinFunction == "In/Out ") {inOut[a] = 1; pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); if(switches.circumventTFT == 0){buttonsPinout[a].drawButton();} *globalpinFunctionPointer[a] = "Input  ";}//
        if(newChip.pinFunction == "Out/In ") {outIn[a] = 1; outputs[a] = 1; tablet.sortedOutputs[a] = 1; if(switches.circumventTFT == 0){tft.fillCircle(80, nudge+9, outputSize, DARKGREY);} *globalpinFunctionPointer[a]= "Output  ";}   //here the Com ports are outputs      
//--------------------------------------------References on the left side-----------------------------------------------//        
        *globalReferencePointer[a]= dataFile.readStringUntil('\n');
        tft.setCursor(17, nudge+5);//
        tft.setTextColor(WHITE);  tft.setTextSize(1);         
        if(switches.circumventTFT == 0){tft.println(*globalReferencePointer[a]);} 

        switch(newChip.pins)
          {
            case 14:nudge=nudge+30;
            break;
            case 16:nudge=nudge+28;
            break;
            case 20:nudge=nudge+26;
            break;
            case 24:nudge=nudge+22;
            break;
          }   
        }
//------------------------------------------Graphics right side pins routine--------------------------------------------//        
        switch(newChip.pins)
        {
          case 14:nudge=210;
          break;
          case 16:nudge=215;
          break;
          case 20:nudge=247;
          break;
          case 24:nudge=254;
          break;
        } 
      
        for(uint8_t a=newChip.pins/2; a<newChip.pins; a++) //7-13
        {               
          newChip.pinFunction.reserve(9);
          newChip.pinFunction = dataFile.readStringUntil('%');
          *globalpinFunctionPointer[a] = newChip.pinFunction; 
          if(newChip.pinFunction == "Input  ") {pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, NAVY, BLUE, WHITE, "", 1); if(switches.circumventTFT == 0){buttonsPinout[a].drawButton();}}//
          if(newChip.pinFunction == "Clock  ") {clock[a] = 1;  buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, BLUE, PURPLE, WHITE, "C", 2); if(switches.circumventTFT == 0){buttonsPinout[a].drawButton();}}//
          if(newChip.pinFunction == "Output ") {outputs[a] = 2; tablet.sortedOutputs[a] = 2; if(switches.circumventTFT == 0){tft.fillCircle(160, nudge+9, outputSize, DARKGREY);}}
          if(newChip.pinFunction == "GND    ") {pinMode (pin[a], OUTPUT); digitalWrite(pin[a], LOW); if(switches.circumventTFT == 0){tft.fillRect(150 + horizontalNudge, nudge , boxSize, boxSize, GREEN);}}//
          if(newChip.pinFunction == "VCC    ") {pinMode (pin[a], OUTPUT); digitalWrite(pin[a], HIGH); if(switches.circumventTFT == 0){tft.fillRect(150 + horizontalNudge, nudge , boxSize, boxSize, RED);}}//      
          if(newChip.pinFunction == "NC     ") {if(switches.circumventTFT == 0){tft.fillRect(150 + horizontalNudge, nudge , boxSize, boxSize, NAVY);}}//
          if(newChip.pinFunction == "In/Out ") {inOut[a] = 1; pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); if(switches.circumventTFT == 0){buttonsPinout[a].drawButton();} *globalpinFunctionPointer[a]= "Input  ";}//
          if(newChip.pinFunction == "Out/In ") {outIn[a] = 1; outputs[a] = 2; tablet.sortedOutputs[a] = 2; if(switches.circumventTFT == 0){tft.fillCircle(160, nudge+9, outputSize, DARKGREY);} *globalpinFunctionPointer[a]= "Output  ";}  
          
  //--------------------------------------References on the right side--------------------------------------------------//        

          *globalReferencePointer[a]= dataFile.readStringUntil('\n');   

          tft.setCursor(175, nudge+5);//
          tft.setTextColor(WHITE);  tft.setTextSize(1);//pin reference 
          if(switches.circumventTFT == 0){tft.println(*globalReferencePointer[a]);}     
          switch(newChip.pins)
          {
            case 14:nudge=nudge-30;
            break;
            case 16:nudge=nudge-28;
            break;
            case 20:nudge=nudge-26;
            break;
            case 24:nudge=nudge-22;
            break;
          }      
        }         
//-------------------------------------Graphics Menu and clock/mux button toggle----------------------------------------// 
        switches.clockmenuToggle = 0; switches.muxdemuxMenuToggle = 0; switches.muxdemuxFlag = 0;
        for (uint8_t i = 0; i < *pinNumberRoutingPointer; i++)
        {
          if(clock[i] != 0) switches.clockmenuToggle = 1;//scanning the IOs for clock inputs
          if(inOut[i] != 0) switches.muxdemuxMenuToggle = 1;//scanning the IOs for mux IO
          if(outIn[i] != 0) switches.muxdemuxMenuToggle = 1;//scanning the IOs for mux COM IO
        }
        if(switches.circumventTFT == 0)
        {
          if(switches.clockmenuToggle == 0 && switches.muxdemuxMenuToggle == 0)
          {
            buttonsMenus[0].initButton(&tft, 40, buttonYaxis, 70, buttonHeight, WHITE, GREY, BLACK, "Menu", 2);
            buttonsMenus[0].drawButton();
            buttonsMenus[3].initButton(&tft, 200, buttonYaxis, 70, buttonHeight, BLACK, BLUE, WHITE, "Table", 2);
            buttonsMenus[3].drawButton();
          }
          if(switches.clockmenuToggle == 1)
          {
            buttonsMenus[0].initButton(&tft, 40, buttonYaxis, 70, buttonHeight, WHITE, GREY, BLACK, "Menu", 2);
            buttonsMenus[0].drawButton();
            buttonsMenus[2].initButton(&tft, 120, buttonYaxis, 70, buttonHeight, BLUE, BLACK, WHITE, "Clock", 2);
            buttonsMenus[2].drawButton();
            buttonsMenus[3].initButton(&tft, 200, buttonYaxis, 70, buttonHeight, BLACK, BLUE, WHITE, "Table", 2);
            buttonsMenus[3].drawButton();
          }
          if(switches.muxdemuxMenuToggle == 1)
          {
            buttonsMenus[0].initButton(&tft, 40, buttonYaxis, 70, buttonHeight, WHITE, GREY, BLACK, "Menu", 2);
            buttonsMenus[0].drawButton();
            buttonsMenus[2].initButton(&tft, 120, buttonYaxis, 70, buttonHeight, WHITE, NAVY, WHITE, "In/Out", 2);
            buttonsMenus[2].drawButton();
            buttonsMenus[3].initButton(&tft, 200, buttonYaxis, 70, buttonHeight, BLACK, BLUE, WHITE, "Table", 2);
            buttonsMenus[3].drawButton();
          }
        }     
       dataFile.close(); break;//This speeds up the process by preventing the program from reading the IC's it's not looking for after finding the right one
      } //When the right chip has been identified routine ends                             
    } //while (dataFile.available()) ends 
    if(numberofIcs == 0)//if the IC chosen by user isn't in the database
    {
      *pinNumberRoutingPointer = 1;//Otherwise it doesn't go into the loop where the menubutton is in screenStatus 3
      tft.setCursor(40, 100);
      tft.setTextColor(RED);  tft.setTextSize(3);
      tft.println(F("IC not in"));
      tft.setCursor(50, 130);
      tft.println(F("Database")); 
      buttonsMenus[0].initButton(&tft, 120, 290, 200, buttonHeight, WHITE, GREY, BLACK, "Main menu", 3);
      buttonsMenus[0].drawButton();
    }
  }//if (dataFile)
}
//----------------------------------------------------------------------------------------------------------------------//
// -------------------------------------------------Output graphics-----------------------------------------------------// 
//----------------------------------------------------------------------------------------------------------------------//
void rectTest(int outputState, int mover)
{
  switch(outputState)
  {
  case 0: tft.fillCircle(80, ((mover+1)*pinSpacer)+shifterLeft , outputSize, DARKGREY);
  break; 
  case 1: tft.fillCircle(80, ((mover+1)*pinSpacer)+shifterLeft , outputSize, WHITE);
  break;
  case 2: tft.fillCircle(160, shifterRight-(mover+1)*pinSpacer , outputSize, DARKGREY);
  break;
  case 3: tft.fillCircle(160, shifterRight-(mover+1)*pinSpacer , outputSize, WHITE);
  break;
  }  
}
//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------Mux/Demux functions-----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void muxIOswitchRefresh()
{  
  word nudge; 
//----------Clearing arrays, variables and setting scale for pins graphical layout depending on number of pins----------//          
  for (uint8_t i = 0; i < *pinNumberRoutingPointer; i++) 
  {
    if(inOut[i] == 1 || outIn[i] == 1)
    {
    digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);  
    buttonsPinout[i] = buttonsClearPinout[0]; 
    }
  }
  if(*pinNumberRoutingPointer == 14) {nudge=30; pinSpacer = 30; shifterLeft = 9;   shifterRight = 459;}//So the pins are evenly spaced depending on number of pins on each chip
  if(*pinNumberRoutingPointer == 16) {nudge=20; pinSpacer = 28; shifterLeft = 1;   shifterRight = 476;} 
  if(*pinNumberRoutingPointer == 20) {nudge=13; pinSpacer = 26; shifterLeft = -3;  shifterRight = 542;}
  if(*pinNumberRoutingPointer == 24) {nudge=13; pinSpacer = 22; shifterLeft = 0;   shifterRight = 550;}
//---------------------------------------Graphics left side pins routine------------------------------------------------//        
  for(uint8_t a=0; a<*pinNumberRoutingPointer/2; a++)
  {                                
  if(inOut[a] == 1) {buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, BLACK, BLACK, BLACK, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//erase previous graphics
  if(outIn[a] == 1) {tft.fillCircle(80, nudge+9, outputSize, BLACK); *globalpinFunctionPointer[a]= "Output  ";}// erase previous graphics         
  if(inOut[a] == 1) {buttonsPinout[a].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1); outputs[a] = 1; tablet.sortedOutputs[a] = 1; tft.fillCircle(80, nudge+9, outputSize, DARKGREY); *globalpinFunctionPointer[a]= "Output  ";}//here the Com ports are inputs 
  if(outIn[a] == 1) {buttonsPinout[a].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1);buttonsPinout[a] = buttonsClearPinout[0]; outputs[a] = 0; tablet.sortedOutputs[a] = 0; pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//            
  
  if(*pinNumberRoutingPointer == 14) {nudge=nudge+30;}
  if(*pinNumberRoutingPointer == 16) {nudge=nudge+28;}
  if(*pinNumberRoutingPointer == 20) {nudge=nudge+26;}
  if(*pinNumberRoutingPointer == 24) {nudge=nudge+22;}
  }
//----------------------------------------Graphics right side pins routine----------------------------------------------//          
  if(*pinNumberRoutingPointer == 14) {nudge=210;}
  if(*pinNumberRoutingPointer == 16) {nudge=215;}
  if(*pinNumberRoutingPointer == 20) {nudge=247;}
  if(*pinNumberRoutingPointer == 24) {nudge=254;}
  for(uint8_t a=*pinNumberRoutingPointer/2; a<*pinNumberRoutingPointer; a++) //7-13
  {                   
  if(inOut[a] == 1) {buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, BLACK, BLACK, BLACK, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//erase previous graphics
  if(outIn[a] == 1) {tft.fillCircle(160, nudge+9, outputSize, BLACK); *globalpinFunctionPointer[a]= "Output  ";}// erase previous graphics      
  if(inOut[a] == 1) {buttonsPinout[a].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1);  outputs[a] = 2; tablet.sortedOutputs[a] = 2; tft.fillCircle(160, nudge+9, outputSize, DARKGREY); *globalpinFunctionPointer[a]= "Output  ";}//here the Com ports are inputs 
  if(outIn[a] == 1) {outputs[a] = 0; tablet.sortedOutputs[a] = 0; pinMode(pin[a], OUTPUT ); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//  

  if(*pinNumberRoutingPointer == 14) nudge=nudge-30;
  if(*pinNumberRoutingPointer == 16) nudge=nudge-28;  
  if(*pinNumberRoutingPointer == 20) nudge=nudge-26;  
  if(*pinNumberRoutingPointer == 24) nudge=nudge-22;      
  }   
}
void demuxIOswitchRefresh()
{
   word nudge;
//----------Clearing arrays, variables and setting scale for pins graphical layout depending on number of pins----------//          
  for (uint8_t i = 0; i < *pinNumberRoutingPointer; i++) 
  {
    if(inOut[i] == 1 || outIn[i] == 1)
    {
    digitalWrite(pin[i], LOW); pinMode(pin[i], INPUT_PULLUP);  
    buttonsPinout[i] = buttonsClearPinout[0]; 
    }
  }
  if(*pinNumberRoutingPointer == 14) {nudge=30; pinSpacer = 30; shifterLeft = 9;   shifterRight = 459;}//So the pins are evenly spaced depending on number of pins on each chip
  if(*pinNumberRoutingPointer == 16) {nudge=20; pinSpacer = 28; shifterLeft = 1;   shifterRight = 476;}
  if(*pinNumberRoutingPointer == 20) {nudge=13; pinSpacer = 26; shifterLeft = -3; shifterRight = 542;}
  if(*pinNumberRoutingPointer == 24) {nudge=13; pinSpacer = 22; shifterLeft = 0;   shifterRight = 550;}
//----------------------------------------Graphics left side pins routine-----------------------------------------------//        
  for(uint8_t a=0; a<*pinNumberRoutingPointer/2; a++)
  {                                 
  if(outIn[a] == 1) {buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, BLACK, BLACK, BLACK, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//erase previous graphics
  if(inOut[a] == 1) {tft.fillCircle(80, nudge+9, outputSize, BLACK); *globalpinFunctionPointer[a]= "Output  ";}// erase previous graphics       
  if(outIn[a] == 1) {buttonsPinout[a].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1);  outputs[a] = 1; tablet.sortedOutputs[a] = 1; tft.fillCircle(80, nudge+9, outputSize, DARKGREY); *globalpinFunctionPointer[a]= "Output  ";}//here the Com ports are inputs 
  if(inOut[a] == 1) {outputs[a] = 0; tablet.sortedOutputs[a] = 0; pinMode(pin[a], OUTPUT); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 80, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//        

  if(*pinNumberRoutingPointer == 14) {nudge=nudge+30;}
  if(*pinNumberRoutingPointer == 16) {nudge=nudge+28;}
  if(*pinNumberRoutingPointer == 20) {nudge=nudge+26;}
  if(*pinNumberRoutingPointer == 24) {nudge=nudge+22;}
  }
//----------------------------------------Graphics right side pins routine----------------------------------------------//          
  if(*pinNumberRoutingPointer == 14) {nudge=210;}
  if(*pinNumberRoutingPointer == 16) {nudge=215;}
  if(*pinNumberRoutingPointer == 20) {nudge=247;}
  if(*pinNumberRoutingPointer == 24) {nudge=254;}
  
  for(uint8_t a=*pinNumberRoutingPointer/2; a<*pinNumberRoutingPointer; a++) //7-13
  {                 
  if(outIn[a] == 1) {buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, BLACK, BLACK, BLACK, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//erase previous graphics
  if(inOut[a] == 1) {tft.fillCircle(160, nudge+9, 10, BLACK); *globalpinFunctionPointer[a]= "Output  ";}// erase previous graphics       
  if(outIn[a] == 1) {buttonsPinout[a].initButton(&tft, 0, 0, 0, 0, BLACK, BLACK, BLACK, "", 1);  outputs[a] = 2; tablet.sortedOutputs[a] = 2; tft.fillCircle(160, nudge+9, outputSize, DARKGREY); *globalpinFunctionPointer[a]= "Output  ";}//here the Com ports are inputs 
  if(inOut[a] == 1) {outputs[a] = 0; tablet.sortedOutputs[a] = 0; pinMode(pin[a], OUTPUT ); digitalWrite(pin[a], LOW); buttonsPinout[a].initButton(&tft, 160, nudge+10, buttonSize, buttonSize, WHITE, DARKGREY, WHITE, "", 1); buttonsPinout[a].drawButton(); *globalpinFunctionPointer[a]= "Input  ";}//       
        
  if(*pinNumberRoutingPointer == 14) nudge=nudge-30;
  if(*pinNumberRoutingPointer == 16) nudge=nudge-28;
  if(*pinNumberRoutingPointer == 20) nudge=nudge-26;
  if(*pinNumberRoutingPointer == 24) nudge=nudge-22;         
  }   
}
void clockAndmuxButtons()
{
// ---------------------------------------clock and IO switch toggle buttons--------------------------------------------//            
  if(switches.clockmenuToggle == 0)
  {
    if (buttonsMenus[3].justPressed())//truth table screen
    { 
      screenStatus = 69; 
    }  
  }
  if(switches.clockmenuToggle == 1)
  {     
    if (buttonsMenus[2].justPressed())
    { 
      if(switches.clockbuttonStatus == 0){switches.clockToggle = 1;buttonsMenus[2].drawButton(true); }//Clock button ON 
      if(switches.clockbuttonStatus == 1){switches.clockToggle = 0;buttonsMenus[2].drawButton();}//Clock button OFF
    }
    if (buttonsMenus[2].justPressed())
    { 
      if(switches.lastclockbuttonStatus == 0){switches.clockbuttonStatus = 1; } //flip the bit for toggle effect
      if(switches.lastclockbuttonStatus == 1){switches.clockbuttonStatus = 0; } //flip the bit for toggle effect 
    }  
    if (buttonsMenus[3].justPressed())//truth table screen
    { 
      screenStatus = 69; 
    }                  
  }             
  if(switches.muxdemuxMenuToggle == 1)
  {
    if (buttonsMenus[2].justPressed()) 
    {
      if(switches.muxdemuxToggleStatus == 0){switches.muxdemuxToggle = 1; switches.muxdemuxFlag = 1;buttonsMenus[2].drawButton(true);}//Mux Demux button ON 
      if(switches.muxdemuxToggleStatus == 1){switches.muxdemuxToggle = 0; switches.muxdemuxFlag = 1;buttonsMenus[2].drawButton();}//Mux Demux button OFF      
      if(switches.lastmuxdemuxToggleStatus == 0){switches.muxdemuxToggleStatus = 1;} //flip the bit for toggle effect
      if(switches.lastmuxdemuxToggleStatus == 1){switches.muxdemuxToggleStatus = 0;} //flip the bit for toggle effect 
    }
       
    if (buttonsMenus[3].justPressed())//truth table screen
    { 
      screenStatus = 69; 
    }   
  }
  if(switches.muxdemuxFlag == 1 && switches.muxdemuxToggle == 1 && switches.muxdemuxMenuToggle == 1)  //lastoutputState = 0; 
  {     
    muxIOswitchRefresh();//Switches the bidirectional inputs to outputs     
    switches.muxdemuxFlag = 0;//so it only refreshes the pinouts once per mode toggle                               
  }        
  if(switches.muxdemuxFlag == 1 && switches.muxdemuxToggle == 0 && switches.muxdemuxMenuToggle == 1)  //lastoutputState = 0; 
  { 
    demuxIOswitchRefresh();  //Switches the bidirectional inputs back to outputs                                             
    switches.muxdemuxFlag = 0;//so it only refreshes the pinouts once per mode toggle  
  }  
}
