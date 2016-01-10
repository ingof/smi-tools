
CC = /opt/bin/gcc
OPT = -g -Wall

# default target
default: all

# compile all files
all:  /comandline/smi.c /monitor/smi-monitor.c
	$(CC) $(OPT) -o smi /comandline/smi.c
	$(CC) $(OPT) -o smi-monitor /monitor/smi-monitor.c
  
# compile only smi commandline
smi:  /comandline/smi.c
	$(CC) $(OPT) -o smi /comandline/smi.c

# compile only monitor
monitor:  /monitor/smi-monitor.c
	$(CC) $(OPT) -o smi-monitor /monitor/smi-monitor.c
  
# removes all object, temporary an executable files
clean:
	$(RM) count *.o *~
  
