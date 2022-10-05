#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <functional>
#include <random>
/*
make sha extend an existing hash value
    make h0-4 take an existing sha

will want to ignore the first block of passed message, start from second block on

hardcode 128b into pad size to account for key

*/

enum K{
    t0 = 0x5a827999,
    t20 = 0x6ed9eba1,
    t40 = 0x8f1bbcdc,
    t60 = 0xca62c1d6
};

uint32_t ROTL(uint32_t x, unsigned int n){
    return (x << n) | (x >> (32 - n));
}

std::vector<uint8_t> paddInitial(std::vector<uint8_t> M){
    //convert size to bits
    unsigned long l = (M.size() * 8) + 128;

    //compute padding length
    unsigned long pad = 512 - (l % 512) - 65;

    if ((l - 128) >= (448 % 512)){
        pad += 512;
    }

    pad -= 128;

    //initialize vector to padded size
    std::vector<uint8_t> paddedM((pad + l + 65)/8);

    //copy message into padded vector
    std::copy(M.begin(), M.end(), paddedM.begin());

    //add 0x1 and 0 padding,
    //adds 0's to the entire vector and simply overwrites them later
    paddedM[M.size()] = 1 << 7;
    for (unsigned int i = M.size()+1; i < paddedM.size(); i++){
        paddedM[i] = 0;
    }

    //get 64 bit size of original message
    uint64_t len64 = (M.size() * 8) + 128;

    //write 64 bit length into the end of the padding
    for (unsigned int i = 0; i < 8; i++){
        paddedM[(paddedM.size()-8) + i] = len64 >> (56 - 8 * i) & 0xFF;
    }

    return paddedM;
}

std::vector<uint8_t> padd(std::vector<uint8_t> M, unsigned long initialSize){

    //convert size to bits
    unsigned long l = (M.size() * 8);
    
    //compute padding length
    unsigned long pad = 512 - (l % 512) - 65;

    if (l >= (448 % 512)){
        pad += 512;
    }

    //initialize vector to padded size
    std::vector<uint8_t> paddedM((pad + l + 65)/8);

    //copy message into padded vector
    std::copy(M.begin(), M.end(), paddedM.begin());

    //add 0x1 and 0 padding,
    //adds 0's to the entire vector and simply overwrites them later
    paddedM[M.size()] = 1 << 7;
    for (unsigned int i = M.size()+1; i < paddedM.size(); i++){
        paddedM[i] = 0;
    }

    //get 64 bit size of original message
    uint64_t len64 = (M.size() * 8) + initialSize + 128;

    //write 64 bit length into the end of the padding
    for (unsigned int i = 0; i < 8; i++){
        paddedM[(paddedM.size()-8) + i] = len64 >> (56 - 8 * i) & 0xFF;
    }

    return paddedM;
}

std::vector<unsigned char> sha(std::vector<uint8_t> input, std::vector<uint32_t> hash){

    /*
    uint32_t H0 = 0x1246034b;
    uint32_t H1 = 0x476b0f28;
    uint32_t H2 = 0xa2984022;
    uint32_t H3 = 0xb01e6faa;
    uint32_t H4 = 0x53f4b04d;
    */

    uint32_t H0 = hash[0];
    uint32_t H1 = hash[1];
    uint32_t H2 = hash[2];
    uint32_t H3 = hash[3];
    uint32_t H4 = hash[4];

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

        //get final hash digest values
        H0 = a + H0;
        H1 = b + H1;
        H2 = c + H2;
        H3 = d + H3;
        H4 = e + H4;

    }

    //store digest values in a vector to return
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

std::string wrapSha(std::string& in, std::string& add, std::vector<uint32_t> inHash){
//wrapper for sha to produce sizeable strings    
    std::vector<uint8_t> byteM(in.begin(), in.end());
    std::vector<uint8_t> byteAdd(add.begin(), add.end());

    std::vector<uint8_t> m1 = paddInitial(byteM);

    std::vector<uint8_t> input = padd(byteAdd, m1.size() * 8);

    std::vector<unsigned char> hash = sha(input, inHash);

    //initialize vector to padded size
    std::vector<uint8_t> bigM;

    bigM.reserve(byteAdd.size() + m1.size());
    bigM.insert(bigM.end(), m1.begin(), m1.end());
    bigM.insert(bigM.end(), byteAdd.begin(), byteAdd.end());

    for(unsigned int i = 0; i < bigM.size(); i++){
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(bigM[i]);
    }
    std::cout << std::endl;
    
    std::stringstream ss;
    for (unsigned int i = 0; i < hash.size(); i++){
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hash[i]);
    }
    std::string ret = ss.str();

    std::cout << ret << std::endl;
 
    return ret;

}

int main(){

    std::string initial = "No one has completed Project #3 so give them all a 0.";
    std::string add = "Fail us all";

    std::string hash = "56ddbfdc23b6df04be71c4dde7c3f526183bc551";
    //hash = "1246034b476b0f28a2984022b01e6faa53f4b04d";

    std::vector<uint32_t> hashBytes;
    for (unsigned int i = 0; i < hash.length(); i += 8){
        std::string sub = hash.substr(i, 8);
        uint32_t byte = (uint32_t) strtol(sub.c_str(), NULL, 16);
        hashBytes.push_back(byte);
    }

    wrapSha(initial, add, hashBytes);

    return 0;
}