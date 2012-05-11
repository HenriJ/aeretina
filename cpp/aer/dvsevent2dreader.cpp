#include "dvsevent2dreader.h"

#include <iostream>

DvsEvent2dReader::DvsEvent2dReader()
{
    file.open("/dev/retina0");

    /*char bias[] = {0xb8,               //request
                            0x00, 0x00,         //value
                            0x00, 0x00,          //index
                            0x00,0x07,0xc8,	    // cas
                            0x10,0xe9,0x8C,		// injGnd
                            0xFF,0xFF,0xFF,		// reqPd
                            0x7c,0x7f,0xf5,		// puX
                            0x00,0x00,0x84,		// diffOff          ++
                            0x02,0x6d,0xab,		// req
                            0x00,0x03,0xc9,		// refr             ++ refractory of the pixels
                            0xFF,0xFF,0xFF,		// puY
                            0x03,0x34,0x4c,		// diffOn           ++
                            0x00,0x33,0x45,		// diff
                            0x00,0x01,0x0f,		// foll
                            0x00,0x01,0x0f}; 	// Pr               ++ Velocity of the "log circuit"

//    char bias[] = {0xb8,               //request
//                            0x00, 0x00,         //value
//                            0x00, 0x00,          //index
//                            0x00,0x00,0x36,	    // cas
//                            0x10,0xe9,0x8C,		// injGnd
//                            0xFF,0xFF,0xFF,		// reqPd
//                            0x7c,0x7f,0xf5,		// puX
//                            0x00,0x00,0x84,		// diffOff
//                            0x02,0x6d,0xab,		// req
//                            0x00,0x00,0x06,		// refr
//                            0xFF,0xFF,0xFF,		// puY
//                            0x07,0x5c,0x8b,		// diffOn
//                            0x00,0x75,0xc9,		// diff
//                            0x00,0x00,0x33,		// foll
//                            0x00,0x00,0x03}; 	// Pr

    //std::cout << (int)( (unsigned char) bias[0]) << std::endl;

    //file.write(bias, 41);

    char start[] = {0xb3, 0, 0, 0, 0};

    //file.write(start, 5);*/
}

DvsEvent2dReader::~DvsEvent2dReader()
{
    close();
}

void DvsEvent2dReader::close()
{
    //char stop[] = {0xb4, 0, 0, 0, 0};
    //file.write(stop, 5);
    std::cout << "DVS stop sent" << std::endl;
    //file.close();
}

bool DvsEvent2dReader::hasNext()
{
    file.peek();
    return !file.eof();
}

Event2d DvsEvent2dReader::readEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    Event2d e = bufferToEvent2d(buffer);

    if (e.t < last_t) {
        shift_t += last_t;
    }

    last_t = e.t;

    e.t += shift_t;

    return e;
}
