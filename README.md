SMI-tools
=========

This is a small smi commandline tool.

you need a USB to SMI interface or a serial to SMI interface wich is connected to the usb-port.
The tool only supports installed drives with a given id.
try the tool at your own risk.

the smi-tools will work on "Synology Diskstation" and on "Raspberry Pi 2"

at the moment there is no check of the commandline-parameters.

Send command to smi drive:
--------------------------
**Syntax:** smi driveid command [serial-port] [serialWait]<br>
* **required:** driveID<br>
the ID of the drive. (0..15)
* **optional:** command<br>
stop, up, hoch, down, runter<br>
default is stop
* **optional:** serial port<br>
default is /dev/ttyUSB1
* **optional:** serialWait<br>
default is 60 (ms)

**example:**<br>
smi drive should go up and is connected via /dev/ttyS0 and is waiting 60ms for Answer
```
smi 12 up /dev/ttyS0 60
```

monitor data on the smi-bus only:
----------------------------
**Syntax:** hex-monitor [serial-speed] [serial-port] [serial-wait]<br>
* **optional:** serialSpeed<br>
default is 2400
* **optional:** serialport<br>
default is /dev/ttyUSB0
* **optional:** serialWait<br>
default is 40 (ms)


**example:**<br>
log on /dev/ttyUSB0 at 19.200Bit/s and wait 5ms for whole telegramm
```
hex-monitor 19200 /dev/ttyUSB0 5
```
now can see the all received data as hexdata


Clone git repository compile and install
----------------------------------------
```
git clone https://github.com/ingof/smi-tools.git
cd smi-tools
make
sudo make install
```



