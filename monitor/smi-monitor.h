typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

/* not used at the moment */
int open_port(void);

/* print buffer to stdio */
void printBuffer(char *buffer, int size);

/* open ports to Switch-bus (SWB) */
int openSwbPort (char *port, int divisor);
int openSwbPort (char *port);
/* open port to smi-bus */
int openSmiPort (char *port);

/* check crc */
int checkSwbCrc(char *buffer, int size);
int checkSmiCrc(char *buffer, int size);

void addSwbCrc(char *buffer, int size);
void createSwbAck(char *buffer, int size);

uint16_t createSwbCRC(char *buffer, int size);
