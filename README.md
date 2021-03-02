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
 *  ^ I also have forked the working branch to my github.
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

