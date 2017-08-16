/* swb-serial.h */
#ifndef SWB_SERIAL_H
#define SWB_SERIAL_H

#import "types.h"
// typedef unsigned char uint8_t;
// typedef unsigned int  unint16_t;

/* print buffer to stdio */
void printSwbBuffer(unsigned char *buffer, int size);
// /* open ports to Switch-bus (SWB) */
// int openSwbPortDiv (char *port, int divisor);
// /* display buffer in hex-format to stdout */
// int openSwbPort (char *port);
//
// /* check swb crc */
// int checkSwbCrc(unsigned char *buffer, int size);

void addSwbCrc(unsigned char *buffer, int size);

// /* create response of received message */
// void createSwbAck(unsigned char *buffer, int size);

/* creates SWB crc16 */
unint16_t createSwbCRC(unsigned char *buffer, int size);

#endif /* swb-serial.h */
