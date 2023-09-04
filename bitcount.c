#include <stdio.h>
#include <assert.h>

int bitcount(unsigned int  number) {
    int counter = 0;
    while(number) {
        if (number & 0x01) counter++;
        number = number >> 1;
    }
    return counter;
}

int main() {
    assert(bitcount(0) == 0); // 0b0
    assert(bitcount(1) == 1); // 0b01
    assert(bitcount(3) == 2);
    assert(bitcount(8) == 1);
    assert(bitcount(255) == 8);
    assert(bitcount(0xff) == 8);

    assert(bitcount(0xffffffff) == 32);
    
    printf("OK\n");


}
