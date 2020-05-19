/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


//PwmOut led(D3);
DigitalIn pb(USER_BUTTON);
AnalogIn ain(A0);
DigitalOut aout(D3);

int main()
{
	while(1) {
		//if(!pb) {

		aout = ain;
		// specify period first
		//led.period(0.001f);      // 4 second period
		//led.write(0.50f);      // 50% duty cycle, relative to period
		//led = 0.5f;          // shorthand for led.write()
		//led.pulsewidth(2);   // alternative to led.write, set duty cycle time in seconds
		//}
	}
}
