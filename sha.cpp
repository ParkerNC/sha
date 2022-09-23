#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <iomanip>
#include <bitset>

enum K{
    t0 = 0x5a827999,
    t20 = 0x6ed9eba1,
    t40 = 0x8f1bbcdc,
    t60 = 0xca62c1d6
};

uint32_t ROTL(uint32_t x, unsigned int n){
    return (x << n) | (x >> (32 - n));
}

std::vector<uint8_t> padd(std::vector<uint8_t> M){
    unsigned long l = M.size() * 8;
    std::cout <<  M.size() << '\n';
    unsigned long pad = 512 - (l % 512) - 65;

    if (l >= (448 % 512)){
        pad += 512;
    }

    std::vector<uint8_t> paddedM((pad + l + 65)/8);

    std::copy(M.begin(), M.end(), paddedM.begin());

    paddedM[M.size()] = 1 << 7;
    for (unsigned int i = 1; i < ((pad - 64)/8); i++){
        paddedM[i + M.size()] = 0;
    }

    paddedM[paddedM.size()] = M.size();

    uint64_t len64 = M.size() * 8;

    for (unsigned int i = 0; i < 8; i++){
        paddedM[(paddedM.size()-8) + i] = len64 >> (56 - 8 * i) & 0xFF;
    }

    return paddedM;
}

std::vector<unsigned char> sha(std::vector<uint8_t> input){
    uint32_t H0 = 0x67452301;
    uint32_t H1 = 0xefcdab89;
    uint32_t H2 = 0x98badcfe;
    uint32_t H3 = 0x10325476;
    uint32_t H4 = 0xc3d2e1f0;

    std::vector<uint32_t> W(80);
    //for loop for parsing 512bit(64byte) blocks
    for (unsigned int block = 0; block * 64 < input.size(); block++){
        //for loop for parsing 16 32bit(4byte) words
        for(unsigned int word = 0; word < 16; word++){
            //for loop for extracting each of the 32bits(4bytes) from the input vector into the block
            for (unsigned int letter = 0; letter < 4 ; letter++){
                W[word] = (W[word] << 8) + input[block * 64 + word * 4 + letter];
            }

        }

        //fill schedule
        for(unsigned int i = 16; i < 80; i++){
            W[i] = ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1);
        }

        //set initial worker vars
        uint32_t a = H0;
        uint32_t b = H1;
        uint32_t c = H2;
        uint32_t d = H3;
        uint32_t e = H4;

        //do the hash, do the monster hash
        uint32_t T = 0;
        for (unsigned int t = 0; t < 80; t++){
            if (t < 20){
                T = ROTL(a, 5) + (((b & c) ^ ((~b) & d))) + e + 0x5a827999 + W[t];
                e = d;
                d = c;
                c = ROTL(b, 30);
                b = a;
                a = T;
            }
            else if (t < 40){
                T = ROTL(a, 5) + (b ^ c ^ d) + e + 0x6ed9eba1 + W[t];
                e = d;
                d = c;
                c = ROTL(b, 30);
                b = a;
                a = T;
            }
            else if (t < 60){
                T = ROTL(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + 0x8f1bbcdc + W[t];
                e = d;
                d = c;
                c = ROTL(b, 30);
                b = a;
                a = T;
            }
            else if (t < 80){
                T = ROTL(a, 5) + (b ^ c ^ d) + e + 0xca62c1d6 + W[t];
                e = d;
                d = c;
                c = ROTL(b, 30);
                b = a;
                a = T;
            }
        }

        H0 = a + H0;
        H1 = b + H1;
        H2 = c + H2;
        H3 = d + H3;
        H4 = e + H4;

    }

    //digest the hash
    std::vector<unsigned char> digest(20);
    for (uint8_t i = 0; i < 4; i++){
        digest[i] = (H0 >> (24 - 8 * i)) & 0xFF;
        digest[i + 4] = (H1 >> (24 - 8 * i)) & 0xFF;
        digest[i + 8] = (H2 >> (24 - 8 * i)) & 0xFF;
        digest[i + 12] = (H3 >> (24 - 8 * i)) & 0xFF;
        digest[i + 16] = (H4 >> (24 - 8 * i)) & 0xFF;
    }

    return digest;
}

void wrapSha(std::string& in, unsigned int len){
    std::cout << in.length() << std::endl;
    std::vector<uint8_t> byteM(in.begin(), in.end());

    std::vector<uint8_t> input = padd(byteM);


    std::vector<unsigned char> hash = sha(input);
    for (unsigned int i = 0; i < hash.size(); i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hash[i]);
    }
    std::cout << std::endl;
}

int main(){

    std::string test0 = "";
    std::string test1 = "This is a test of SHA-1.";
    std::string test2 = "Kerckhoff’s principle is the foundation on which modern cryptography is built.";
    std::string test3 = "SHA-1 is no longer considered a secure hashing algorithm.";
    std::string test4 = "SHA-1 or SHA-2 should be used in place of SHA-1.";
    std::string test5 = "Never roll your own crypto!";

    //wrapSha(test1, 0);
    //wrapSha(test2, 0);
    //wrapSha(test3, 0);
    wrapSha(test4, 0);
    //wrapSha(test5, 0);

    return 0;
}