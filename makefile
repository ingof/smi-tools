
CC = gcc
OPT = -g -Wall

# default target
default: all

# compile all files
all:  comandline/smi.c monitor/smi-monitor.c
	cd ./comandline
	$(CC) $(OPT) -o comandline/smi comandline/smi.c
	cd ./monitor
	$(CC) $(OPT) -o monitor/smi-monitor monitor/smi-monitor.c
  
# compile only smi commandline
smi:  comandline/smi.c
	$(CC) $(OPT) -o comandline/smi comandline/smi.c

# compile only monitor
monitor:  monitor/smi-monitor.c
	$(CC) $(OPT) -o monitor/smi-monitor monitor/smi-monitor.c
  
# removes all object, temporary an executable files
clean:
	$(RM) count *.o *~
	rm -rf ../smi-tools *
	cd ..
	
# removes all object, temporary an executable files
install:
	cp monitor/smi-monitor /usr/local/bin/
	cp comandline/smi /usr/local/bin/
  
