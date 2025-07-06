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

