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
#include <omp.h>

struct hash_t{
    int pos;
    char c;
};

auto part1(const std::string& in)
{
    std::string password;
    std::vector<std::vector<hash_t>> per_thread(omp_get_max_threads(), std::vector<hash_t>());

    #pragma omp parallel for 
    for(int i=0; i<9000000; ++i){
        std::string s = in + std::to_string(i);
        md5_hash md5;
        md5.append((uint8_t*)s.data(),s.size());
        std::string h = md5.str();
        if(h[0] == '0' && h[1] == '0' && h[2] == '0' && h[3] == '0' && h[4] == '0'){
            per_thread[omp_get_thread_num()].push_back({i, h[5]});
        }
    }

    std::vector<hash_t> all_hashes;
    for(auto& hashes : per_thread){
        for(auto& hash : hashes){
            all_hashes.push_back(hash);
        }
    }

    std::sort(all_hashes.begin(), all_hashes.end(), [](auto& a,auto& b){
        return a.pos < b.pos;
    });

    for(auto& hash : all_hashes){
        password += hash.c;
        if(password.size() == 8){
            return password;
        }
    }

    return password;
}


struct hash2_t{
    int pos;
    int idx;
    char c;
};

auto part2(const std::string& in)
{
    std::string password(8, '_');
    std::vector<std::vector<hash2_t>> per_thread(omp_get_max_threads(), std::vector<hash2_t>());

    #pragma omp parallel for 
    for(int i=0; i<28000000; ++i){
        std::string s = in + std::to_string(i);
        md5_hash md5;
        md5.append((uint8_t*)s.data(),s.size());
        std::string h = md5.str();
        if(h[0] == '0' && h[1] == '0' && h[2] == '0' && h[3] == '0' && h[4] == '0'){
            if(isdigit(h[5]) && (h[5]-'0') < 8){
                per_thread[omp_get_thread_num()].push_back({i, h[5]-'0', h[6]});
            }
        }
    }

    std::vector<hash2_t> all_hashes;
    for(auto& hashes : per_thread){
        for(auto& hash : hashes){
            all_hashes.push_back(hash);
        }
    }

    std::sort(all_hashes.begin(), all_hashes.end(), [](auto& a,auto& b){
        return a.pos < b.pos;
    });

    int found = 0;
    for(auto& hash : all_hashes){
        if(password[hash.idx] == '_'){
            password[hash.idx] = hash.c;
            found++;
        }
        if(found == 8){
            return password;
        }
    }

    return "fail: " + password;
}

void main()
{
    std::cout << "part1: " << part1("abc") << std::endl;
    std::cout << "part1: " << part1("ffykfhsq") << std::endl;

    std::cout << "part2: " << part2("abc") << std::endl;
    std::cout << "part2: " << part2("ffykfhsq") << std::endl;
}
