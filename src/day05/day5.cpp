#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <queue>
#include "md5.h"


auto part1(const std::string& in)
{
    int i = 0;
    std::string password;
    while(password.size() < 8){
        std::string s = in + std::to_string(i);
        std::string h = md5(s);
        if(h[0] == '0' && h[1] == '0' && h[2] == '0' && h[3] == '0' && h[4] == '0'){
            password += h[5];
        }
        i++;
    }

    return password;
}

auto part2(const std::string& in)
{
    int i = 0;
    int found = 0;
    std::string password(8, '_');
    while(found < 8){
        std::string s = in + std::to_string(i);
        std::string h = md5(s);
        if(h[0] == '0' && h[1] == '0' && h[2] == '0' && h[3] == '0' && h[4] == '0'){
            if(isdigit(h[5]) && (h[5]-'0') < 8 && password[h[5]-'0'] == '_'){
                found++;
                password[h[5]-'0'] = h[6];
                //std::cout << h[5] << ", " << h[6] << ", " << password << std::endl;
            }
        }
        i++;
    }

    return password;
}

void main()
{
    std::cout << "part1: " << part1("abc") << std::endl;
    std::cout << "part1: " << part1("ffykfhsq") << std::endl;

    std::cout << "part2: " << part2("abc") << std::endl;
    std::cout << "part2: " << part2("ffykfhsq") << std::endl;
}
