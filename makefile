CC = gcc
OPT = -g -Wall

# default target
default: all

# compile all files
all:  comandline/smi.c monitor/smi-monitor.c
	cd comandline && $(MAKE) all
	cd monitor && $(MAKE) all
	#cd ./comandline
	#$(CC) $(OPT) -o comandline/smi comandline/smi.c
	#cd ./monitor
	#$(CC) $(OPT) -o monitor/smi-monitor monitor/smi-monitor.c
  
# compile only smi commandline
smi:  comandline/smi.c
	cd comandline && $(MAKE) all
	#$(CC) $(OPT) -o comandline/smi comandline/smi.c

# compile only monitor
monitor:  monitor/smi-monitor.c
	cd monitor && $(MAKE) all
	#$(CC) $(OPT) -o monitor/smi-monitor monitor/smi-monitor.c
	
# removes all object, temporary and executable files
clean:
	$(RM) count *.o *~
	rm -rf ../smi-tools *
	
# installes the executable files
install:
	cp monitor/smi-monitor /usr/local/bin/
	cp comandline/smi /usr/local/bin/
	#cd comandline && $(MAKE) all
	#cd monitor && $(MAKE) all

  
