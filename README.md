# esp32-cnc-plotter
Low-cost CNC plotter built with ESP32,H bridge, and recycled CD drive stepper motors

This is a project is kind of a diy project.This was made to repurpose the laptop dvd drives that are replaced with ssd caddy.  

DIY 2-axis CNC plotter using ESP32 and CD/DVD stepper motors.

## 🚀 Features
* 2-axis motion (X & Y)
* ESP32 control
* Simultaneous movement
* Low-cost design(used H bridge instead of stepper motor driver)
* 
## 🔧 Hardware Used

* ESP32 S3 
* CD/DVD stepper motors
* MX1508 motor drivers
* Power supply
  
## How to Use
Upload code to ESP32 and send commands via Serial Monitor:

* F100 - moves 100 steps forward  
* B100 - moves 100 steps backward
* R100 - moves 100 steps to right 
* L100 - moves 100 steps to left
* U    - lifts the pen up
* D    - lowers the pen down

## 📈 Future Improvements
* Add Z-axis
* G-code support
* WiFi control
  
## ⚠️ Notes
* Motors may heat up
* Not highly precise (CD motors)
