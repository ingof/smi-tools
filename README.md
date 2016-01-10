SMI-tools
=========

This is a small smi commandline tool.

you need a USB to SMI interface or a serial to SMI interface wich is connected to the usb-port.
The tool only supports installed drives with a given id.
try the tool at your own risk.

i connected my smi-interface to a synology diskstation.

at the moment there is no check of the commandline-parameters.

**Syntax:** smi driveid command [serial-port]

* **required**: driveID
the ID of the drive. (0..15)
* **required**: command
stop, up, hoch, down, runter

**optional: serialport**<br>
default is /dev/ttyUSB0

example:
smi drive should go up an is connected via /dev/ttyS0
smi 12 up /dev/ttyS0

Clone git repository and compile
--------------------------------
```
git clone https://github.com/ingof/smi-tools.git
cd smi-tools
make
```



