CC = gcc
OPT = -g -Wall

# default target
default: all

# compile only monitor
all:
	$(CC) $(OPT) -o hex-monitor hex-monitor.c
	$(CC) $(OPT) -o smi-monitor smi-monitor.c
  
# removes all object, temporary and executable files
clean:
	$(RM) count *.o *~
  
