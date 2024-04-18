# UART Asynchronous Communication with GSM To Transmit Receive Messages and Making a Call

This example shows how to use the GSM interface for sending and receiving messages, as well as making calls. It's a basic setup that you can build upon for more advanced serial communication applications.

## How to use example

### Hardware Required

The example can be run on any commonly available ESP32, ESP32-S and ESP32-C series based development board. You will need a USB cable to connect the
development board to a computer, and a simple one-wire cable for shorting two pins of the board.

### Setup the Hardware

The `RXD_PIN` and `TXD_PIN` which are configurable in the code (by default `GPIO32` and `GPIO14`) need to be configured in
order to receive and transmit the data.

### Configure the project

```
idf.py menuconfig
```

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output

Following are the list of AT commands used in this project:
######################################################################################################################################
AT+CMGF=1 : To convert SMS to text
AT+CMGS=\"%s : Send text to mobile number
ATD%s : To make a call
AT+CMGL=\"ALL : It will respond with a list of all SMS messages stored in memory, including read, unread, sent, and received messages.
AT+CMGR=1 : For reading messages (including the sender's phone number, timestamp, and message text).
######################################################################################################################################

