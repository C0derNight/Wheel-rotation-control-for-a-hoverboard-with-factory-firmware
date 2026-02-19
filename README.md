# Wheel-rotation-control-for-a-hoverboard-with-factory-firmware
This project describes a method for controlling each wheel by connecting an additional controller to the hoverboard motherboard.


I have such model hoverboard (wheels 10.5 inch)
![HOVERBOARD CON RUOTE DA 10 POLLICI - SMART BALANCE -800x800](https://github.com/user-attachments/assets/27af85b9-e8d7-4302-9894-9ff2719cf279)


Oscilloscope measurements showed that the system uses 3.3-volt logic levels. It turned out to be a UART interface. The minimum UART bit duration was 37 µs, which corresponds to a data rate of 27,777 bits per second.
![photo_5238090300176144052_y](https://github.com/user-attachments/assets/b81c47f0-5289-4dd1-b975-9087e74f0dde)
![photo_5238090300176144050_y](https://github.com/user-attachments/assets/eea29c4f-82ab-40da-95f8-bff9f15ed4f3)




The data could be read using a CH340 adapter. After connecting TX gyro sensor to RX CH340 and opening the serial monitor, I received a stream of bytes.
<img width="350" height="636" alt="Без имени" src="https://github.com/user-attachments/assets/0b8129f6-642c-4762-b61f-7a33f827cb60" />
![photo_5240342099989829151_y](https://github.com/user-attachments/assets/6fe78fdf-9373-4853-93e3-7d034a42c473)

The goal was to emulate the gyroscope board in order to make the wheel spin. Therefore, using the CH340, I captured the bytes coming from the gyroscope board, but identifying the data structure was difficult.

After exporting the captured gyroscope data into Excel using a Python script, cyclic patterns became visible. This made it possible to identify the packet transmitted by the gyroscope.

Analysis showed that the packet consisted of eight bytes:

{0, fractional part of the tilt angle, integer part of the tilt angle, fractional part of the tilt angle, integer part of the tilt angle, button state, acceleration, acceleration}
![photo_5240342099989828591_y](https://github.com/user-attachments/assets/39d71b28-7934-467d-8972-6f3b2c6b570f)

The mainboard was sending some data to the hoverboard control board (it was unclear why). UART analysis showed that it was a six-byte packet: when the button was not pressed, all bytes were zeros, and when one of the buttons was pressed, the packet contained the number 4 followed by five zeros. However, this data was not useful.

When attempting to send a packet to motherboard with the same values at the same UART speed, the hoverboard did not even signal that the button had been pressed to start the stabilization process. The wheel did not spin. It turned out that this was a UART interface that could be read but could not simply be spoofed.

There was almost no chance of gaining control over the wheel. I tried many different approaches, but the last one eventually turned out to be successful.

Using a Python script, I measured the transition durations of the TX pin gyroscope board from high to low and from low to high and exported them into Excel. In just a few seconds of capturing data from this mysterious and unique UART gyroscope, I collected several thousand timing values (the recording was done with the button pressed and the wheel spinning).

While searching through the timing data, I found a time interval corresponding to the UART pre-start pause (341). The numbers between such “pauses” turned out to be data packets. Knowing the packet structure and the approximate value of each byte, I reconstructed every byte based on timing information. 
<img width="442" height="703" alt="image" src="https://github.com/user-attachments/assets/a4dfcccd-5669-4fc6-9329-ae33fcb72ad4" />

I then created an array of timings and manually switched the pin state sequentially whenever each timing interval expired. In this way, I reproduced the recorded packet in a loop, and after that the wheel finally started spinning.

Important: the code must be uploaded to the board first, and only then should the hoverboard be powered on using the button — otherwise it will not work. If the hoverboard does not receive UART data for a long time, it completely stops functioning and requires a reboot.

I modified the code to allow speed control by sending commands through the serial monitor.

The hoverboard behavior when controlled by the original gyroscope boards is as follows: if the hoverboard is lifted off the ground and both pedals (left and right) are pressed, the wheels accelerate to maximum speed and keep spinning until the pedals are released. The behavior is identical when using the emulated gyroscope boards. This suggests that when only one wheel is active, a P controller regulates the speed, while activating both wheels enables a PI controller.

The problem is that when controlling only one wheel (left or right), the rotation speed changes proportionally to the tilt angle. However, when the second wheel is enabled (simulating a pressed pedal button), both wheels accelerate to maximum speed and stop only after the button is released.

I decided to tap into the Hall sensor signals and use them as an encoder to measure rotational speed. However, connecting one Hall sensor directly to a Teensy 4.0 digital input did not work because the signal level was 5 V.

It may be necessary to take acceleration into account when controlling both wheels.

Another possible approach would be to emulate a PI controller in order to achieve full simultaneous control over both wheels.
