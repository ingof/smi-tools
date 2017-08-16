SMI-tools
=========

This is a small smi commandline tool.

You need a USB to SMI interface or a serial to SMI interface wich is connected to the usb-port.
For smi-monitor you should have an extra usb-RS485-adapter for the Switch-Bus.
The tool only supports installed drives with a given id.
Try the tool at your own risk.

The smi-tools are testet on "Synology Diskstation DSM6.1" and on "Raspberry Pi 2"

At the moment there is no check of the commandline-parameters.

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

**Example:**<br>
Smi drive should go up and is connected via /dev/ttyS0 and is waiting 60ms for Answer
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


**Example:**<br>
log on /dev/ttyUSB0 at 19.200Bit/s and wait 5ms for whole telegramm
```
hex-monitor 19200 /dev/ttyUSB0 5
```
now can see the all received data as hexdata


Monitor the data on the smi-bus and swb-bus:
--------------------------------------------
**Syntax:** smi-monitor [serialSwbPort0] [serialSwbWait] [serialSwbDivisor] [serialSwbAck] [serialSmi-port0] [serialSmiWait]<br>
* **optional:** serialSwbPort0<br>
This is the serial-port for the SwitchBus (19200 Bit/s)
default is /dev/ttySWB0
* **optional:** serialSwbWait<br>
The time to receive a whole SWB telegram
default is 3 (ms)
* **optional:** serialSwbDivisor<br>
This is the divisor for special Baudrates
default is 0
* **optional:** serialSwbAck<br>
If set to 1 SWB-telegrams will automatically achnowlegded. Must set if smi-monitor will be the Busmaster.
default is 0
* **optional:** serialSmi-port0<br>
default is /dev/ttySMI0
The serial-port for the SMI-Bus (2400 Bit/s)
* **optional:** serialSmiWait<br>
default is 10 (ms)


**Example:**<br>
log on /dev/ttyUSB0 (3ms wait, 19200Bit/s, master) and on /dev/ttyUSB1 (13ms wait)
```
smi-monitor /dev/ttyUSB0 3 0 1 /dev/ttyUSB1 40
```
Now can see the all received data as hexdata


Clone git repository compile and install
----------------------------------------
```
git clone https://github.com/ingof/smi-tools.git
cd smi-tools
make
sudo make install
```



