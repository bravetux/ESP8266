# ESP8266
ESP8266 with dual relay board

### 🔧 Project Title:

**IoT-Based Dual Relay Controller with Web Timer Interface using ESP8266**

---

### 📋 Project Explanation:

This project revolves around an **ESP8266 Wi-Fi microcontroller**, programmed to control **two relays** that can independently switch AC-powered devices like a **water pump** and a **solenoid valve**.

Instead of using physical switches, the system creates a **Wi-Fi Access Point** (`vishnu-ap`), which anyone nearby can connect to using a smartphone or computer. Once connected, users can access a built-in **web page hosted by the ESP8266**, which acts as a control panel.

On the webpage:

* Each relay is represented by a **toggle switch**
* A **dropdown menu** lets users select a timer (either **10 seconds or 15 seconds**)
* After switching ON, a **live countdown timer** appears, automatically turning OFF the relay when it hits zero

This ensures that the connected device doesn’t stay powered longer than necessary — a simple and effective **automation and safety feature**. The code uses **asynchronous server handling**, meaning the ESP8266 can manage multiple requests without getting blocked or frozen.

The hardware includes:

* 2-channel relay module (connected to GPIO 5 and 4)
* Optional: submersible pump and solenoid valve wired to the relays (powered via 220V AC)

It’s an excellent project for:

* Home automation
* School science exhibitions
* Timed liquid dispensing (e.g., water dispensers)
* Smart control of simple AC appliances

In short: **It’s an intuitive, user-friendly IoT system that brings smart control to basic electronics — with no app or cloud needed.**

Here’s a breakdown of the **hardware components** used in this IoT-based Dual Relay Timer Control project:

---

### 🧰 **Hardware Components List**

| Component                                               | Description                                                                                        | Quantity  |
| ------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | --------- |
| 🔌 **ESP8266 NodeMCU**                                  | The main microcontroller with built-in Wi-Fi. Handles web server, relay control, and timing logic. | 1         |
| ⚡ **2-Channel Relay Module**                            | Switches AC devices (like pump or solenoid valve) based on GPIO signals from ESP8266.             | 1         |
| 🌊 **Submersible Water Pump** *(optional)*              | AC pump connected to one relay for dispensing or irrigation.                                       | 1         |
| 🚰 **Solenoid Valve (220V AC)** *(optional)*            | Used for fluid control, connected to the second relay.                                             | 1         |
| 🔌 **5V Power Supply / USB Cable**                      | Powers the ESP8266 board. Use stable 5V (minimum 500mA).                                           | 1         |
| ⚡ **Relay Power Source (220V AC)**                      | Main AC supply line connected to the relay’s COM & NO/NC terminals.                               | 1         |
| 🧪 **Resistors / Jumpers / Breadboard** *(for testing)* | For initial prototyping and safer wiring.                                                          | As needed |
| 🔩 **Enclosure Box (optional)**                         | For mounting and safe insulation of live wires and relays.                                         | 1         |

---

### ⚠️ **Safety Note**

Since relays switch **220V AC**, take proper precautions:

* Use **screw terminals** and insulation
* Do **not** touch live wires while testing
* Keep the low-voltage (ESP8266 side) isolated from AC terminals

---


