

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//----------------------Original code made by Baweja Akshay-----------------------//
//                                                                                //
//----------------https://www.instructables.com/Smart-IC-Tester/------------------//
//---------------https://github.com/akshaybaweja/Smart-IC-Tester------------------//
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//-----------------------------Modified by A.S.G----------------------------------//
//                                                                                //
//--------------------Changes to the original code include:-----------------------//
//----------------------------------Additions-------------------------------------//
//1.Repeat testing,monitoring failed and passed tests + a bit more detailed report//
//2.Fast mode || detailed mode so the user can see what chips are being tested.etc//
//3.Manual graphical test mode so the user can test individual pins in real time--//
//4.Visual Truthtables that cam be saved as csv files-----------------------------//
//5.Saves information about last test making it easier to test same types of ICs--//
//6.20 and 24 pin ICs have been added---------------------------------------------//
//---------------------------------Improvements-----------------------------------//
//1.Device does not need to be restarted after each test or when changing modes---//
//--------------------------------Known bugs--------------------------------------//
//1.has to run 2-3 test for it to read the arduino pins correctly in IC test mode-//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <pin_magic.h>
#include <registers.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>
#include <SPFD5408_Util.h>
#include <pins_arduino.h>
#include "KickSort.h" //to sort the references alphabetically 


//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <TouchScreen.h> //Touch Screen Library
//SD CARD
#include <SPI.h>
#include <SD.h> //need to override the default SD library with the adafruit SD library C:\Users\username\AppData\Local\Arduino15\libraries on win 10
//the SD library can be found here https://github.com/akshaybaweja/Smart-IC-Tester/tree/master/Libraries

#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0
#define LCD_RESET A4  // LCD RESET goes to Analog 4

#define YP A3  // must be an analog pin, use "An" notation!, Screen 1 = A3 // screen 2 = A1
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin, Screen 1 = 9 // Screen 2 = 7
#define XP 8   // can be a digital pin, Screen 1 = 8 //Screen 2 = 6

#define TS_MINX 150  //150
#define TS_MINY 120  //120
#define TS_MAXX 920  //920
#define TS_MAXY 940  //940

#define MINPRESSURE 10//was originally 10
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x291F
#define RED 0xF800
#define GREEN 0x24A1
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x9CF4
#define DARKGREY 0x4209
#define MAROON 0x7800
#define NAVY 0x000F
#define OLIVE 0x7BE0
#define ORANGE 0xFDA0
#define PURPLE 0x780F
#define NAVY 0x000F

//TFT initialization
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//Buttons
Adafruit_GFX_Button buttonsPinout[24]; 
Adafruit_GFX_Button buttonsClearPinout[1];
Adafruit_GFX_Button buttonsMenus[12];

//Pin Definitions             1   2   3   4   5   6   7   8   9   10  11  12  13  14  
 constexpr int PIN14[14]  = { 45, 43, 41, 39, 37, 35, 33, 32, 34, 36, 38, 40, 42, 44 };//digital pins on the Arduino Mega 
//Pin Definitions             1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
 constexpr int PIN16[16]  = { 45, 43, 41, 39, 37, 35, 33, 31, 30, 32, 34, 36, 38, 40, 42, 44 };//digital pins on the Arduino Mega 
//Pin Definitions             1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20
 constexpr int PIN20[20]  = { 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44 };//digital pins on the Arduino Mega 
//Pin Definitions             1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24
 constexpr int PIN24[24]  = { 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44 };//digital pins on the Arduino Mega 

File myFile;
#define chipSelect 10
//Database File names !!!!!!txt files cane only have 8 characters in their names!!!!!!!!!
#define fname  "database.txt"//here the tests for each IC is stored
#define fname2 "Pinouts.txt"//here is the information about each ICs pinout stored
#define fname3 "Table.csv"//here the truthtables/timing diagrams are stored
#define fname4 "State.txt"//Order of data, IC number, ScreenStatus, Number of tests, Cycles, Diagram ON/OFF 
#define fname5 "Config.txt"//order of data, last IC, enable inputs with clock, Clear CSV, Auto set to diagram, speed, Screensaver
#define fname6 "Position.txt"//position of an IC
//Function Headers
void autoSearchResult(uint8_t mode = 0);
boolean testIC(String buffer, byte pins);   

