# Software
STM32 software written in **STM32CubeIDE** (version 18.0.0)

Microcontroller: **STM32C031K6U6**

**RGBlinky: Product software**

Overview:
- Because all the GPIO pins are on the **same port**, we can update them all with a single register write. This enables easy multiplexing of the LEDs and creation of patterns via **DMA**.
- The DMA buffer is generated between two frames; with **double buffering**, one frame is being transmitted while the other is being prepared.

**RGBlinky_Test: Simple test software to test hardware**

Lights up all LEDs one-by-one, then color-by-color.
