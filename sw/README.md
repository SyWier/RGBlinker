# Software
STM32 software written in **STM32CubeIDE** (version 18.0.0)

Microcontroller: **STM32C031K6U6**

**RGBlinky**: Product software

Overview:
- Because all the GPIO pins are on the **same port**, we can update them all with a single register write. With **double buffering**, one frame is being transmitted while the other is being prepared.
- *Apperently the DMA in the C0 series cannot access the GPIO bus, so we are using Timer Interrupts to transmit the data.*

---
**RGBlinky_Test**: This is a simple test program designed to verify basic hardware functionality and allow experimentation during development.
- Sequentially lights up all LEDs one by one
- Cycles through colors (if RGB LEDs are present)
- Could be useful for initial board bring-up and diagnostics