//Structure definition for alphabetized outputs, used in truthtables and diagrams
typedef struct 
{
  String sortReference[24];
  int8_t sortedOutputs[24];//this is the only variable that can be a negative number
  String refBuffer[24];  
} truthTablet;
truthTablet tablet;

//Structure Definiton for Chip under test 
typedef struct 
{
  String num;               
  String name;
  int pins;
  String pinFunction;        
  String pinReference;         
} chip;

//Structure for 1 bit switches
struct boolSwitches 
{
  bool clockmenuToggle          : 1;// if the IC has a clock pin    
  bool clockToggle              : 1;// if the IC has a clock pin  
  bool clockbuttonStatus        : 1;// if the IC has a clock pin  
  bool lastclockbuttonStatus    : 1;// if the IC has a clock pin  
  bool muxdemuxMenuToggle       : 1;//if the IC is a multiplexer/demultiplexer
  bool muxdemuxToggle           : 1;//if the IC is a multiplexer/demultiplexer
  bool muxdemuxToggleStatus     : 1;//if the IC is a multiplexer/demultiplexer
  bool lastmuxdemuxToggleStatus : 1;//if the IC is a multiplexer/demultiplexer
  bool muxdemuxFlag = 0;        : 1;//if the IC is a multiplexer/demultiplexer
  bool fastMode = 0;            : 1;//this is used to bypass time consuming graphical display updates  
  bool fastStatus;              : 1;//this is used to bypass time consuming graphical display updates  
  bool lastfastStatus;          : 1;//this is used to bypass time consuming graphical display updates  
  bool config1 = 0;             : 1;//config 1 toggle
  bool config1Status            : 1;//config 1 toggle 
  bool lastconfig1Status        : 1;//config 1 toggle
  bool config2 = 0;             : 1;//config 2 toggle
  bool config2Status            : 1;//config 2 toggle 
  bool lastconfig2Status        : 1;//config 2 toggle
  bool config3 = 0;             : 1;//config 3 toggle
  bool config3Status            : 1;//config 3 toggle 
  bool lastconfig3Status        : 1;//config 3 toggle  
  bool config4 = 0;             : 1;//config 4 toggle
  bool config4Status            : 1;//config 4 toggle 
  bool lastconfig4Status        : 1;//config 4 toggle
  bool config5 = 0;             : 1;//config 5 toggle
  bool config5Status            : 1;//config 5 toggle 
  bool lastconfig5Status        : 1;//config 5 toggle
  bool keypadIC = 0;            : 1;//Keypad last IC toggle       
  bool autosearchEnd = 0;       : 1;
  bool switchCaseFininshed = 0; : 1;
  bool manualSwitch             : 1;
  bool circumvent = 0;          : 1;//used to bypass the rest of the routine(usually the keypad function) and go to main menu 
  bool diagram = 0;             : 1;//timing diagram / truth table switch       
  bool lastDiagram = 0;         : 1;//timing diagram / truth table switch  
  bool statusDiagram = 0;       : 1;//timing diagram / truth table switch  
  bool fullCycle = 0;           : 1;//trigger type switch
  bool statusfullCycle = 0;     : 1;//trigger type switch
  bool lastfullCycle = 0;       : 1;//trigger type switch
  bool saveStateSD = 0;         : 1;//this differentiate between lastTest or regular test
  bool status = 0;              : 1;
  bool endlessLoopSwitch = 0;   : 1;//test an IC until the user stops the test
  bool pushingButtons = 0;      : 1;//automatic input button push switch   
  bool clearCSV = 0;            : 1;//clear CSV file after every run switch
  bool holdButtons = 0;         : 1;//hold inputs high switch  
  bool circumventTFT = 0;       : 1;//flips the output low after certain amount of inputs have been held high 
  bool macgyver = 0;            : 1;//switch for the screensaver
  bool pushed = 0;              : 1;//touchscreen input
}; 
boolSwitches switches;

//Variables
bool storeErrorPlace[24];     
bool tableRow[24];             
bool inputs[24];                          
bool lastDiagramState[24];    
bool clock[24];              
bool inOut[24];//stores the pin information for mux/demux, set as Inputs               
bool outIn[24];//stores the pin information for mux/demux, set as Outputs   
bool inputPushing[24];//     
bool lastInputPushing[24];    
   
