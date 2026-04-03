# STM32 Projects

A collection of bare-metal embedded firmware projects for the **STM32F4** microcontroller, written in C using direct register manipulation (no HAL abstraction). Built as part of a hands-on embedded systems learning path.

---

## Board

**STM32 Nucleo-F401RE** (STM32F401RE)
- Cortex-M4 core @ 84 MHz
- 512 KB Flash, 96 KB RAM
- Integrated ST-Link/V2-1 debugger/programmer
- Toolchain: STM32CubeIDE + ST-Link

---

## Projects

| # | Project | Description |
|---|---------|-------------|
| 0 | `0_test` | Minimal bringup — verifies toolchain, flash, and debugger connection |
| 1 | `1_Debugging_Basic` | Debugger basics: breakpoints, watch variables, step execution |
| 2 | `2_Working_With_ITM` | ITM output over SWO — sending characters via `ITM_SendChar()` |
| 3 | `3_Working_With_ITM_printf` | Redirecting `printf` to ITM via `__io_putchar()` override |
| 4 | `4_Working_With_ITM_printf_logs` | Structured logging over ITM with formatted output |
| 5 | `5_Working_With_UART` | UART2 TX bare-metal driver on PA2 — `printf` over serial at 115200 baud |
| 7 | `7_DMA_MemToMem` | DMA memory-to-memory transfer — no CPU involvement |
| 8 | `8_ADC_DMA` | ADC continuous conversion with DMA — reading analog values without polling |
| 9 | `9_ADC_TIM_DMA` | ADC triggered by a timer, transferred via DMA — precise periodic sampling |
| 10 | `10_UART_DMA` | UART TX driven by DMA — offloads serial transmission from the CPU |
| 11 | `11_SPI_DMA` | SPI communication using DMA for high-speed data transfers |

---

## Key Concepts Covered

- **Bare-metal register programming** — direct manipulation of RCC, GPIO, USART, SPI, ADC, DMA registers
- **Clock configuration** — enabling peripheral clocks via AHB1/APB1/APB2 buses
- **GPIO alternate functions** — configuring pins for UART, SPI, ADC via AFR registers
- **ITM / SWO debugging** — lightweight `printf`-style output without UART
- **UART driver** — custom TX driver with baudrate calculation and blocking transmit
- **DMA** — memory-to-memory, peripheral-to-memory, triggered and untriggered modes
- **ADC** — single and continuous conversion modes, DMA-linked sampling
- **SPI** — master mode communication over DMA

---

## Project Structure

Each project folder follows the standard STM32CubeIDE layout:

```
<project>/
├── Core/
│   ├── Inc/        # Header files
│   └── Src/        # Source files (main.c, peripherals)
├── Drivers/        # CMSIS headers
└── ...             # IDE project files
```

---

## Getting Started

1. Clone the repo:
   ```bash
   git clone https://github.com/retroBoy97/Stm32-Projects.git
   ```
2. Open **STM32CubeIDE** → `File > Open Projects from File System`
3. Select the project folder you want
4. Build and flash via ST-Link

---

## Author

**retroBoy97** — Software Engineering student at INSAT, specializing in embedded systems.
