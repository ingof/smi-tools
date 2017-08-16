CC = gcc
OPT = -g -Wall

# default target
default: all

# compile only monitor
all:	hex-monitor.c smi-monitor.c crc.c swb-serial.c
	$(CC) $(OPT) -o hex-monitor hex-monitor.c
	$(CC) $(OPT) -o smi-monitor swb-serial.c smi-monitor.c
	$(CC) $(OPT) -o crc crc.c

# removes all object, temporary and executable files
clean:
	$(RM) count *.o *~