byte pinSpacer;//space between pins in pinout mode
byte screenStatus = 0; //switches between states/routines for the program
byte lastStatus = 0; //switches between states/routines for the program
byte automaticInputButtonPusher = 0; 
byte speed = 1;//speed of clock cycle in pinout mode and the speed of clock trigger in truthtable/diagram mode
byte countingOutputs = 0;//used ro count numbers of outputs for diagram/truthtable mode
byte numberofIcs; // keeps track of number of IC that matched the test of IC under IC search
byte pinNumberRouting;//this keeps track of the number of pins on a IC that is being tested between modes
byte lapsCounter = 0; //keeps count of how many times the main fumction(flow) has been processed for the clock function
byte outputCounter = 0, lastOutputCounter = 0;
byte rowsOfValuesCounter = 0;
byte anotherSpacer = 0;
byte spaceCounter = 0;
byte OutputIndex = 1;  
byte screenStatusSD = 0; //keeps track of the last screenstatus the program was in when it got saved to the SD card
byte linecount = 1;//keeps count on the number of test lines for a IC under test in IC test mode
byte buttonSize = 22;//size of buttons in pinout mode
byte outputSize = 10; //size of outputs in pinout mode
byte boxSize = 20;  //size of unchangable pins in pinout mode eg. VCC,GND and NC pins
byte horizontalNudge = 0;
byte previousScreenstatus = 0;
char converter[10]; //used to convert IC numbers into char for button displaying
byte outputs[24]; //state of the output pins            
byte clearArray[1]; //used to clear arrays
bool buttonStatus[24]; //these used to be integers
bool buttonNonToggleStatus[24]; 
bool lastbuttonStatus[24];  //should trim these a bit
bool lastoutputState[24];//to reduce reduntant drawings of graphics on screen could be bool
word macgyverTimingMechanism = 0;//screensaver
word macgyverCounter = 0;// screensaver
word fade = 100;//demo graphics
word shifterLeft;
word shifterRight;
word clockCounter = 0;//should be able to get away with using a byte and should maybe start at 0
word tableRowCounter = 0;
word cycle = 0; //
word lastclockCounter = 0;
String chipDescription;
String numberAuto[10];//stores the ID of the ICs that matched the auto search
String numberRouting; //stores the name/number of the chip in question e.g 4022 or 74682
String globalReference[24], globalpinFunction[24], storeTestLines[24], storeErrorLines[24];
String numberofTests;//stores the number of tests!!!!!!!!!!!why is this a string
byte *pinNumberRoutingPointer = &pinNumberRouting; 
int8_t *sortedOutputsPointer[24];
int  *pin, pinCount = 0;
String *globalReferencePointer[24];
String *globalpinFunctionPointer[24];
String *sortReferencePointer[24];
String *chipDescriptionPointer;
char previousTestPinFunction[24];
unsigned int cycles2;

void SD_init()
{
  pinMode(SS, OUTPUT);
  if (!SD.begin(10, 11, 12, 13)) {Serial.println(F("Card failed, or not present")); return;}
  Serial.println("SD card initialized.");
}

void setup() 
{  
  for(uint8_t w=0; w<24; w++)
  {  
    sortedOutputsPointer[w] = &tablet.sortedOutputs[w];  
    globalReferencePointer[w] = &globalReference[w];
    globalpinFunctionPointer[w] = &globalpinFunction[w];
    sortReferencePointer[w] = &tablet.sortReference[w];
  } 
  Serial.begin(115200);
  
  delay(1000); // power-up safety delay 
  tft_init();  //TFT setup 
  SD_init(); //SD Card Setup
}

void loop() 
{    
  while(lapsCounter < speed/2) {flow();}//Serial.println("inside of loop");}
  if(switches.clockmenuToggle == 0 && cycle > 0){automateRoutine(1);} 
  if(switches.clockToggle == 1){clockRoutine(1);} //ClockPulse High, Clock pulse Routine has to be outside the main loop otherwise I have to slow down the loop for it to be sensible  
  while(lapsCounter < speed) {flow();}//Serial.println("inside of loop");}
  lapsCounter = 0; //reset the counter that keeps track of how many times the main loop has passed
  if(switches.clockmenuToggle == 0 && cycle > 0){automateRoutine(0); automaticInputButtonPusher++;}   
  if(switches.clockToggle == 1){clockRoutine(0); automaticInputButtonPusher++;} //ClockPulse LOW 
  //Serial.println("End of loop");
}