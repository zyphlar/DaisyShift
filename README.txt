//**************************************************************//
//  Name    : DaisyShift - Daisy Chain Shift Register display
//              using Arduino Ethernet
//  Author  : Will Bradley
//  Date    : 18 Feb, 2012
//  Version : 1.0
//  Notes   : Outputs HTTP data to 74HC595 shift registers
//            Based on ShiftOut and WebClient examples from Arduino
//  URL     : http://zyphon.com/daisyshift
//****************************************************************

DaisyShift - Daisy Chain Shift Register display using Arduino Ethernet
by Will Bradley, www.zyphon.com/daisyshift - twitter @willbradley

For creating an Ethernet-enabled, daisy chainable LED matrix that displays information from a webservice on the LEDs. Each "row" has 8 LEDs, and you can stack as many rows as you like.
Intentionally kept hackable, with lots of commented-out sections and debugging information available, because everyone's environment is slightly different.

Included in distribution:
- nagiosdisplay.pde, based on Arduino WebClient and shiftOut example code
- nag.php, based on GPL-licensed code by Jason Antman
- ShiftOutExample images, copyright Arduino
- schematics, board layouts, and photos, by me

Overview: This example assumes you'll be using Nagios, but it can be modified to parse almost any data you can access via PHP and/or HTTP. Nagios keeps its status info in a status.dat file, which nag.php parses and outputs in a simplified way. nagiosdisplay.pde gets loaded onto an Arduino Ethernet, connects to nag.php via HTTP, parses that simple data, and lights up LEDs corresponding to each server. You'll need to edit some settings in both files to make them work, but if you're halfway familiar with your Nagios installation, Arduino programming (especially the ShiftOut and SimpleClient examples), and networking (what's a subnet mask?) you should be fine. I'd love to see other people use this code and ask questions, too. Send me photos!

To assemble the hardware, I recommend an Arduino Ethernet, which is essentially an Arduino + Ethernet Shield combined, and my DaisyShift kit available at the website above. You can make your own shift register displays by following the ShiftOutExample images taken from the Shift Out Arduino tutorial at http://arduino.cc/en/Tutorial/ShiftOut. The only difference between their example and this code is that we are using pins 5,6, and 7 instead of 8, 11, and 12.

SOLDERING TIP: If you use the DaisyShift kit, I suggest soldering all the strip's right-angle headers upside down (on the bottom of the board) so that they line up with the right-angle header on the shield. If you solder them all "right side up" you'll have a hard time plugging the strip into the shield due to the height differences.

To get the microcontroller software working, just look at the nagiosdisplay.pde comments to see what to modify.

To get the server working, put the nag.php script on your Nagios server outside of any authentication-required folders (or make an exception for this file somehow. The Arduino doesn't currently authenticate.) On my server, I put the file at /var/www/nag.php . See the comments in the file and modify settings as needed.

Please send me feedback, I'd love to help!
