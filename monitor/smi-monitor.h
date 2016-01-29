typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

int checkSwbCRC(char *dataBuffer, int bufferSize);
int checkSmiCRC(char *dataBuffer, int bufferSize);
int addSwbCrc(char *dataBuffer, int bufferSize);

void printBuffer(char *buffer, int size);
uint16_t createSwbCRC(char *buffer, int size);
