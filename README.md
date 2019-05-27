# Arduino_RF_NRF24L01
RF data transmission between two arduino boards using the RF module NRF24L01.
Target is to transmit data to log, e.g. ambient temperature, relative humidity, status of other sensors like garage door open/closed etc.

This project can be used to transmit data between two Arduino Nano boards.

The sources for the transmitter are located in file tx_simple.ino 
The sources for the receiver are located in file rx_simple.ino 

The transmitter SW includes the support for the following sensors:
- Temperature and Humidity (DHT)

