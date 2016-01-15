CC = gcc
OPT = -g -Wall

# default target
default: monitor

# compile only monitor
monitor:  hex-monitor.c smi-monitor.c
	$(CC) $(OPT) -o hex-monitor hex-monitor.c
	$(CC) $(OPT) -o smi-monitor smi-monitor.c
  
# removes all object, temporary an executable files
clean:
	$(RM) count *.o *~
  
