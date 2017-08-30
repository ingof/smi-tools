EXEC=smi-monitor
OBJS=smi-monitor.o

CC=/usr/local/i686-pc-linux-gnu/bin/i686-pc-linux-gnu-gcc \
LD=/usr/local/i686-pc-linux-gnu/bin/i686-pc-linux-gnu-ld \
RANLIB=/usr/local/i686-pc-linux-gnu/bin/i686-pc-linux-gnu-ranlib \
CFLAGS="-I/usr/local/i686-pc-linux-gnu/i686-pc-linux-gnu/sys-root/usr/include" \
LDFLAGS="-L/usr/local/i686-pc-linux-gnu/i686-pc-linux-gnu/sys-root/lib"


./configure \
	    --host=i686-pc-linux-gnu \
	    --target=i686-pc-linux-gnu \
	    --build=i686-pc-linux \
	    --prefix=/usr/local

all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean:
    rm -rf *.o $(PROG) *.core
