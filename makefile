


all:  /commandline/smi.c /monitor/smi-monitor.c
  /opt/bin/gcc -o smi /commandline/smi.c
  /opt/bin/gcc -o smi-monitor /monitor/smi-monitor.c
  
smi:  /commandline/smi.c
  /opt/bin/gcc -o smi /commandline/smi.c

monitor:  /monitor/smi-monitor.c
  /opt/bin/gcc -o smi-monitor /monitor/smi-monitor.c
  
