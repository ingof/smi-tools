void printBuffer(char *buffer, int size);
int addSwbSmi(char *buffer, int size);
uint16_t  createSwbCRC(char *buffer, int size);

int checkSwbCRC(char *dataBuffer, int bufferSize);
int checkSmiCRC(char *dataBuffer, int bufferSize);
