
CC = gcc
OPT = -g -Wall

# default target
default: all

# compile all files
all:  comandline/smi.c monitor/smi-monitor.c
	cd ./comandline
	$(CC) $(OPT) -o smi comandline/smi.c
	cd ./monitor
	$(CC) $(OPT) -o smi-monitor monitor/smi-monitor.c
  
# compile only smi commandline
smi:  comandline/smi.c
	$(CC) $(OPT) -o smi comandline/smi.c

# compile only monitor
monitor:  monitor/smi-monitor.c
	$(CC) $(OPT) -o smi-monitor monitor/smi-monitor.c
  
# removes all object, temporary an executable files
clean:
	$(RM) count *.o *~
	rm -rf ./
	
# removes all object, temporary an executable files
install:
	rm /usr/local/bin/smi-monitor
	cp ./monitor/smi /usr/local/bin/
	cp ./comandline/smi-monitor /usr/local/bin/
  
