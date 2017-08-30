# Makefile

## You can use CC CFLAGS LD LDFLAGS CXX CXXFLAGS AR RANLIB READELF STRIP after include env.mak
include /env.mak

EXEC= monitor/smi-monitor monitor/smi-serial.o monitor/swb-serial.o
OBJS= monitor/smi-monitor.o monitor/smi-serial.o monitor/swb-serial.o


all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

install: $(EXEC)
	mkdir -p $(DESTDIR)/usr/bin/
	install $< $(DESTDIR)/usr/bin/

clean:
    rm -rf *.o $(EXEC)
