SMI-tools
=========

This is a small smi commandline tool.

you need a USB to SMI interface or a serial to SMI interface wich is connected to the usb-port.
The tool only supports installed drives with a given id.
try the tool at your own risk.

i connected my smi-interface to a synology diskstation.

at the moment there is no check of the commandline-parameters.

Send command to smi drive:
--------------------------
**Syntax:** smi driveid command [serial-port]<br>
* **required:** driveID<br>
the ID of the drive. (0..15)
* **optional:** command<br>
stop, up, hoch, down, runter<br>
default is stop
* **optional:** serial port<br>
default is /dev/ttyUSB0<br>
**example:**
smi drive should go up an is connected via /dev/ttyS0
''' smi 12 up /dev/ttyS0 '''

monitor data on the smi-bus:
----------------------------
**Syntax:** smi-monitor [serial-port]<br>
* **optional:** serialport<br>
default is /dev/ttyUSB1

now can see the received data in as hexdata

Clone git repository and compile
--------------------------------
```
git clone https://github.com/ingof/smi-tools.git
cd smi-tools
make
```



