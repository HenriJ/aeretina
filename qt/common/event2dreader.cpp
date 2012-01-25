#include "event2dreader.h"

using namespace std;

Event2dReader::Event2dReader()
{
}

Event2d Event2dReader::bufferToEvent2d(char * buffer)
{
    int i = 0;

    unsigned int part_1 = 0x000000FF&buffer[i];
    unsigned int part_2 = 0x000000FF&buffer[i+1];
    unsigned int part_3 = 0x000000FF&buffer[i+2];
    unsigned int part_4 = 0x000000FF&buffer[i+3];

    unsigned int blob = (part_4)|(part_3<<8)|(part_2<<16)|(part_1<<24);

    int x = (blob & 0x000000fE)>>1;
    int y   = (blob & 0x00007f00)>>8;
    int p = (((blob & 0x00000001)>>0)==0)?-1:1;	//+1 ON, -1 OFF

    unsigned int part_5 = 0x000000FF&buffer[i+4];
    unsigned int part_6 = 0x000000FF&buffer[i+5];
    unsigned int part_7 = 0x000000FF&buffer[i+6];
    unsigned int part_8 = 0x000000FF&buffer[i+7];

    unsigned int t = (part_8)|(part_7<<8)|(part_6<<16)|(part_5<<24);

    return Event2d(x, y, p, t);
}
