//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------------Truthtable/diagram functions--------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void truthtableData()
{
  String *refBufferPointer[24];  
  for(uint8_t w=0; w<24; w++)
  {  
    refBufferPointer[w] = &tablet.refBuffer[w];
  } 
  tft.fillScreen(BLACK);//"Clear" the screen
     
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++) //here we make an numerical index of the output so we can keep track of them when the output references get alphabetized later in the process
  {      
    *refBufferPointer[u] = String(OutputIndex++); //adds index to the buffer             
    if(*sortedOutputsPointer[u] == 0) 
    {
      *refBufferPointer[u] = " ";   //adds whitespace to the buffer if it isn't an output         
    }
  }    
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)//here we make the reference to be sorted from global Reference
  { 
    tablet.sortReference[u].reserve(8);    
   *sortReferencePointer[u] = *globalReferencePointer[u]; 
    tablet.sortReference[u].trim();   
  }
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)//adding whitespace to the references to make it easier to add the index and later subtract the index from the alphabetized array 
  {   
    while(tablet.sortReference[u].length() < 9)
    {   
      tablet.sortReference[u]+=" ";  
    }
  }
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)
  {   
    tablet.sortReference[u] += *refBufferPointer[u]; //adding the reference string and the index together   
  }
  KickSort<String>::quickSort(tablet.sortReference,*pinNumberRoutingPointer); //alphabetize the array
  for(uint8_t t=0; t<*pinNumberRoutingPointer; t++)
  {  
  *refBufferPointer[t] = clearArray[0]; //clear the buffer since I'll be using it again
  }
  for(uint8_t u=0; u<*pinNumberRoutingPointer; u++)
  {  
  *refBufferPointer[u] = tablet.sortReference[u].substring(9,11); //here we subtract the index from the sorted array
  tablet.refBuffer[u].trim(); 
  *sortedOutputsPointer[u] = tablet.refBuffer[u].toInt();  //transfer from the buffer to the sorted outputs              
  *sortedOutputsPointer[u] = *sortedOutputsPointer[u]-1; //to exclude the elements of the output array that isn't an output we make them -1 since the first element in the pin array is 0                 
  }
   writeTableInputs();
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Write Table Inputs-----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void writeTableInputs()
{
  countingOutputs = 0;
  for(uint8_t t=0; t<*pinNumberRoutingPointer; t++)
  {  
    if(*sortedOutputsPointer[t] != -1) //prints out the alphabetized references
    {     
      countingOutputs++;      
    }
  }         
  byte index = 0;         
  for(uint8_t a=0; a<*pinNumberRoutingPointer; a++) //
  {                     
    globalReference[a].trim();
    inputs[a] = 0;
    if(*globalpinFunctionPointer[a] == "Input  ")
    {      
      
      inputs[a] = 1;//here we keep track of the inputs for the automated input table
      if(countingOutputs < 14)
      { 
        if(index<=8){tft.setCursor((index*25)+15, 240);}// bottom row
        if(index>8 && index<18){tft.setCursor(((index-9)*25)+15, 200);} //upper row
        if(index>=18){tft.setCursor(((index-18)*25)+15, 160);} //top row
        tft.setTextColor(WHITE);  tft.setTextSize(1);    
        tft.print(globalReference[a].substring(0,2));  //the references for the inputs    
            
        if(index<=8)//bottom row
        {
          buttonsPinout[a].initButton(&tft, (index*25)+20, 225, 22, 22, NAVY, BLUE, WHITE, "", 1); 
          if(buttonStatus[a] == 1)
          {
            buttonsPinout[a].drawButton(true);
          }
          if(buttonStatus[a] == 0)
          { 
            buttonsPinout[a].drawButton();
          }
        } 
        if(index>8 && index<18)//upper row
        {
          buttonsPinout[a].initButton(&tft, ((index-9)*25)+20, 185, 22, 22, NAVY, BLUE, WHITE, "", 1); 
          if(buttonStatus[a] == 1)
          {
            buttonsPinout[a].drawButton(true);
          }
          if(buttonStatus[a] == 0)
          { 
            buttonsPinout[a].drawButton();
          }
        } 
        if(index>=18)//top row
        {
          buttonsPinout[a].initButton(&tft, ((index-18)*25)+20, 145, 22, 22, NAVY, BLUE, WHITE, "", 1); 
          if(buttonStatus[a] == 1)
          {
            buttonsPinout[a].drawButton(true);
          }
          if(buttonStatus[a] == 0)
          { 
            buttonsPinout[a].drawButton();
          }
        }  
        index++; //this helps with updating only once per input                   
      }
      if(countingOutputs >= 14)//if the number of outputs is 14 or higher I needed to move them to the right of the screen and have fewer steps per screen instead...
      { 
        tft.setCursor(222,(index*25)+20);
        tft.setTextColor(WHITE);  tft.setTextSize(1);    
        tft.print(globalReference[a].substring(0,2));  //the references for the inputs    
            
        buttonsPinout[a].initButton(&tft, 210,(index*25)+25, 22, 22, NAVY, BLUE, WHITE, "", 1); //row on the right side
        if(buttonStatus[a] == 1)
          {
            buttonsPinout[a].drawButton(true);
          }
          if(buttonStatus[a] == 0)
          { 
            buttonsPinout[a].drawButton();
          }  
        index++; //this helps with updating only once per input                   
      }
    }                       
  }           
}
//----------------------------------------------------------------------------------------------------------------------//
//--------------------------Write Table Outputs to the CSV FIle(for chips with a clock pin)-----------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void writeOutputs()
{
  myFile = SD.open(fname3, FILE_WRITE);
  for(uint8_t t=0; t<*pinNumberRoutingPointer; t++)
  {  
    if(*sortedOutputsPointer[t] != -1) //prints out the alphabetized references
    {     
      if (myFile) // if the file opened okay, write to it:
      {
        myFile.print(","); myFile.print(tablet.sortReference[t].substring(0,8)); //prints the output references to the SD card
      } 
      else 
      {    
        Serial.println("error opening Table.csv"); // if the file didn't open, prints an error:
      }           
    }      
  }  
  myFile.println(",Clock"); //since this function is only called when there is a clock pin   
  myFile.close(); 
}
//----------------------------------------------------------------------------------------------------------------------//
//---------------------------Write Inputs and Outputs to the CSV file(for chips without clock)--------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void writeIOs()
{
  myFile = SD.open(fname3, FILE_WRITE);
  for(uint8_t t=0; t<*pinNumberRoutingPointer; t++)
  {     
    if (myFile) // if the file opened okay, write to it:
    {         
      while(globalReference[t].length() < 9)//since I had to trim the references in another function I add whitespaces for it to fit the format on the csv file
      {   
        globalReference[t]+=" ";  
      }  
      myFile.print(","); myFile.print(globalReference[t].substring(0,8));//this function prints all the references both inputs and outputs 
    } 
    else 
    {    
      Serial.println("error opening Table.csv"); // if the file didn't open, prints an error:
    }                   
  }  
  myFile.println(",Clock");    
  myFile.close(); 
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------Sorted Output Text for the truthtable----------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void outputText()
{
  tft.setTextSize(1);  
  tft.setTextColor(YELLOW);
  tft.setCursor(10,2);tft.print(F("Clock")); 
  tft.setTextColor(CYAN);     
  for(uint8_t t=0; t<*pinNumberRoutingPointer; t++)
  {  
    if(*sortedOutputsPointer[t] != -1) //prints out the alphabetized references
    {
      anotherSpacer++;
      outputCounter++;   
      tft.setTextSize(1); 
      tft.setCursor(10, (outputCounter*15)+2); 
      tft.print(tablet.sortReference[t].substring(0,8)); //we subtract the first 8 char from the array since references are only 8 char and that excludes the index that we made
    }
  }  
}
//----------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------Draws the table -----------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void table()
{
  if(anotherSpacer<14)
  {   
    for(uint8_t t=0; t<=anotherSpacer; t++)
    {   
      tft.drawFastHLine(5, 13+(t*15), 240, WHITE); 
    }
    for(uint8_t t=0; t<=8; t++)
    {     
      tft.drawFastVLine(62+(t*22), 0, (anotherSpacer+1)*15, WHITE);
    } 
  }
  if(anotherSpacer>=14)
  {  
    for(uint8_t t=0; t<=anotherSpacer; t++)
    {   
      tft.drawFastHLine(5, 13+(t*15), 190, WHITE); 
    }     
    for(uint8_t t=0; t<=6; t++)
    {     
      tft.drawFastVLine(62+(t*22), 0, (anotherSpacer+1)*15, WHITE);
    } 
  }
}
//----------------------------------------------------------------------------------------------------------------------//
//----------------------------------Prints on screen the Truthtable Values----------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void truthTableValues()
{
  
  tft.setTextSize(1);
  while(lastclockCounter != clockCounter )//
  {
    spaceCounter = 0; tableRowCounter = 0;
    
    for(uint8_t d=0; d<*pinNumberRoutingPointer; d++)
    {
      if(switches.clockmenuToggle == 0)//this truthtable is without a clock
      {          
        tft.setTextColor(YELLOW);//code below prints out the number of clock cycles the process has gone through  
        if(clockCounter <10){tft.setCursor(50+(rowsOfValuesCounter*22),2); tft.print(clockCounter);}//formatting clock text
        if(clockCounter >=10 && clockCounter < 100){tft.setCursor(46+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text       
        if(clockCounter >=100){tft.setCursor(42+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text   
        //below                    
        if(digitalRead(pin[d]) == 0)
        {            
          tableRow[tableRowCounter]=0; //this array is for the values that get saved on the SD card
          tableRowCounter++;//this is so only the values of outputs get saved to the tableRow array                                       
        }
        if(digitalRead(pin[d]) == 1)
        {        
          tableRow[tableRowCounter]=1; //this array is for the values that get saved on the SD card
          tableRowCounter++; //this is so only the values of outputs get saved to the tableRow array  
        }
        if(tablet.sortedOutputs[d] != -1) //excludes the elements that aren't outputs
        { 
          spaceCounter++;
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0)
          {
            tft.setCursor(50+(rowsOfValuesCounter*22), (spaceCounter*15)+2); tft.setTextColor(WHITE); tft.print(F("0"));                                     
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1)
          {
            tft.setCursor(50+(rowsOfValuesCounter*22), (spaceCounter*15)+2); tft.setTextColor(GREEN); tft.print(F("1"));           
          } 
        }                  
      }
      if(switches.clockmenuToggle == 1)
      {       
        if(*sortedOutputsPointer[d] != -1) //excludes the elements that aren't outputs
        {                           
          spaceCounter++;//formatting text helper 
          tft.setTextColor(YELLOW);
          if(clockCounter <10){tft.setCursor(50+(rowsOfValuesCounter*22),2); tft.print(clockCounter);}//formatting clock text
          if(clockCounter >=10 && clockCounter < 100){tft.setCursor(46+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text       
          if(clockCounter >=100){tft.setCursor(42+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text   
          //below                     
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0)
          {
            tft.setCursor(50+(rowsOfValuesCounter*22), (spaceCounter*15)+2); tft.setTextColor(WHITE); tft.print(F("0"));
            tableRow[tableRowCounter]=0; //this array is for the values that get saved on the SD card
            tableRowCounter++;//this is so only the values of outputs get saved to the tableRow array                                       
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1)
          {
            tft.setCursor(50+(rowsOfValuesCounter*22), (spaceCounter*15)+2); tft.setTextColor(GREEN); tft.print(F("1"));           
            tableRow[tableRowCounter]=1; //this array is for the values that get saved on the SD card
            tableRowCounter++; //this is so only the values of outputs get saved to the tableRow array  
          }
        }
      }              
    }
    lastclockCounter = clockCounter; 
//-----------------------------------------------CSV Truthtable Values--------------------------------------------------//    
    if(switches.clockmenuToggle == 0)
    {
      myFile = SD.open(fname3, FILE_WRITE);      
      for(uint8_t r=0; r<*pinNumberRoutingPointer; r++)
      {                     
        if (myFile) // if the file opened okay, write to it:
        {
          myFile.print(","); myFile.print(tableRow[r]); myFile.print("       ");
        } 
        else 
        {         
          Serial.println("error opening Table.csv"); // if the file didn't open, print an error:
        }       
      }
    }     
    if(switches.clockmenuToggle == 1)
    {
      myFile = SD.open(fname3, FILE_WRITE);      
      for(uint8_t r=0; r<anotherSpacer; r++)
      {                     
        if (myFile) // if the file opened okay, write to it:
        {
          myFile.print(","); myFile.print(tableRow[r]); myFile.print("       ");
        } 
        else 
        {         
          Serial.println("error opening Table.csv"); // if the file didn't open, print an error:
        }       
      }
    }
    myFile.print(","); myFile.println(clockCounter);      
  myFile.close();
  }                     
}
//----------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------Draws the diagram------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------//
void timingDiagram()
{
tft.setTextSize(1);  
  while(lastclockCounter != clockCounter )//
  {
    spaceCounter = 0;tableRowCounter = 0;
     
    for(uint8_t d=0; d<*pinNumberRoutingPointer; d++)
    {
      if(switches.clockmenuToggle == 0)//this truthtable is without a clock
      {          
        tft.setTextColor(YELLOW);
        if(clockCounter <10){tft.setCursor(50+(rowsOfValuesCounter*22),2); tft.print(clockCounter);}//formatting clock text
        if(clockCounter >=10 && clockCounter < 100){tft.setCursor(46+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text       
        if(clockCounter >=100){tft.setCursor(42+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text   
        //below             
        if(digitalRead(pin[d]) == 0)
        {            
          tableRow[tableRowCounter] = 0; //this array is for the values that get saved on the SD card
          tableRowCounter++;//this is so only the values of outputs get saved to the tableRow array                                       
        }
        if(digitalRead(pin[d]) == 1)
        {        
          tableRow[tableRowCounter] = 1; //this array is for the values that get saved on the SD card
          tableRowCounter++; //this is so only the values of outputs get saved to the tableRow array  
        }
        if(*sortedOutputsPointer[d] != -1) //excludes the elements that aren't outputs
        { 
          spaceCounter++;
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0 && lastDiagramState[d] == 0)//Low state
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 15+(spaceCounter*15), 22, WHITE);
            lastDiagramState[d] = 0;                                               
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0 && lastDiagramState[d] == 1)//from Low to High state
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 15+(spaceCounter*15), 22, WHITE);
            tft.drawFastVLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 10, GREEN);
            lastDiagramState[d] = 0;                                               
          }    
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1 && lastDiagramState[d] == 1)// High state
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 22, GREEN);    
            lastDiagramState[d] = 1;      
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1 && lastDiagramState[d] == 0)//High to Low state
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 22, GREEN);    
            tft.drawFastVLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 10, GREEN);   
            lastDiagramState[d] = 1;       
          }  
        }                  
      }
      if(switches.clockmenuToggle == 1)
      {    
        if(*sortedOutputsPointer[d] != -1) //excludes the elements that aren't outputs
        {                          
          spaceCounter++;//formatting text helper 
          tft.setTextColor(YELLOW);
          if(clockCounter <10){tft.setCursor(50+(rowsOfValuesCounter*22),2); tft.print(clockCounter);}//formatting clock text
          if(clockCounter >=10 && clockCounter < 100){tft.setCursor(46+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text       
          if(clockCounter >=100){tft.setCursor(42+(rowsOfValuesCounter*22),2); tft.print(clockCounter);} //formatting clock text   

          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0 && lastDiagramState[d] == 0)
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 15+(spaceCounter*15), 22, WHITE);
            lastDiagramState[d] = 0;          
            tableRow[tableRowCounter]=0; //this array is for the values that get saved on the SD card
            tableRowCounter++;//this is so only the values of outputs get saved to the tableRow array                                       
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 0 && lastDiagramState[d] == 1)
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 15+(spaceCounter*15), 22, WHITE);
            tft.drawFastVLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 10, GREEN);
            lastDiagramState[d] = 0;          
            tableRow[tableRowCounter]=0; //this array is for the values that get saved on the SD card
            tableRowCounter++;//this is so only the values of outputs get saved to the tableRow array                                       
          }    
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1 && lastDiagramState[d] == 1)
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 22, GREEN);    
            lastDiagramState[d] = 1;      
            tableRow[tableRowCounter]=1; //this array is for the values that get saved on the SD card
            tableRowCounter++; //this is so only the values of outputs get saved to the tableRow array  
          }
          if(digitalRead(pin[*sortedOutputsPointer[d]]) == 1 && lastDiagramState[d] == 0)
          {
            tft.drawFastHLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 22, GREEN);    
            tft.drawFastVLine(45+rowsOfValuesCounter*22, 5+(spaceCounter*15), 10, GREEN);   
            lastDiagramState[d] = 1;      
            tableRow[tableRowCounter]=1; //this array is for the values that get saved on the SD card
            tableRowCounter++; //this is so only the values of outputs get saved to the tableRow array  
          }  
        }              
      }
    }    
    lastclockCounter = clockCounter; 
//-----------------------------------------------CSV Truthtable Values--------------------------------------------------//      
    if(switches.clockmenuToggle == 0)
    {
      myFile = SD.open(fname3, FILE_WRITE);      
      for(uint8_t r=0; r<*pinNumberRoutingPointer; r++)
      {                     
        if (myFile) // if the file opened okay, write to it:
        {
          myFile.print(","); myFile.print(tableRow[r]); myFile.print("       ");
        } 
        else 
        {         
          Serial.println("error opening Table.csv"); // if the file didn't open, print an error:
        }       
      }
    }     
    if(switches.clockmenuToggle == 1)
    {
      myFile = SD.open(fname3, FILE_WRITE);      
      for(uint8_t r=0; r<anotherSpacer; r++)
      {                     
        if (myFile) // if the file opened okay, write to it:
        {
          myFile.print(","); myFile.print(tableRow[r]); myFile.print("       ");
        } 
        else 
        {         
          Serial.println("error opening Table.csv"); // if the file didn't open, print an error:
        }       
      }
    }
    myFile.print(","); myFile.println(clockCounter);      
    myFile.close();
  }                   
}