#include <antplusdefs.h>
#include <USBHost_t36.h>

/*
 *  Author: William Stillwell ( KI4SWY // Hamster )
 *        * Also author of the romident.coinopflorida.com website.
 *  Description: USB to Fluke 9100, 9105, 9110FT etc, 
 *  Version: Public Release 2018.01.23 
 *  Donations Welcome :) 
 *
 *  Hardware Requirements
 *  Teensy 3.6 ( pjrc or digikey part # 1568-1442-ND )  -- MUST BE a Teensy 3.6
 *  Logic Level Converter - Bi Directional ( SparkFun ) or Digikey ( 1568-1209-ND ) ~ Or make your own.
 *  USB Header ( pjrc or Digikey Part # ED2989-ND ) ( or cut the end of the keyboard cable off ) 
 *  5pin Dinn Connector ( Digikey Part # SC1605-ND )
 *
 *  
 *  Library requirements:   
 *  https://github.com/PaulStoffregen/USBHost_t36
 *  ^ download the zip and install it via the Library - > Install option.
 *  ^ Tested Version has been saved in this Repository
 *  
 *  Wiring Pinout:
 *    Fluke DIN 5:
 *      Pin 1: Not Used ( /KBDATA )
 *      Pin 2: KBData ( To HV1 input of Logic Level Converter )
 *      Pin 3: KBrst  ( To HV2 input of Logic Level Converter )
 *      Pin 4: GND ( to GND on Logic Level Converter and GND on Teensy 3.6 )
 *      Pin 5: +5v ( to HV on Loic Level Converter and +5v on Teensy 3.6 )
 *      
 *   Teensy 3.6 ( PJRC or Digikey ) 
 *      +5v to Pin 5 on Fluke 5pin DIN & HV on Loic Level Converter
 *      +3.3V to LV on Logic Level Converter
 *      Input 0 to LV2 on Logic Level Convert ( Reset in , On Active Low, Fluke sent Reset )
 *      Output 8 ( Serial 3 TX ) to LV1 on Logic Level Converter ( KB Data Out ) 
 *      
 *      USB HOST port (  5+, D+, D-, and GND ) to USB Keyboard
 *      
 *      
 *    Keyboard Mapping:
 *    
 *      ALT+F1 = Field Select
 *      ALT+F2 = Edit
 *      ALT+F3 = Quit
 *      ALT+F4 = Msgs
 *      ALT+F5 = Help
 *      ALT+F6 = Info
 *      ALT+F7 = Begin File
 *      ALT+F8 = End File
 *      ALT+F9 = Scroll Forward [ May map Page Up for this in Future Version ]
 *      ALT+F10= Scroll Backward [ May map Page Down for this in Future Version ]
 *      ALT+F11 = Begin Line
 *      ALT+F12 = End Line
 *      
 *    Note, ALl CTRL+, SHIFT+ commands key combos are supported.
 *    
 *    ToDo: 
 *    
 *      Enable Number Pad / Num Lock, Currently Number pad does do anything
 *    
 */


struct flukemap
  {
      int okey;   // OEM Key ( from USB ) 
      int nkey;   // Normal Fluke Key Code
      int skey;   // Fluke Shifted Key Code ie, SHIFT+
      int ckey;   // Fluke Control Key Code ie, CTRL+
  };

