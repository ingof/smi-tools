#include <stdio.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;


// das ist die inverse Berechnung der CRC
uint16_t  checkSum(uint8_t *input, uint8_t length)
{
  uint16_t crc = 0xffff;    // CRC Vorbelegung
  uint16_t CRC = 0x8408;    // für reverse Berechnung CRC-16-CCITT

  int i,j;

  for (i=0; i < length; i++){
    crc = crc ^ input[i];
    for (j=0; j<8; j++){
      if((crc & 0x01) == 0){
        crc = crc >> 1;
      }
      else{
        crc = crc >> 1;
        crc = crc ^ CRC;
      }
    //   printf("\ncrc(%d):\t%04x\t%04x",j,crc,~crc);
    }
    // printf("\nbyte: %d",i);
  }
return crc;
}



int main(void) {
   int j;
   for ( j = 0; j <= 255; j++) {
       uint8_t buffer[10];
       uint16_t i, num;

       buffer[0]= 0xf0;
       buffer[1]= 0x24;
       buffer[2]= 0xd3;
       buffer[3]= 0x81;
       buffer[4]= j;

       num = ~checkSum(buffer,5);    // Komplement der CRC

       buffer[5]= (uint8_t) num; //(Lowbyte. uint8_t oder char typcasten)
       buffer[6]= (uint8_t) (num>>8); // Highbyte

     printf("\nMsg: ");
     for(i=0;i<7;i++)
       printf("%02X ", buffer[i]);
   }
   for ( j = 0; j <= 255; j++) {
       uint8_t buffer[10];
       uint16_t i, num;

       buffer[0]= 0xf0;
       buffer[1]= 0x24;
       buffer[2]= 0xd3;
       buffer[3]= 0x81;
       buffer[4]= j;

       num = ~checkSum(buffer,5);    // Komplement der CRC

       buffer[5]= (uint8_t) num; //(Lowbyte. uint8_t oder char typcasten)
       buffer[6]= (uint8_t) (num>>8); // Highbyte

     printf("\nMsg: ");
     for(i=0;i<7;i++)
       printf("%02X ", buffer[i]);
   }
   for ( j = 0; j <= 255; j++) {
       uint8_t buffer[10];
       uint16_t i, num;

       buffer[0]= 0xf0;
       buffer[1]= 0x24;
       buffer[2]= 0xd3;
       buffer[3]= 0x01;
       buffer[4]= j;

       num = ~checkSum(buffer,5);    // Komplement der CRC

       buffer[5]= (uint8_t) num; //(Lowbyte. uint8_t oder char typcasten)
       buffer[6]= (uint8_t) (num>>8); // Highbyte

     printf("\nMsg: ");
     for(i=0;i<7;i++)
       printf("%02X ", buffer[i]);
   }
  return  0;
}
