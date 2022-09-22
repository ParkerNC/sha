#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#define uchar unsigned char

enum K{
    t0 = 0x5a827999,
    t20 = 0x6ed9eba1,
    t40 = 0x8f1bbcdc,
    t60 = 0xca62c1d6
};

uint32_t H0 = 0x67452301;
uint32_t H1 = 0xefcdab89;
uint32_t H2 = 0x98badcfe;
uint32_t H3 = 0x10325476;
uint32_t H4 = 0xc3d2e1f0;

void ROTL(uint32_t x, unsigned int n){
    x = (x >> n) | (x << (32 - n));
}

std::vector<uint32_t> padd(std::vector<uint32_t> M){

}

int main(){

}