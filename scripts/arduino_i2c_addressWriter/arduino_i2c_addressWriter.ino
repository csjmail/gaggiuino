
 /*
 * Arduino sketch to update I2C address for GY-530 VL53L0X module
 * Watch video instruction for this code: https://youtu.be/RRQASevYK3g
 * Download this code from: http://robojax.com
 * 
* Written by Ahmad Shamshiri for Robojax.com on Dec 06, 2018 
 * in Ajax, Ontario, Canada
 * Video Tutorial recorded Dec 06, 2018
 * 
  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//UNO pins:
//SDA A4
//SCL A5

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();


void setup() {
  lox.begin(0x2A);//address between 0x29 to 0x7F 
}

void loop(){
  
}
