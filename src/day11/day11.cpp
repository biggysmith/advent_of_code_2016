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
#include <bitset>
#include <intrin.h>
#include "timer.hpp"

using floor_t = std::set<std::string>;

struct state_t{
    std::vector<floor_t> floors;
    int elevator = 0;
    int dist = 0;
};

struct bit_state_t{
    uint64_t bits = 0;
    int elevator = 0;
    int max_size;
    int dist = 0;

    void insert(int floor, int item){
        bits |= (1ULL << (floor*16+item)); 
    }

    void erase(int floor, int item){
        bits &= ~(1ULL << (floor*16+item)); 
    }

    void set_level(int level){
        elevator = level;
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

    void print() const{
        std::cout << std::endl;
        int f = 4;
        for(int f=3; f>=0; --f){
            std::cout << "F" << f << "  ";
            for(int j=0; j<max_size; ++j){
                if(floor_has(f, j)){
                    std::cout << (char)((j/2)+'a') << (j%2 ? 'm' : 'g');
                }else{
                    std::cout << " .";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};



bit_state_t load_input(const std::string& file){
    bit_state_t ret;
    std::ifstream fs(file);
    std::string line;
    char label;
    char c = 'a';
    std::map<std::string,char> names;
    int f = 0;

    int g = 0;
    int m = 1;
    while(std::getline(fs, line)){
        //ret.floors.push_back(floor_t());
        std::stringstream ss(line);
        std::string str, last_str;
        while(ss >> str){
            if(str.back() == '.'){
                str.pop_back();
            }
            if(str.back() == ','){
                str.pop_back();
            }
            if(str == "generator"){
                if(names.count(last_str)){
                    label = names[last_str];
                }else{
                    names[last_str] = c;
                    label = c;
                    c++;
                    //g+=2;
                }
                //ret.floors.back().insert(label+std::string("g"));
                ret.insert(f, g);
                g += 2;
            }else if(str == "microchip"){
                std::string remove("-compatible");
                size_t pos = last_str.find(remove);
                last_str.erase(pos, remove.size());
                if(names.count(last_str)){
                    label = names[last_str];
                }else{
                    names[last_str] = c;
                    label = c;
                    c++;
                    //m+=2;
                }
                //ret.floors.back().insert(label+std::string("m"));
                ret.insert(f, m);
                m += 2;
            }
            last_str = str;
        }
        f++;
    }
    ret.max_size = m-1;
    return ret;
}



struct bit_state_hash_t {
    size_t operator()(const bit_state_t& g) const{ 
        return std::hash<uint64_t>()(g.elevator) ^ std::hash<uint64_t>()(g.bits);
    }                                              
};

bool operator==(const bit_state_t& a,const bit_state_t& b){
    return a.elevator == b.elevator && a.bits == b.bits;
}

bool operator<(const bit_state_t& a,const bit_state_t& b){
    return a.dist > b.dist;
}

auto bfs2(const bit_state_t& src, const bit_state_t& dst)
{
    int num_paths = 0;
    std::queue<bit_state_t> q;

    //std::unordered_set<state_t,state_hash_t> visited;
    std::unordered_map<bit_state_t,int,bit_state_hash_t> dist;

    q.push(src);
    //visited.insert(src);

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        //curr.print();

        if (curr == dst){     
            num_paths++;
            std::cout << "winner winner" << std::endl;
            //print(curr);
            return dist[dst];
        }

        auto try_add_state = [&](int new_level){
            if(new_level >= 0 && new_level < 4){
                for(int i=0; i<curr.max_size; i++){
                    if(!curr.floor_has(curr.elevator,i)){
                        continue;
                    }
                    auto new_state = curr;
                    new_state.elevator = new_level;
                    new_state.erase(curr.elevator,i);
                    new_state.insert(new_state.elevator,i);
                    if(!dist.count(new_state)){
                        bool floora_safe = new_state.floor_safe(curr.elevator);
                        bool floorb_safe = new_state.floor_safe(new_state.elevator);
                        if(floora_safe && floorb_safe){
                            new_state.dist = dist[curr] + 1;
                            q.push(new_state);
                            dist[new_state] = dist[curr] + 1;
                        }
                    }

                    for(int j=0; j<curr.max_size; j++){
                        if(i != j){
                            if(!curr.floor_has(curr.elevator,j)){
                                continue;
                            }
                            auto new_state2 = new_state;
                            new_state2.elevator = new_level;
                            new_state2.erase(curr.elevator,j);
                            new_state2.insert(new_state2.elevator,j);
                            if(!dist.count(new_state2)){
                                bool floora_safe2 = new_state2.floor_safe(curr.elevator);
                                bool floorb_safe2 = new_state2.floor_safe(new_state2.elevator);
                                if(floora_safe2 && floorb_safe2){
                                    new_state2.dist = dist[curr] + 1;
                                    q.push(new_state2);
                                    dist[new_state2] = dist[curr] + 1;
                                }
                            }
                        }
                    }
                }
            }
        };

        try_add_state(curr.elevator+1);
        try_add_state(curr.elevator-1);
    }

    return 0;
}

state_t dst_state(const state_t& state){
    floor_t all_items;
    for(auto& floor : state.floors){
        all_items.insert(floor.begin(), floor.end());
    }
    state_t dst;
    dst.floors = { floor_t(), floor_t(), floor_t(), all_items };
    dst.elevator = 3;
    return dst;
}

bit_state_t dst_state(const bit_state_t& state){
    bit_state_t dst;
    for(int i=0; i<state.max_size; ++i){
        dst.insert(3, i);
    }
    dst.elevator = 3;
    dst.max_size = state.max_size;
    return dst;
}

auto part1(const bit_state_t& state)
{
    //return bfs(state, dst_state(state));
    return 0;
}



auto part2(const bit_state_t& state)
{
    scoped_timer t;


    /*auto to_bstate = [](const state_t& state){
        bit_state_t bstate;
        int f = 0;
        for(auto& floor : state.floors){
            for(auto& item : floor){
                bstate.insert(f, (item[0]-'a')*2 + (item[1]=='m' ? 1 : 0));
            }
            f++;
        }
        bstate.set_level(state.elevator);
        bstate.max_size = (int)__popcnt64(bstate.bits);
        return bstate;
    };

    auto state2 = state;

    state2.floors[0].insert("fg");
    state2.floors[0].insert("fm");
    state2.floors[0].insert("gg");
    state2.floors[0].insert("gm");
    
    auto src = to_bstate(state2);
    auto dst = to_bstate(dst_state(state2));*/

    auto src = state;

    for(int i=0; i<4; ++i){
        src.insert(0, i+src.max_size);
    }
    src.max_size += 4;
    auto dst = dst_state(src);



    src.print();
    dst.print();

    return bfs2(src, dst);
}

void main()
{
    auto test_values = load_input("../src/day11/example_input.txt");
    auto actual_values = load_input("../src/day11/input.txt");

    //std::cout << "part1: " << part1(test_values) << std::endl;
    //std::cout << "part1: " << part1(actual_values) << std::endl;

    //std::cout << "part2: " << part2(test_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;

    /*std::cout << "part2: " << part2(test_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;*/
}
