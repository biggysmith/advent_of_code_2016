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

std::string md5(const std::string& s){
    md5_hash md5;
    md5.append((uint8_t*)s.data(),s.size());
    return md5.str();
}

std::string has_triple(const std::string& s){
    for(int i=0; i<s.size()-2; ++i){
        if(s[i] == s[i+1] && s[i+1] == s[i+2]){
            return std::string(1,s[i]);
        };
    }
    return "";
}

bool has_quintuple(const std::string& s,const std::string& c){
    for(int i=0; i<s.size()-4; ++i){
        if(c[0] == s[i] && s[i] == s[i+1] && s[i+1] == s[i+2] && s[i+2] == s[i+3] && s[i+3] == s[i+4]){
            return true;
        };
    }
    return false;
}

struct key_t{
    std::string triple;
    int tries = 0;
    int idx;
};

auto process(const std::string& in, int iters)
{
    int i = 0;
    std::vector<int> real_keys;
    std::vector<key_t> potential_keys;
    while(true){
        std::string s = in + std::to_string(i);
        std::string h = md5(s);
        for(int a=1; a<iters; ++a){
            h = md5(h);
        }
        std::string triple = has_triple(h);
        bool found_new = false;
        if(!triple.empty()){
            potential_keys.push_back({ triple, 0, i });
            found_new = true;
        }
        for(int k=0; k<potential_keys.size(); ){
            if(found_new && k==potential_keys.size()-1){
                k++;
                continue;
            }
            if(has_quintuple(h, potential_keys[k].triple)){
                real_keys.push_back(potential_keys[k].idx);
                if(real_keys.size()==64){
                    return real_keys.back();
                }
            }
            potential_keys[k].tries++;
            if(potential_keys[k].tries == 1000){
                potential_keys.erase(potential_keys.begin()+k);
            }else{
                k++;
            }
        }
        i++;
    }
    return -1;
}

void main()
{
    std::cout << "part1: " << process("abc",1) << std::endl;
    std::cout << "part1: " << process("ahsbgdzn",1) << std::endl;

    std::cout << "part2: " << process("abc",2017) << std::endl;
    std::cout << "part2: " << process("ahsbgdzn",2017) << std::endl;
}
