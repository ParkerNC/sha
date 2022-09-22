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

std::vector<uint8_t> padd(std::vector<uint8_t> M){
    unsigned long l = M.size() * 8;
    unsigned long pad = 512 - (l % 512) - 65;

    if (pad < 512){
        pad += 512;
    }

    std::cout << ((pad + l + 65) / 8) << std::endl;

    return M;
}

int main(){

    std::string test1 = "This is a test of SHA-1.";
    std::string test2 = "Kerckhoff’s principle is the foundation on which modern cryptography is built.";
    std::string test3 = "SHA-1 is no longer considered a secure hashing algorithm.";
    std::string test4 = "SHA-2 or SHA-3 should be used in place of SHA-1.";
    std::string test5 = "Never roll your own crypto!";

    std::vector<uint8_t> byteM(test1.begin(), test1.end());
    std::cout << test1.length() << std::endl;

    padd(byteM);

    return 0;
}