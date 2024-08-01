# Battery-management-system
BMS-ELECTRIC VEHICLES
This project implements a Battery Management System (BMS) using an ESP32 microcontroller, Blynk for IoT functionality, an OLED display for local data visualization, a DHT11 sensor for temperature and humidity measurements, and an ACS712 current sensor for current measurement.

******Overview******
The BMS monitors and manages battery voltage, current, and temperature, taking appropriate actions if any parameter goes outside safe limits. It includes functionalities for remote monitoring and control via Blynk, as well as local data display on an OLED screen.

****Features****
Real-time battery monitoring (voltage, current, temperature)
Visual alerts via LEDs for different statuses (charging, discharging, safety, threat)
Remote monitoring and control using Blynk app
Local data visualization on OLED display
Automated control using relays based on battery state.

****Hardware Components****

ESP32 microcontroller,
DHT11 temperature and humidity sensor,
ACS712 current sensor,
OLED display (SSD1306),
Relays,
LEDs,
Buzzer,
Miscellaneous: resistors, wires, breadboard, etc.

****Software Components****

Arduino IDE,
Blynk Library,
Adafruit GFX and SSD1306 Libraries,
DHT Sensor Library,
ACS712 Library,
WiFi Library.