flukemap key_map[82] = {
  {58,129,177,177},
  {59,130,178,178},
  {60,131,179,179},
  {0,132,180,180},    //  This is the Fluke Key between F3 and F4 , not mapped.
  {61,133,181,181},
  {62,134,182,182},
  {63,135,183,183},
  {64,136,184,184},
  {0,137,185,185},    // This is the Fluke Key between F7 and F8, not mapped.
  {65,138,186,186},
  {66,139,187,187},
  {67,140,188,188},
  {41,27,27,27},
  {30,49,33,225},
  {31,50,64,0},
  {32,51,35,226},
  {33,52,36,227},
  {34,53,37,228},
  {35,54,94,30},
  {36,55,38,229},
  {37,56,42,230},
  {38,57,40,231},
  {39,48,41,224},
  {45,45,95,31},
  {46,61,43,232},
  {53,96,126,233},
  {42,8,8,8},
  {70,243,243,243},
  {43,9,9,9},
  {20,113,81,17},
  {26,119,87,23},
  {8,101,69,5},
  {21,114,82,18},
  {23,116,84,20},
  {28,121,89,25},
  {24,117,85,21},
  {12,105,73,9},
  {18,111,79,15},
  {19,112,80,16},
  {47,91,123,27},
  {48,93,125,29},
  {76,127,127,239},
  {4,97,65,1},
  {22,115,83,19},
  {7,100,68,4},
  {9,102,70,6},
  {10,103,71,7},
  {11,104,72,8},
  {13,106,74,10},
  {14,107,75,11},
  {15,108,76,12},
  {51,59,58,237},
  {52,39,34,238},
  {40,13,13,13},
  {49,92,124,28},
  {29,122,90,26},
  {27,120,88,24},
  {6,99,67,3},
  {25,118,86,22},
  {5,98,66,2},
  {17,110,78,14},
  {16,109,77,13},
  {54,44,60,234},
  {55,46,62,235},
  {56,47,63,236},
  {82,161,218,209},
  {176,240,241,242},    // ALT+F1 Remap
  {44,32,32,0},
  {80,164,212,212},
  {81,162,210,210},
  {79,163,211,211},
  {177,141,189,189},    // ALT+F2 Remap 
  {178,143,191,191},    // ALT+F3
  {179,145,193,193},    // ...
  {180,146,194,194},    // ...
  {181,148,196,196},
  {182,149,197,197},
  {183,151,199,199},
  {184,153,201,201},
  {185,155,203,203},
  {186,157,205,205},
  {187,159,207,207}
};

USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
KeyboardController keyboard1(myusb);
KeyboardController keyboard2(myusb);

void setup()
{
  //while (!Serial) ; // wait for Arduino Serial Monitor
  //Serial.println("Fluke USB Adapter");

  // Teensy 3.6 has 0% error rate @ 1200 Baud @ 180mhz on Serial 3
  Serial3.begin(1200,SERIAL_8N2);

  // Reset Trap
  attachInterrupt(0, keyboard_reset, FALLING) ;

  // Start USB Hub
  myusb.begin();

  // Attached two Keyboards ( yes, if you have a hub, you can use two keyboards ) 
  keyboard1.attachPress(OnPress);
  keyboard2.attachPress(OnPress);
}

void loop()
{
  myusb.Task();
}

void keyboard_reset()
{
  Serial3.write(170); // Stock Keyboard Sends 0xAA
  //Serial.println("Reset Detected"); 
}

void OnPress(int key)
{
  int oKey = 0;
  int mKey = 0;
  int led = 0;

  int returnValue = -1;

  if ( keyboard1 )
  {
    oKey = keyboard1.getOemKey();
    mKey = keyboard1.getModifiers();
    led =  keyboard1.LEDS();
  }
  else
  {
    oKey = keyboard2.getOemKey();
    mKey = keyboard2.getModifiers();
    led =  keyboard2.LEDS();    
  }

  // do any key remaps.
  // ie ALT+F1 = Function 

  if (( mKey & 0x40 ) ^ ( mKey & 0x04 ) ) {
    //Serial.println("Alt Key Pressed - Remapping F1-F10 to Special Fluke Keys");
    if (( oKey >= 0x3a ) & ( oKey <= 0x45 )) {
      oKey = oKey + 118;
      //Serial.println("Remapped oKey");
    }
  }
  
  //scan keymap with oemkey
  for (int i=0;i<82;i++)
  {
    if (key_map[i].okey == oKey) {
      //Serial.print("KeyFound at index: ");
      //Serial.println(i);

      // get the Normal Value for the key
      returnValue = key_map[i].nkey;

      // if the Left or Right Shift Key is Pressed Rerturn the Shift Value or if Shift Lock is on.
      if (((mKey & 0x20) ^ (mKey & 0x02)) | (led & 0x02))
      { 
        //Serial.println("Shift Key Pressed.");
        returnValue = key_map[i].skey;
      }
      // if the Left or Right Control key is Pressed Return Ctrl Value
      if ((mKey & 0x10) ^ (mKey & 0x01))
      {
        //Serial.println("Ctrl Key Prssed.");
        returnValue = key_map[i].ckey;
      }
      
    }
  }

  //Serial.print("Key Press Key Code:");
  //Serial.print(key, HEX);
  //Serial.print(" Modifiers: ");
  //Serial.print(mKey, HEX);
  //Serial.print(" OemKey: ");
  //Serial.print(oKey, HEX);
  //Serial.print(" Led Status: ");
  //Serial.println(led, HEX);
  
  if ( returnValue >= 0 ) {
    //Serial.print("Fluke KeySend: ");
    //Serial.println(returnValue,HEX);
    Serial3.write(returnValue);
  }
  //else
  //{
  //  Serial.println("No mapping found, not sending any data");
  //}
  

}


