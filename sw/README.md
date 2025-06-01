# Software
STM32 software written in **STM32CubeIDE** (version 18.0.0)

Microcontroller: **STM32C031K6U6**

Overview:
- Because all the GPIO pins are on the **same port**, we can update all of them with one register write. This will allow us to easily multiplex the LEDs and create patterns via **DMA**.
- The DMA buffer is generated between two frames; with **double buffering**, one frame is being transmitted while the other is being prepared.
