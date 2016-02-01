typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

/* print buffer to stdio */
void printBuffer(char *buffer, int size);
/* open ports to Switch-bus (SWB) */
int openSwbPortDiv (char *port, int divisor);
/* display buffer in hex-format to stdout */
int openSwbPort (char *port);
/* open port to smi-bus */
int openSmiPort (char *port);
/* check swb crc */
int checkSwbCrc(char *buffer, int size);
/* check smi checksum */
int checkSmiCrc(char *buffer, int size);
/* add the crc to an existing message */
void addSwbCrc(char *buffer, int size);
/* create response of received message */
void createSwbAck(char *buffer, int size);
/* creates SWB crc16 */
uint16_t createSwbCRC(char *buffer, int size);
