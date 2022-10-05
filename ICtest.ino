//----------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------Testing of IC functions---------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void repeatTest(const String& number, int numberTest)
{
  File dataFile = SD.open(fname);// open the file. note that only one file can be open at a time.
  word good = 0;//keeps track of tests that the chip passed
  word bad = 0;//keeps track of the tests that the chip failed
  byte failedTests = 1;
  word fail = 0;
  byte goodTest = 1;
  word passed = 0;
  word testLoop = 0; //number of tests chosen by user
  String buffer;//the IC numbers on the SD that the program then compares to the IC selected by user input
  String newCase;
  unsigned long testPosition = 0;//this keeps track of test lines on the SD card
  unsigned long hardPosition = 0;//this keeps track of start of test lines on the SD card
  unsigned long *testPositionPointer = &testPosition;//this keeps track of test lines on the SD card
  unsigned long *hardPositionPointer = &hardPosition; ;//this keeps track of start of test lines on the SD card
  byte testSize = 2;  
  byte testPlacer = 0;
  String linekeepTrack;
  linecount = 1;//keeps count on the number of test lines for a IC under test
  chip newChip;
  //int pins;
  boolean result = true;
  boolean resultFix = true;//Needed to fix the transition from a failed to passed test
  
  byte testSequenceLenght = 1;
  byte *testSequenceLenghtPointer;
  testSequenceLenghtPointer = &testSequenceLenght; 
  byte testlineError[24];
  byte *testlineErrorPointer[24];
  for(uint8_t w=0; w<24; w++){testlineErrorPointer[w] = &testlineError[w];}
  newCase.reserve(25);
  //----------Onsceen text-----------------------
  tft.fillScreen(BLACK);//"Clear" the screen
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(40, 45);
  tft.setTextColor(GREY);  tft.setTextSize(3);
  tft.println(F("Loop Test"));
  tft.setCursor(60, 100);
  tft.setTextColor(GREY);  tft.setTextSize(2);
  tft.println(F("Line count"));
  for (uint8_t i = 0; i < 24; i++) {storeErrorPlace[i] = clearArray[0]; storeErrorLines[i] = clearArray[0]; } //Clear the Pin number error assignments 
  for (uint8_t i = 0; i < 24; i++) {storeTestLines[i] ="";}//need to change the dependency on 24 as max need to tie it to a reference 

// if the file is available, read from it:
  if (dataFile)
  {   
    while (dataFile.available())
    {
      result = true;
      dataFile.readStringUntil('$');
      buffer.reserve(6);
      buffer = dataFile.readStringUntil('\n');//stores the IC number the program reads from the SD
      buffer.trim();   
      if (number == buffer)//Compares the IC number read from the SD to the IC number chosen by the user
      {
        newChip.num.reserve(6);
        newChip.num = buffer; //stores the name of the chip example 4xxx or 74xx
        numberofIcs = 1; //This is used to indicate that the chip chosen by the user has been found in the database if this value is 0 then the chip isn´t in the database
        chipDescriptionPointer = &newChip.name; 
        *chipDescriptionPointer = dataFile.readStringUntil('\n'); //this is where the description of the IC is stored
        *pinNumberRoutingPointer = dataFile.readStringUntil('\n').toInt(); //this stores the number of pins the chosen IC has
        //pin = pins;
        //*pinNumberRoutingPointer = pins; //so I can use the appropiate number of pins outside the scope
        
        switch(*pinNumberRoutingPointer)
          {
            case 14:pin = PIN14; testPlacer = 20; testSize = 2;//calls an appropiate array with the pin assignment of the microcontroller
            break;
            case 16:pin = PIN16; testPlacer = 15; testSize = 2;//Also sets the graphical shifting for the failed test routines onscreen and size as the 24 pin test is too big for the screen  
            break;
            case 20:pin = PIN20; testPlacer = 5;  testSize = 2;
            break;
            case 24:pin = PIN24; testPlacer = 40; testSize = 1;
            break;
          } 
        
        tft.setTextColor(BLUE);tft.setTextSize(3);
        
        if(!isDigit(buffer[4]))//this is a position for those chips with 4 numbers in their ID
         {
          tft.setCursor(85, 75); 
         }          
        if(isDigit(buffer[4]))//this is a position for those chips with 4 numbers in their ID
         {
          tft.setCursor(75, 75); 
         } 
        tft.println(buffer); //What Ic is beeing tested
        
        if(switches.fastMode == 0)
        {
          tft.fillRect(164, 150 , 60, 20, BLACK); //
          tft.setCursor(10, 150);
          tft.setTextColor(RED);  tft.setTextSize(2);
          tft.print(F("Failed Tests:")); tft.println(fail);        
          
          tft.fillRect(164, 170 , 60, 20, BLACK);
          tft.setCursor(10, 170);
          tft.setTextColor(GREEN);  tft.setTextSize(2);
          tft.print(F("Passed Tests:")); tft.println(good);
          
          tft.fillRect(164, 190 , 60, 20, BLACK);
          tft.setCursor(10, 190);
          tft.setTextColor(RED);  tft.setTextSize(2);
          tft.print(F("Failed Lines:")); tft.println(bad);
          
          tft.fillRect(164, 210 , 60, 20, BLACK);
          tft.setCursor(10, 210);
          tft.setTextColor(GREEN);  tft.setTextSize(2);
          tft.print(F("Passed Lines:")); tft.println(good);
        }
        if(switches.fastMode == 1){tft.setTextSize(3); tft.setTextColor(GREY);tft.setCursor(33, 211); tft.println("TESTING.."); tft.setTextColor(WHITE);tft.setCursor(30, 210);tft.println("TESTING..");}
        
        *testPositionPointer = dataFile.position(); //this variable gets set to start of each line of the test routines, here it´s set to the start of the test sequence
        *hardPositionPointer = dataFile.position(); //this keeps track of the start position of the chosen IC        
        
        while (dataFile.peek() != '$')//here the program runs through the test sequence until the next chip symbol $ is read
        {
          dataFile.readStringUntil('\n'); //this loop counts the number of lines the test code for the chosen IC has
          linecount++;//the count is keept track of here
        }
        dataFile.seek(*hardPositionPointer);//this variable stores the start location of the test code 
        testLoop = 0;//clears the number of times the loop has been executed
        if(numberTest == 0){numberTest = 10; switches.endlessLoopSwitch = 1;}//this makes sure that the numberTest variable isn't 0 when entering the while loop
//------------------------------------------------------------------Testing starts-----------------------------------------------------------------------------------------
        while(testLoop < numberTest*linecount) //testloop is the number of times this loop has been executed and the numberTest is the number of tests chosen by the user
        {  
                              
          if(*testSequenceLenghtPointer == linecount){failedTests = 1; goodTest = 1; *testSequenceLenghtPointer = 1;} //this is so the test sequence starts over at the right position on the SD card 

          switch(*testSequenceLenghtPointer)
          {
            case 1: *testPositionPointer = *hardPositionPointer - *pinNumberRoutingPointer - 2;//this is so the test sequence starts over at the right position on the SD card 
          }
          dataFile.seek(*testPositionPointer + (*pinNumberRoutingPointer + 2) * *testSequenceLenghtPointer);//this "scrolls/jumps" through the test lines
          
          newCase = dataFile.readStringUntil('\n'); //Since the original coder made it so that the testcase can only handle one line of test being feed to it at time here it's disected line by line                                     
         
          switch (testCase2(newCase, *pinNumberRoutingPointer))
          {
            case false:  bad++;//keeps track of how many times the test failed                        
            testlineErrorPointer[*testSequenceLenghtPointer] = *testSequenceLenghtPointer;  //here we store the linenumber of the failed tests                       
            storeTestLines[*testSequenceLenghtPointer].reserve(24);
            storeTestLines[*testSequenceLenghtPointer] = newCase;
            failedTests++; goodTest = 1;
            if(failedTests == linecount){fail++; failedTests == 1;}
            switch(switches.fastMode)                        
            {
              case 0:
              tft.fillRect(164, 190 , 70, 20, BLACK);
              tft.setCursor(166, 190);
              tft.setTextColor(RED);  tft.setTextSize(2);
              tft.println(bad);//prints on screen how many lines have failed
              
              tft.fillRect(164, 150 , 70, 20, BLACK);
              tft.setCursor(166, 150);
              tft.setTextColor(RED);  tft.setTextSize(2);
              tft.println(fail);//prints on screen how many times the test has failed
              break;
            }
            resultFix = false;
            break;          
          }   
          
          switch(resultFix)
          {
            case true:
            good++;//keeps track of how many times the IC passed the test  
            goodTest++; 
            if(goodTest == linecount){passed++;}
            switch(switches.fastMode)                        
            {
              case 0:
              tft.fillRect(164, 210 , 70, 20, BLACK);
              tft.setCursor(166, 210);
              tft.setTextColor(GREEN);  tft.setTextSize(2);
              tft.println(good);//prints on screen how many times the IC has passed the test       
              
              tft.fillRect(164, 170 , 70, 20, BLACK);
              tft.setCursor(166, 170);
              tft.setTextColor(GREEN);  tft.setTextSize(2);
              tft.println(passed);//prints on screen how many times the IC has passed the test     
              break;
            }
            break;              
           }                                                                              
          resultFix = true; //this makes sure that a failed test followed by a passed test don't get counted together in the same run of the loop        
          testSequenceLenght++; // the numer of lines in the test code, gets incremented by one in each run of the loop    
          switch(switches.fastMode)                        
          {
            case 0:        
            tft.setCursor(110, 120);
            tft.setTextColor(BLACK);  tft.setTextSize(2);//here we clear the onscreen count of the tests processed
            tft.println(testLoop);
            tft.setCursor(110, 120);
            tft.setTextColor(BLUE);  tft.setTextSize(2);//here we display onscreen the number of tests processed 
            tft.println(testLoop);
            break;
          }  
          switch(switches.endlessLoopSwitch)                        
          {
            case 0:  testLoop++; //increments the loop count by one unless the user choses 0 as number of tests than it runs until the variables max out     
            break;
          }                       
          getTouch();//this is so the user can stop the test at any time
        if(switches.status == 1) break;//this is so the user can stop the test at any time
          // take the measurement          
        } 
        Serial.print("Cycles: ");
        Serial.println(cycles2 - 1);
        Serial.print("Microseconds: ");
        Serial.println((float)(cycles2 - 1) / 16);
       dataFile.close(); break; //breaks out of the loop after finding the right chip and testing otherwise it runs through the rest of the ICs                           
      }    
    }    
  looptestResult(*chipDescriptionPointer, good, bad, linecount, fail, passed);  //here it goes to the onscreen information after the test
  } 
  else
  {
    //here below is onscreen SD card error routine
    dataFile.close();
    SD.begin(10, 11, 12, 13);
    tft.setCursor(5, 140);
    tft.setTextColor(RED);  tft.setTextSize(4);
    tft.print(F("ERROR:"));
    tft.setTextColor(MAGENTA);
    tft.println(F("SD CARD"));
    tft.setCursor(5, 300);
    tft.setTextColor(RED);  tft.setTextSize(2);
    tft.println(F("Touch for MAIN MENU"));
    while(switches.status == 0)
    {    
    getTouch();        
    }    
    flow();  
  } 
}
//--------------------------------------------Error Information Routine-----------------------------------------------------
void errorInfo()
{
  File dataFile = SD.open(fname2);
  chip newChip;
  unsigned long testPosition = 0;//this keeps track of test lines on the SD card
  unsigned long *testPositionPointer = &testPosition;//this keeps track of test lines on the SD card
  newChip.num.reserve(6);
  if (dataFile)
  {   
   if(numberRouting.toInt() > 7399 && numberRouting.toInt() < 7500 || numberRouting.toInt() > 74000){dataFile.seek(33700);}// this line here speeds up the process of looking for the 7400 series,could cause problems if a IC of the cd4000 series is removed!! 
    while (dataFile.available())
    {           
//-------------------------------------------Looking for the right chip routine----------------------------------------------  
      dataFile.readStringUntil('$');    
      *testPositionPointer = dataFile.position(); //this variable gets set to start of each line of the test routines, here it´s set to the start of the test sequence         
      newChip.num = dataFile.readStringUntil('\n'); //chip id   
      if(newChip.num.toInt() != numberRouting.toInt()){dataFile.seek(*testPositionPointer + 240);}//this "scrolls/jumps" through the test lines        
//-----------------------------When the right chip has been identified routine------------------------------------
      if(newChip.num.toInt() == numberRouting.toInt()) //comparing IC number to the one chosen by the user
      {        
        newChip.name = dataFile.readStringUntil('\n'); // chip description
        newChip.pins = dataFile.readStringUntil('\n').toInt(); //pin count            
        pin = new int[newChip.pins]; 
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
    
       *globalReferencePointer[24] = "";   
        for(uint8_t a=0; a<newChip.pins; a++)
        { 
          *globalpinFunctionPointer[a] = dataFile.readStringUntil('%'); 
          *globalReferencePointer[a] = dataFile.readStringUntil('\n');
        }
        dataFile.close(); break;
      }
    }
  }  
  byte d = 0;
  tft.fillScreen(BLACK);//"Clear" the screen
  tft.setTextSize(1);
  tft.setCursor(5, 5);
  tft.setTextColor(WHITE);  
  tft.print(F("#   ChipIO   !   Name    ArduinoIO  *"));

  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)
  {
    int pinMode = 0;
    if(*pinNumberRoutingPointer == 14){pinMode = getPinMode(PIN14[u]);}
    if(*pinNumberRoutingPointer == 16){pinMode = getPinMode(PIN16[u]);}
    if(*pinNumberRoutingPointer == 20){pinMode = getPinMode(PIN20[u]);}
    if(*pinNumberRoutingPointer == 24){pinMode = getPinMode(PIN24[u]);}

    //pinNumbers
    tft.setTextColor(GREEN);
    tft.setCursor(5, 10+(10*(u+1))); tft.print(u+1); 
    tft.setTextColor(WHITE);
    globalReference[u].reserve(8);
    globalReference[u].trim();
    if(*globalReferencePointer[u] == "+V"){tft.setTextColor(RED);}
    if(*globalReferencePointer[u] == "0V"){tft.setTextColor(GREEN);}
    tft.setCursor(100, 10+(10*(u+1))); tft.print(*globalReferencePointer[u]);
    //functiontype    
    if(*globalpinFunctionPointer[u] == "Output "){tft.setTextColor(YELLOW);}
    if(*globalpinFunctionPointer[u] == "Input  "){tft.setTextColor(NAVY);}
    if(*globalpinFunctionPointer[u] == "VCC    "){tft.setTextColor(RED);}
    if(*globalpinFunctionPointer[u] == "GND    "){tft.setTextColor(GREEN);}
    if(*globalpinFunctionPointer[u] == "Clock  "){tft.setTextColor(MAROON);}
    if(*globalpinFunctionPointer[u] == "In/Out "){tft.setTextColor(ORANGE);}
    if(*globalpinFunctionPointer[u] == "Out/In "){tft.setTextColor(PURPLE);}
    if(*globalpinFunctionPointer[u] == "NC     "){tft.setTextColor(OLIVE);}
    tft.setCursor(30, 10+(10*(u+1))); tft.print(*globalpinFunctionPointer[u]);
    //Errortypes
   // storeErrorLines[u].reserve(1);
    if(storeErrorLines[u]== "L"){tft.setTextColor(PURPLE);}
    if(storeErrorLines[u]== "H"){tft.setTextColor(GREEN);}
    if(storeErrorLines[u]== "0"){tft.setTextColor(BLACK);}
     tft.setCursor(220, 10+(10*(u+1))); tft.print(storeErrorLines[u]);
    //PinType
    if (pinMode == 0) 
    {    
      tft.setTextColor(CYAN);
      tft.setCursor(160, 10+(10*(d+1))); tft.print(F("INPUT"));       
    }
    if (pinMode == 1) 
    {
      tft.setTextColor(YELLOW);
      tft.setCursor(160, 10+(10*(d+1))); tft.print(F("OUTPUT"));  
    }
    d++;
    //Errors
    if(storeErrorPlace[u] == 1)
    {
      tft.setTextColor(RED);
      tft.setCursor(80, 10+(10*(u+1))); tft.print(F("X"));        
    }  
  }
  buttonsMenus[0].initButton(&tft, 40, 300, 70, 40, WHITE, GREY, BLACK, "Menu", 2);
  buttonsMenus[0].drawButton(); //back to main menu from autosearch result
  buttonsMenus[3].initButton(&tft, 120, 300, 70, 40, BLACK, GREY, BLACK, "Data", 2);
  buttonsMenus[3].drawButton(); 
  buttonsMenus[4].initButton(&tft, 200, 300, 70, 40, WHITE, BLACK, GREY, "Test", 2);
  buttonsMenus[4].drawButton(); 
}
//-------------------------------------------Raw Data Routine-------------------------------------------------------------
void rawData()
{

  String errorLine;
  String errorPlace;
  byte c = 0;
  byte linePosition;
  if(*pinNumberRoutingPointer == 14){linePosition = 80;}
  if(*pinNumberRoutingPointer == 16){linePosition = 75;}
  if(*pinNumberRoutingPointer == 20){linePosition = 62;}
  if(*pinNumberRoutingPointer == 24){linePosition = 50;}

  tft.fillScreen(BLACK);//"Clear" the screen
  tft.setTextColor(CYAN);  
  tft.setTextSize(1);
  tft.setCursor(40, 10);
  tft.print(F("Pin inconsistant with test = 1"));
  tft.setCursor(10, 20);
  tft.setTextColor(GREEN);
  tft.print(F("Logic inconsistant with test 5V=H 0V=L"));
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)
  {
    tft.setTextColor(CYAN);  
    tft.setCursor(linePosition+(6*u), 30); 
    tft.print(storeErrorPlace[u]);
    errorPlace.reserve(25);
    errorPlace += storeErrorPlace[u];// summing the array into a string to be able to compare with another string
    tft.setTextColor(GREEN);
    tft.setCursor(linePosition+(6*u), 40);
    storeErrorLines[u].reserve(24);
    tft.println(storeErrorLines[u]);
   errorLine.reserve(25);
    errorLine += storeErrorLines[u];
  } 
  String testLinesArray[24];
  tft.setTextColor(RED);  
  tft.setTextSize(2);
  tft.setCursor(50, 50);
  tft.print(F("Failed tests"));
  tft.setTextSize(1);

  for(uint8_t t=0; t<linecount; t++)
  {  
    if(storeTestLines[t] !="")
    {
      tft.setTextColor(GREEN);
      tft.setCursor(linePosition-20, (10*(c+1))+70); tft.print(t); //position and the line number of failed tests
      for(uint8_t o=0; o<*pinNumberRoutingPointer; o++)
      {
        testLinesArray[o] = storeTestLines[t].charAt(o); 
      } 
      tft.setCursor(linePosition, (10*(c+1))+70);//position for the failed tests
      for(uint8_t r=0; r<*pinNumberRoutingPointer; r++)
      {
        tft.setTextColor(YELLOW);
        if(storeErrorLines[r] =="H" && testLinesArray[r] =="L") {tft.setTextColor(RED);}  
        if(storeErrorLines[r] =="L" && testLinesArray[r] =="H") {tft.setTextColor(RED);}   
        tft.print(testLinesArray[r]);
      }
    c++;
    }
  }
  buttonsMenus[0].initButton(&tft, 120, 300, 160, 40, WHITE, GREY, BLACK, "MainMenu", 3);
  buttonsMenus[0].drawButton(); //back to main menu from autosearch result
}
