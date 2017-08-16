// typedef unsigned char uint8_t;
// typedef unsigned int  unint16_t;

/* print buffer to stdio */
void printBuffer(unsigned char *buffer, int size);

/* nonblocking for network socket */
int setNonblocking(int fd);

/* open port to smi-bus */
int openSmiPort (char *port);
/* check smi checksum */
int checkSmiCrc(unsigned char *buffer, int size);
/* add the cheksum byte to the buffer and returns number of cheksum bytes */
int addSmiCrc(unsigned char *buffer, int size);
/* add the crc to an existing message */

/* extract post data */
int getPostData(unsigned char *buffer, int size);
