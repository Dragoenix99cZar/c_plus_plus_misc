#include <bits/stdc++.h>
#include "TinySHA1.hpp"
#include <iostream>
#include <string>


// OUT :: Calculated : ("a") = 86f7e437 faa5a7f

void testSHA1(const std::string& val) {
    sha1::SHA1 s;
   s.processBytes(val.c_str(), val.size());
//    uint32_t digest[5];
   uint32_t digest[2];
   s.getDigest(digest);	
//    char tmp[48];
   char tmp[8+1+8];
//    snprintf(tmp, 45, "%08x %08x %08x %08x %08x", digest[0], digest[1], digest[2], digest[3], digest[4]);
   snprintf(tmp, (8+1+8), "%08x %08x", digest[0], digest[1]);
   std::cout<<"Calculated : (\""<<val<<"\") = "<<tmp;
}

int main(int argc, char* argv[])
{
    std::cout << "You have entered " << argc
              << " arguments:" << std::endl;

    // testSHA1("The quick brown fox jumps over the lazy dog");

    // Using a while loop to iterate through arguments
    int i = 0;
    while (i < argc) {
        // std::cout << "Argument " << i << ": " << argv[i] << " ";
        testSHA1(argv[i]);
        std::cout << std::endl;
        i++;
    }
    // system(argv[1]);

    return 0;
}