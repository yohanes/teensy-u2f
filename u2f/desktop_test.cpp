//to prevent arduino IDE from compiling this
#ifdef IS_DESKTOP_TEST

//test in desktop
#define _POSIX_C_SOURCE 200809L

#include <sys/time.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//for storing fake eprom
#include <map>
//for fake input
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>

#define DESKTOP_TEST

//fake eprom
#define F(X) X

typedef unsigned char byte;

//this is used for random

enum OUTPUT_FORMAT_ENUM
{
    HEX = 1
};

std::vector<std::string> fake_input;
int current_fake_input;

int hexchar2int(int c)
{
    if (c<='9')
	return c-'0';
    return 10 + (c-'A');
}

void hex2bytes(const std::string & inp, unsigned char **res, int *len)
{
    std::string tmp;
    //ignore non hex characters (e.g: space, tab)
    for (size_t i = 0; i < inp.size(); i++) {
	int c = toupper(inp[i]);
	if ((c>='0' && c<='9') || (c>='A' && c<='F')) {
	    tmp += (char)c;
	}
    }
    *len = tmp.size()/2;    
    unsigned char *tmpres = (unsigned char *)malloc(*len);
    size_t j =0;
    for (size_t i = 0; i < tmp.size(); i+=2, j++) {
	unsigned int c1 = hexchar2int(tmp[i]);
	unsigned int c2 = hexchar2int(tmp[i+1]);
	unsigned int c = (c1 << 4) + c2;
	tmpres[j] = c;
    }
    *res = tmpres;

}

int get_next_fake_input(unsigned char **res)
{
    if (current_fake_input >= fake_input.size())
	return -1;
	    
    int len;
    printf("CURRENT FAKE INPUT: %s\n", fake_input[current_fake_input].c_str());
    hex2bytes(fake_input[current_fake_input++], res, &len);
    return len;   
}

void read_file(const std::string & filename)
{
    std::ifstream file(filename);
    std::string temp;
    while(std::getline(file, temp)) {
	fake_input.push_back(temp);
    }
    current_fake_input = 0;
}


int RNG(uint8_t *dest, unsigned size)
{
    for (int i =0; i < size; i++) {
	dest[i] = rand() % 255;
    }
    return 1;
}

long system_millis()
{
#if 0    
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;
    
    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

    return s*1000 + ms;
#endif
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    
    long time_in_mill = 
	(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
    return time_in_mill;
}


int millis()
{
    
    return 0;
}

void delayMicroseconds(int micro)
{

}

class EEPROMClass {
    std::map<int, unsigned int> values;
public:
    void get(int address, unsigned int &value)
	{
	    value =  values[address];
	}
    void put(int address, int value)
	{
	    values[address] = value;
	}
};

class SerialClass {

public:
	void begin(int speed) {
} 
void print(const char *msg)
{
    printf("%s", msg);
}
    void println() {
	printf("\n");
    }
void println(const char *msg)
	{
	    printf("%s\n", msg);
	}
void println(int number)
	{
	    printf("%d\n", number);
	}
void print(int number, OUTPUT_FORMAT_ENUM e)
	{
	    printf("%02x", number);
	}
void println(int number, OUTPUT_FORMAT_ENUM e)
	{
	    printf("%02x", number);
	}
};

class RawHIDClass {
public:
void send(byte *buffer, int to)
	{
	    printf("HID SEND: ");
	    for (int i =0; i < 64; i++) {
		printf("%02x ", buffer[i]);
	    }
	    printf("\n");
	}
int recv(byte *buffer, int timeout)
	{
	    unsigned char *inp;
	    int len = get_next_fake_input(&inp);
	    if (len==-1) {
		printf("END OF INPUT\n");
		exit(0);
	    }
	    printf("HID READ: ");
	    for (int i =0; i < len; i++) {
		printf("%02x ", inp[i]);
	    }
	    printf("\n");
	    memcpy(buffer, inp, len);
	    free(inp);	    
	    return len;
	}
};

SerialClass Serial;
RawHIDClass RawHID;
EEPROMClass EEPROM;


#include "u2f.ino"

int main(int argc, char *argv[])
{
    if (argc<2) {
	printf("usage desktop_test <INPUT>\n");
	return 0;
    }
    read_file(argv[1]);
    setup();
    while (1) {
	loop();
    }
}

#endif
