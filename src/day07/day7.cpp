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

struct sequence_t{
    std::string str;
    bool in_brackets;
};

struct ip_t{
    std::vector<sequence_t> sequences;
};

std::vector<ip_t> load_input(const std::string& file){
    std::vector<ip_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        ip_t ip {{ {std::string(), 0} }};
        for(auto c : line){
            if(c == '[' || c == ']'){
                ip.sequences.push_back({std::string(), c == '['});
                continue;
            }
            ip.sequences.back().str += c;
        }
        ret.push_back(ip);
    }   

    return ret;
}

bool has_abba(const std::string& s){
    for (int i=0; i<s.size()-3; ++i) {
        if(s[i] == s[i+3] && s[i+1] == s[i+2] && s[i] != s[i+1] && s[i+2] != s[i+3]){
            return true;
        }
    }
    return false;
}

bool supports_tls(const ip_t& ip){
    bool ret = false;
    for(auto& seq : ip.sequences){
        bool abba_present = has_abba(seq.str);
        if(abba_present && seq.in_brackets){
            return false;
        }
        if(abba_present && !seq.in_brackets){
            ret = true;
        }
    }
    return ret;
}

auto part1(const std::vector<ip_t>& ips)
{
    return std::count_if(ips.begin(), ips.end(), supports_tls);
}


bool supports_ssl(const ip_t& ip)
{
    std::map<std::string,int> aba;
    std::map<std::string,int> bab;
    for(auto& seq : ip.sequences){
        for (int i=0; i<seq.str.size()-2; ++i) {
            if(seq.str[i] == seq.str[i+2] && seq.str[i] != seq.str[i+1]){
                if(seq.in_brackets){
                    bab[std::string(1,seq.str[i+1]) + seq.str[i] + seq.str[i+1]]++;
                }else{
                    aba[std::string(1,seq.str[i]) + seq.str[i+1] + seq.str[i+2]]++;
                }
            }
        }
    }

    if(aba.empty() || bab.empty()){
        return false;
    }

    if(bab.size() > aba.size()){
        return std::any_of(aba.begin(), aba.end(), [&](auto& a){
            return bab[a.first] > 0;
        });
    }else{
        return std::any_of(bab.begin(), bab.end(), [&](auto& a){
            return aba[a.first] > 0;
        });
    }
}

auto part2(const std::vector<ip_t>& ips)
{
    return std::count_if(ips.begin(), ips.end(), supports_ssl); 
}

void main()
{
    auto actual_values = load_input("../src/day07/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
