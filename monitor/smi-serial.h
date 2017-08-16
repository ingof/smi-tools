typedef unsigned char uint8_t;
typedef unsigned int  unint16_t;

/* open port to smi-bus */
int openSmiPort (char *port);
/* check smi checksum */
int checkSmiCrc(unsigned char *buffer, int size);
/* add the cheksum byte to the buffer and returns number of cheksum bytes */
int addSmiCrc(unsigned char *buffer, int size);
/* add the crc to an existing message */
