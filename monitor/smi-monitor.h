typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

void printBuffer(char *buffer, int size);
int addSwbCrc(char *buffer, int size);
uint16_t  createSwbCRC(char *buffer, int size);

int checkSwbCRC(char *dataBuffer, int bufferSize);
int checkSmiCRC(char *dataBuffer, int bufferSize);
