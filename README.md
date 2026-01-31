# Arduino Multi-App Console

This project is a multi-functional Arduino console that combines games, utilities, and sensor monitoring into a single device. It features a menu-driven interface displayed on a 16x2 LCD, controlled via an analog joystick and an IR remote.

## Features

The console includes the following applications:

1.  **Snake Game**: A classic implementation of Snake on a 16x2 grid.
2.  **Music Player**: Plays melodies (Nokia Tune, Star Wars, Mario) using a buzzer.
3.  **Calculator**: A basic calculator controlled via IR remote.
4.  **Weather Station**: Displays real-time temperature and humidity using a DHT11 sensor.
5.  **Dino Game**: A side-scrolling obstacle game inspired by the Chrome Dino game.
6.  **Simon Says**: A memory game using 4 LEDs and IR remote input.

## Hardware Requirements

*   **Microcontroller**: Arduino Uno (or compatible)
*   **Display**: 16x2 LCD with I2C Interface (Address `0x27`)
*   **Input**:
    *   Analog Joystick (2-axis + button)
    *   IR Receiver Module (e.g., VS1838B)
    *   IR Remote Control
*   **Output**:
    *   Active/Passive Buzzer
    *   4x LEDs (Red, Green, Blue, Yellow recommended)
*   **Sensors**:
    *   DHT11 Temperature & Humidity Sensor
*   **Resistors**: Current limiting resistors for LEDs (e.g., 220Ω or 330Ω).

## Pinout Configuration

| Component | Pin Name | Arduino Pin |
| :--- | :--- | :--- |
| **LCD I2C** | SDA | A4 (or SDA) |
| | SCL | A5 (or SCL) |
| **Joystick** | VRx | A0 |
| | VRy | A1 |
| | SW | D3 |
| **IR Receiver** | Signal | D7 |
| **Buzzer** | Signal | D8 |
| **DHT11** | Signal | D2 |
| **LEDs** | LED 1 | D13 |
| | LED 2 | D12 |
| | LED 3 | D11 |
| | LED 4 | D10 |

## Software Dependencies

This project is built using PlatformIO. Ensure the following libraries are installed in your `platformio.ini` or Library Manager:

*   `LiquidCrystal_I2C`
*   `IRremote`
*   `DHT sensor library`
*   `Adafruit Unified Sensor`

## Usage Instructions

### Main Menu
*   **Navigate**: Move the Joystick **Left/Right** to scroll through apps.
*   **Select**: Press the Joystick Button (**SW**) to open an app.

### Applications

#### 1. Snake
*   **Objective**: Eat the food ('O') to grow without hitting the walls or yourself.
*   **Controls**: Use the Joystick (Up, Down, Left, Right) to steer.
*   **Exit**: Press the Joystick Button.

#### 2. Music Player
*   **Navigate**: Joystick Left/Right to select a song.
*   **Play**: Press Joystick Button.
*   **Exit**: Move Joystick Down.

#### 3. Calculator
*   **Input**: Uses the IR Remote.
*   **Mapping**:
    *   Numbers `0-9`: Standard number keys.
    *   Operations: `+`, `-`, `*`, `/` mapped to specific remote codes (check `main.cpp` for hex codes).
    *   Calculate: `=` button.
    *   Clear: `EQ` or similar button (Code `0xFFA25D`).
*   **Exit**: `MODE` or similar button (Code `0xFF629D`).

#### 4. Weather
*   **Display**: Shows Temperature (°C) and Humidity (%).
*   **Exit**: Press the Exit button on IR Remote (Code `0xFF629D`).

#### 5. Dino Game
*   **Objective**: Jump over incoming obstacles.
*   **Controls**:
    *   **Jump**: Joystick Up OR IR Remote Button '1'.
*   **Exit**: Press the Exit button on IR Remote.

#### 6. Simon Says
*   **Objective**: Repeat the sequence of flashing LEDs.
*   **Controls**:
    *   Watch the LED sequence.
    *   Repeat it by pressing buttons `1`, `2`, `3`, or `4` on the IR Remote corresponding to LEDs 1-4.
*   **Exit**: Press the Exit button on IR Remote.

## IR Remote Codes

The code uses raw IR data values. You may need to update the hex codes in `main.cpp` if your remote sends different signals.
*   **Key '1'**: `0xEE11FB04`
*   **Key '+'**: `0xFD02FB04`
*   **Exit Key**: `0xFF629D`
*   *(See source code for full list)*
