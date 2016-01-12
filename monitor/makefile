CC = gcc
OPT = -g -Wall

# default target
default: monitor

# compile only monitor
monitor:  smi-monitor.c
	$(CC) $(OPT) -o smi-monitor smi-monitor.c
  
# removes all object, temporary an executable files
clean:
	$(RM) count *.o *~
  
