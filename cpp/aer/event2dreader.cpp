#include "event2dreader.h"

using namespace std;

Event2dReader::Event2dReader()
{
}

Event2dReader::~Event2dReader()
{
}

Event2d Event2dReader::bufferToEvent2d(char * buffer) const
{
    // Thank you for the code Joao & Charles

    int i = 0;

    unsigned int part_1 = 0x000000FF&buffer[i];
    unsigned int part_2 = 0x000000FF&buffer[i+1];
    unsigned int part_3 = 0x000000FF&buffer[i+2];
    unsigned int part_4 = 0x000000FF&buffer[i+3];

    unsigned int blob = (part_4)|(part_3<<8)|(part_2<<16)|(part_1<<24);

    int x = (blob & 0x000000fE)>>1;
    int y = (blob & 0x00007f00)>>8;
    int p = (((blob & 0x00000001)>>0)==0) ? -1 : 1;	//+1 ON, -1 OFF

    unsigned int part_5 = 0x000000FF&buffer[i+4];
    unsigned int part_6 = 0x000000FF&buffer[i+5];
    unsigned int part_7 = 0x000000FF&buffer[i+6];
    unsigned int part_8 = 0x000000FF&buffer[i+7];

    unsigned int t = (part_8)|(part_7<<8)|(part_6<<16)|(part_5<<24);

    return Event2d(x, y, p, t);
}

Event2d Event2dReader::atisToEvent2d(char * buffer) const
{
    int i = 0;

    unsigned int part_1 = 0x000000FF&buffer[i];
    unsigned int part_2 = 0x000000FF&buffer[i+1];
    unsigned int part_3 = 0x000000FF&buffer[i+2];
    unsigned int part_4 = 0x000000FF&buffer[i+3];

    unsigned int blob = (part_4<<24)|(part_3<<16)|(part_2<<8)|(part_1);

    int x = (blob & 0xFFF000)>>12;
    int y = (blob & 0xFFF);
    int p = (((blob & 0x80000000)>>31)==0) ? -1 : 1;	//+1 ON, -1 OFF

    unsigned int part_5 = 0x000000FF&buffer[i+4];
    unsigned int part_6 = 0x000000FF&buffer[i+5];
    unsigned int part_7 = 0x000000FF&buffer[i+6];
    unsigned int part_8 = 0x000000FF&buffer[i+7];

    unsigned int t = (part_8<<24)|(part_7<<16)|(part_6<<8)|(part_5);

    return Event2d(x, y, p, t);
}
