/* smi-monitor.h */
#ifndef SMI_MONITOR_H
#define SMI_MONITOR_H


/* print buffer to stdio (HEX)*/
void printBuffer(unsigned char *buffer, int size);
/* print buffer to stdio (ASCII)*/
void printBufferAscii(unsigned char *buffer, int size);
/* nonblocking for network socket */
int setNonblocking(int fd);
/* extract post data */
int getPostData(unsigned char *buffer, int size);


#endif /* smi-monitor.h */
