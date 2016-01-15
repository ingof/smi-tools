CC = gcc
OPT = -g -Wall

# default target
default: all

# compile all files
all:  comandline/smi.c monitor/smi-monitor.c
	cd comandline && $(MAKE) all
	cd monitor && $(MAKE) all

# compile only smi commandline
smi:  comandline/smi.c
	cd comandline && $(MAKE) all

# compile only monitor
monitor:  monitor/smi-monitor.c
	cd monitor && $(MAKE) all

# removes all object, temporary and executable files
clean:
	$(RM) count *.o *~
	rm -rf ../smi-tools *
	
# installes the executable files
install:
	cp monitor/smi-monitor /usr/local/bin/
	cp comandline/smi /usr/local/bin/
  
