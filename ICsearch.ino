//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------IC identification function---------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void autoSearch(int pins)
{
  tft.fillScreen(BLACK); //"clears" the screen
  tft.setCursor(10, 10);
  tft.setTextColor(BLUE);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(12, 12);
  tft.setTextColor(GREY);  tft.setTextSize(4);
  tft.println(F("IC TESTER"));
  tft.setCursor(22, 46);
  tft.setTextColor(GREY);  tft.setTextSize(3);
  tft.println(F("Identifying"));
  tft.setCursor(20, 45);
  tft.setTextColor(WHITE);  tft.setTextSize(3);
  tft.println(F("Identifying"));
  tft.setTextColor(GREY);tft.setTextSize(2);
  tft.setCursor(10, 100); //
  tft.print(F("Number of pins: ")); tft.println(pins); 
  File dataFile = SD.open(fname); //open the test database
  int t = 120; //
  String nameAuto[10];
  numberofIcs = 0;//Used here to indicate if the chip chosen by user is in the database
  String newCase; //
  chip newChip; //Ic's being tested  
  uint8_t ICpins;//In the autotest sequence the only thing the program has to go on is how many pins the chip has so it uses all the tests for appropriate number of pins on a chip under test.
  boolean result; //if the number of pin matches 
  chip ans[10];//here the program stores all the chips that passed the test.
  //IC possibles[24];
  pinNumberRouting = pins;
  if (dataFile) //here the reading/comparing of SD data begins
  {    
    uint8_t count = 0; //holds the number of matches
    pin = pins;

    switch(pins)
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

    while (dataFile.available())
    {
      dataFile.readStringUntil('$');
      newChip.num .reserve(6);
      newChip.num = dataFile.readStringUntil('\n'); //here it stores the number of each IC it reads
      newChip.name = dataFile.readStringUntil('\n'); //this is where the description of the IC is stored
      ICpins = dataFile.readStringUntil('\n').toInt(); //here it stores the number of pins each IC it reads in the datafile so it can be matched with the user assigned number of pins

      if(switches.fastMode == 0) {tft.setTextColor(BLUE);tft.setTextSize(3); tft.setCursor(80, 75); tft.println(newChip.num);}  //What Ic's are beeing tested, clearing them is further down in code        
    //  failCount = 0;
      if (ICpins == pins) //comparing the number of pins on IC read and the user chosen one
      {
       
        result = true; //the number of pins matches
        while (dataFile.peek() != '$') //the testing of an IC starts
        {
          newCase.reserve(25);
          newCase = dataFile.readStringUntil('\n'); ;//Applies test sequences found in the database.txt file        
          newCase.trim();
          
          if (testCase(newCase, ICpins) == false)//If the chip failed the test it continues
          {
            result = false;
           // if(failCount == 1){possibles[count++] = newChip;}
            break;
          }
        }        
        if (result == true)//The chips that passed the tests
        {      
          newChip.num.reserve(6);
          chipDescription = newChip.name;//storing description of IC that passed a test
          ans[count++] = newChip;  //here we store the informations of matched ICs                
          tft.setTextColor(WHITE);tft.setTextSize(2);//What IC matched          
        
          if(!isDigit(newChip.num[4]))//this is a position for those chips with 4 numbers in their ID
            {
              tft.setCursor(100, t); //position for the chips that have passed a test
            }
          if(isDigit(newChip.num[4]))//this is a position for those chips with 5 numbers in their ID
            {
              tft.setCursor(93, t);
            }            
          tft.println(newChip.num);   
          t=t+20;       
        }
       
       // Serial.println("Test Completed"); //each chip that has the right amount of pins gets tested and this is after each one
      }            
      if(switches.fastMode == 0){ tft.setTextColor(BLACK);tft.setTextSize(3); tft.setCursor(80, 75); tft.println(newChip.num);}//clearing Ic's that have been tested
    }
    //delete(pin);
    dataFile.close();////here the reading/comparing test phase of SD data ends
   
    if (count != 0)
    {
      numberofIcs = count;//to store number of ICs matched in every scope   
        for(uint8_t r=0; r<count; r++) 
     {
      numberAuto[r].reserve(5);
      numberAuto[r+1] = ans[r].num;      
      nameAuto[r+1] = ans[r].name; 
     }
    }
  }
  else
  {
    Serial.print(F("error opening ")); Serial.println(fname);
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