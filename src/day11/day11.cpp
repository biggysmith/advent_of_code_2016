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
#include <unordered_set>
#include "timer.hpp"

struct state_t{
    uint64_t bits = 0;
    int elevator = 0;
    int max_size;

    void insert(int floor, int item){
        bits |= (1ULL << (floor*16+item)); 
    }

    void erase(int floor, int item){
        bits &= ~(1ULL << (floor*16+item)); 
    }

    bool floor_has(int floor,int item) const{
        return (bits & (1ULL << (floor*16+item))) != 0;
    }

    bool all_generators(int floor){
        uint64_t microchip_mask = 0xAAAAull << (floor*16);
        return (bits & microchip_mask) == 0;
    }

    bool all_microchips(int floor){
        uint64_t generator_mask = 0x5555ull << (floor*16);
        return (bits & generator_mask) == 0;
    }

    bool all_empty(int floor){
        uint64_t empty_mask = 0xFFFFull << (floor*16);
        return (bits & empty_mask) == 0;
    }

    bool floor_safe(int floor){
        if(all_empty(floor) || all_generators(floor) || all_microchips(floor)){
            return true;
        }

        for(int i=1; i<max_size; i+=2){
            if(floor_has(floor,i)){
                if(!floor_has(floor,i-1)){
                    return false;
                }
            }
        }

        return true;
    }
};

state_t load_input(const std::string& file){
    state_t ret;
    std::ifstream fs(file);
    std::string line;
    int f = 0;
    int g = 0;
    int m = 1;
    while(std::getline(fs, line)){
        std::stringstream ss(line);
        std::string str, last_str;
        while(ss >> str){
            if(str.back() == '.' || str.back() == ','){
                str.pop_back();
            }
            if(str == "generator"){
                ret.insert(f, g); g += 2;
            }else if(str == "microchip"){
                ret.insert(f, m); m += 2;
            }
        }
        f++;
    }
    ret.max_size = m-1;
    return ret;
}

struct state_hash_t {
    size_t operator()(const state_t& g) const{ 
        return std::hash<int>()(g.elevator) ^ std::hash<uint64_t>()(g.bits);
    }                                              
};

bool operator==(const state_t& a,const state_t& b){
    return a.elevator == b.elevator && a.bits == b.bits;
}

auto bfs(const state_t& src, const state_t& dst)
{
    std::queue<state_t> q;

    std::unordered_map<state_t,int,state_hash_t> dist;

    q.push(src);

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        if (curr == dst){  
            return dist[dst];
        }

        auto get_new_state = [&](const state_t& state,int new_level,int i){
            state_t new_state = state;
            new_state.elevator = new_level;
            new_state.erase(curr.elevator,i);
            new_state.insert(new_state.elevator,i);
            if(!dist.count(new_state) && new_state.floor_safe(curr.elevator) && new_state.floor_safe(new_state.elevator)){
                q.push(new_state);
                dist[new_state] = dist[curr] + 1;
            }
            return new_state;
        };

        auto add_new_states = [&](int new_level){
            if(new_level >= 0 && new_level < 4){
                for(int i=0; i<curr.max_size; i++){
                    if(!curr.floor_has(curr.elevator,i)){
                        continue;
                    }
                    auto new_state = get_new_state(curr, new_level, i);

                    for(int j=0; j<curr.max_size; j++){
                        if(i != j){
                            if(!curr.floor_has(curr.elevator,j)){
                                continue;
                            }
   
                            get_new_state(new_state, new_level, j);
                        }
                    }

                }
            }
        };

        add_new_states(curr.elevator+1);
        add_new_states(curr.elevator-1);
    }

    return dist[dst];
}

state_t dst_state(const state_t& state){
    state_t dst;
    for(int i=0; i<state.max_size; ++i){
        dst.insert(3, i);
    }
    dst.elevator = 3;
    dst.max_size = state.max_size;
    return dst;
}

auto part1(const state_t& src)
{
    return bfs(src, dst_state(src));
}

auto part2(const state_t& state)
{
    auto src = state;
    for(int i=0; i<4; ++i){
        src.insert(0, i+src.max_size);
    }
    src.max_size += 4;

    return bfs(src, dst_state(src));
}

void main()
{
    auto test_values = load_input("../src/day11/example_input.txt");
    auto actual_values = load_input("../src/day11/input.txt");

    std::cout << "part1: " << part1(test_values) << std::endl;
    std::cout << "part1: " << part1(actual_values) << std::endl;

    std::cout << "part2: " << part2(actual_values) << std::endl;
}