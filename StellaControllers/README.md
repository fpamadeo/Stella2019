# Construction Guide:
_______________________________________________________________________________________________________________________________
## 0. Table of Contents:
1 | [Microcontroller/s used](README.md#1-microcontrollers-used)
2 | [Controller Pinouts](README.md#2-Controller-Pinouts)
• 2.1 | [LCD Screen](README.md#21-LCD-Screen)
• 2.2 | [Joystick/s](README.md#22-Joysticks)
• 2.3 | [Transceiver](README.md#23-Transceiver)
• 2.4 | [Push Buttons (Std)/Tactile Buttons (Trigger)/Trigger Switch](README.md#24-push-buttons-stdtactile-buttons-triggertrigger-switch)
3 | [Remote Pinouts](README.md#2-Remote-Pinouts)
• 3.1 | [Transceiver](README.md#31-Transceiver)
4 | [Menu](README.md#4-Menu)
_______________________________________________________________________________________________________________________________
## 1. Microcontroller/s used:
* Arduino MEGA for the controller itself
* Arduino UNO for the remote control on the robot
_______________________________________________________________________________________________________________________________
## 2. Controller Pinouts:
### 2.1 LCD Screen:
Arduino | LCD Screen 
--- | --- 
D22 | DB7
D24 | DB6
D26 | DB5
D28 | DB4
D27 | DB3
D29 | DB2
D31 | DB1
D30 | DB0
D23 | E 
D25 | RS

### 2.2 Joystick/s:
Arduino | Joysticks 
--- | --- 
A0 | JS1 (X-Axis)
A1 | JS1 (Y-Axis)
A2 | JS2 (X-Axis)
A3 | JS2 (Y-Axis)
 
### 2.3 Transceiver:
Arduino | Transceiver 
--- | --- 
VCC | 3.3 V (**DON'T USE 5V**)
CE | D7
CS | D8
SCK | D13
MOSI | D11
MISO | D12

### 2.4 Push Buttons (Std)/Tactile Buttons (Trigger)/Trigger Switch:
Arduino | Switch/Button 
--- | --- 
D38 | PB1 (UP)
D39 | PB2 (DOWN)
D41 | PB3 (SELECT)
D40 | TB1 (Previous/UPx3)
D42 | TB2 (Next/DOWNx3)
D25 | TS1 (Shooter)
_______________________________________________________________________________________________________________________________
## 3. Remote Pinouts:
### 3.1 Transceiver:
Arduino | Transceiver 
--- | --- 
VCC | 3.3 V (**DON'T USE 5V**)
CE | D7
CS | D8
SCK | D13
MOSI | D11
MISO | D12
_______________________________________________________________________________________________________________________________
## 4. Menu:
1. Shooter (Lock/Unlock)
2. Grabber (Lock/Unlock)
3. Foam Balls (Lock/Unlock)
4. Turn On Shooter
5. Grab now
6. Release Pit Balls
7. Release 1 Foam Ball
8. Release Foam Balls
9. Lock All
10. Unlock All
_______________________________________________________________________________________________________________________________
## 5. Notes:
* **LCD Screen/LiquidCrystal.h** seems to have a bug and print a weird character when *lcd.clear()* and *lcd.setCursor(int,int)* is called in *loop()* instead of doing what it should be. To work around this, we brute forced this and just printed a 20x4 screen each call to *printScreen()*.
* **20x4 LCD Screen** prints in the order of: Line 1 -> Line 3 -> Line 2 -> Line 4 due to how the Hardware is made. 
_______________________________________________________________________________________________________________________________

If you have any questions, please contact Francis
