#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include "md5.h"
#include "timer.hpp"
#include <queue>
#include <set>
#include <map>

struct pos_t{
    int x,y;
};

pos_t operator+(const pos_t& a, const pos_t& b){
    return { a.x + b.x, a.y + b.y };
}

bool operator==(const pos_t& a, const pos_t& b){
    return std::make_tuple(a.x,a.y) == std::make_tuple(b.x,b.y);
}

struct path_t{
    std::string passcode;
    pos_t pos;
    int length = 0;
};

bool operator<(const path_t& a, const path_t& b){
    return std::make_tuple(a.pos.x,a.pos.y) < std::make_tuple(b.pos.x,b.pos.y);
}

bool inside_grid(const pos_t& p){
    return p.x >= 0 && p.y >= 0 && p.x < 4 && p.y < 4;
}

std::string md5(const std::string& s){
    md5_hash md5;
    md5.append((uint8_t*)s.data(),s.size());
    return md5.str();
}

auto bfs(const path_t& src, const path_t& dst)
{
    std::queue<path_t> q;
    q.push(src);

    std::string shortest;
    int max_dist = 0;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        if(curr.pos == dst.pos){
            if(shortest.empty()){
                shortest = std::string(curr.passcode.begin()+src.passcode.size(), curr.passcode.end());
            }
            max_dist = std::max(curr.length,max_dist);
            continue;
        }

        std::string new_hash = md5(curr.passcode);

        auto valid = [](char c){
            return c>='b' && c<='f';  
        };

        if(valid(new_hash[0])){
            path_t new_path { curr.passcode + 'U', curr.pos + pos_t{0,-1}, curr.length+1 };
            if(inside_grid(new_path.pos)){
                q.push(new_path);
            }
        }

        if(valid(new_hash[1])){
            path_t new_path { curr.passcode + 'D', curr.pos + pos_t{0,1}, curr.length+1 };
            if(inside_grid(new_path.pos)){
                q.push(new_path);
            }
        }

        if(valid(new_hash[2])){
            path_t new_path { curr.passcode + 'L', curr.pos + pos_t{-1,0}, curr.length+1 };
            if(inside_grid(new_path.pos)){
                q.push(new_path);
            }
        }

        if(valid(new_hash[3])){
            path_t new_path { curr.passcode + 'R', curr.pos + pos_t{1,0}, curr.length+1 };
            if(inside_grid(new_path.pos)){
                q.push(new_path);
            }
        }
    }

    return std::make_pair(shortest, max_dist);
}

auto process(const std::string& pass)
{
    return bfs({pass,{0,0}}, {"",{3,3}});
}

void main()
{
    auto part = process("dmypynyp");

    std::cout << "part1: " << part.first << std::endl;
    std::cout << "part2: " << part.second << std::endl;
}
