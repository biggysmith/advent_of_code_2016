#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include "timer.hpp"

std::string dragonise(const std::string& a){
    std::string b = a;

    std::reverse(b.begin(),b.end());

    for(auto& c : b){
        if(c == '1'){
            c = '0';
        }else{
            c = '1';
        }
    }

    return a + "0" + b;
}

std::string checksum(const std::string& a){
    std::string b;

    for(int i=0; i<a.size(); i+=2){
        if(a[i] == a[i+1]){
            b += '1';
        }else{
            b += '0';
        }
    }

    return b;
}

auto process(const std::string& in, int len)
{
    std::string a = in;

    while(a.size() < len){
        a = dragonise(a);
    }

    a = std::string(a.begin(), a.begin()+len);
    std::string check = checksum(a);

    while((check.size() % 2) == 0){
        check = checksum(check);
    }

    return check;
}

void main()
{
    std::cout << "part1: " << process("10000", 20) << std::endl;
    std::cout << "part1: " << process("01110110101001000", 272) << std::endl;

    std::cout << "part2: " << process("01110110101001000", 35651584) << std::endl;
}
