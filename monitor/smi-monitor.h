typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

int checkSwbCrc(char *buffer, int size);
int checkSmiCrc(char *buffer, int size);

void addSwbCrc(char *buffer, int size);
void createSwbAck(char *buffer, int size);

void printBuffer(char *buffer, int size);
uint16_t createSwbCRC(char *buffer, int size);
