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

struct state_hash_t {
    size_t operator()(const state_t& g) const{ 
        size_t h = g.elevator+1;
        for(auto& floor : g.floors){
            for(auto& item : floor){
                h ^= item[0] + 0x9e3779b9 + (h << 6) + (h >> 2);
                h ^= item[1] + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
        }
        return h;
    }                                              
};

bool operator==(const state_t& a,const state_t& b){
    return std::make_tuple(a.elevator,a.floors) == std::make_tuple(b.elevator,b.floors);
    //return a.floors == b.floors;
}

bool operator<(const state_t& a,const state_t& b){
    return a.dist > b.dist;
    //return a.floors == b.floors;
}

state_t load_input(const std::string& file){
    state_t ret;
    std::ifstream fs(file);
    std::string line;
    char label;
    char c = 'a';
    std::map<std::string,char> names;
    while(std::getline(fs, line)){
        ret.floors.push_back(floor_t());
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
                }
                ret.floors.back().insert(label+std::string("g"));
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
                }
                ret.floors.back().insert(label+std::string("m"));
            }
            last_str = str;
        }
    }
    return ret;
}



struct bit_state_t{
    uint64_t bits = 0;
    int elevator = 0; // todo plus in bits
    int max_size;
    int dist = 0;

    void insert(int floor, int item){
        //state.set(floor*16+item);
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
        //int s = floor*16;
        /*bool any_microchips = false;
        for(int i=1; i<max_size; i+=2){ 
            if(floor_has(floor,i)){ // we have microchip, return false
                return false;
            }

            //any_microchips |= floor_has(floor,i);
        }*/

        uint64_t microchip_mask = 0xAAAAull << (floor*16);
        return (bits & microchip_mask) == 0;
        //return !any_microchips;
    }

    bool all_microchips(int floor){
        //int s = floor*16;
        /*bool any_gens = false;
        for(int i=0; i<max_size; i+=2){ 
            if(floor_has(floor,i)){ // we have generator, return false
                return false;
            }

            //any_gens |= floor_has(floor,i);
        }
        //return !any_gens;
        return true;*/

        uint64_t generator_mask = 0x5555ull << (floor*16);
        return (bits & generator_mask) == 0;
    }

    bool all_empty(int floor){
        //int s = floor*16;
        /*for(int i=0; i<max_size; i+=1){
            if(floor_has(floor,i)){
                return false;
            }
        }
        return true;*/

        uint64_t empty_mask = 0xFFFFull << (floor*16);
        return (bits & empty_mask) == 0;
    }

    bool floor_safe(int floor){
        bool all_e = all_empty(floor);
        bool all_g = all_generators(floor);
        bool all_m = all_microchips(floor);
        if(all_empty(floor) || all_generators(floor) || all_microchips(floor)){
            return true;
        }

        bool safe = true;
        //for(auto& item : items){
            /*if(is_microchip(item)){
                // needs corresponding generator
                std::string gen = std::string(1,item[0]) + 'g';
                safe &= std::find(items.begin(),items.end(),gen) != items.end();
            }*/
            for(int i=1; i<max_size; i+=2){
                if(floor_has(floor,i)){
                    safe &= floor_has(floor,i-1);
                }
            }
       // }

        if(safe){
            return true;
        }

        return false;
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

struct bit_state_hash_t {
    size_t operator()(const bit_state_t& g) const{ 
        return std::hash<uint64_t>()(g.elevator) ^ std::hash<uint64_t>()(g.bits);
    }                                              
};

bool operator==(const bit_state_t& a,const bit_state_t& b){
    return a.elevator == b.elevator && a.bits == b.bits;
    //return std::make_tuple(a.elevator,a.bits) == std::make_tuple(b.elevator,b.bits);
    //return a.floors == b.floors;
}

bool operator<(const bit_state_t& a,const bit_state_t& b){
    return a.dist > b.dist;
    //return a.floors == b.floors;
}

bool is_generator(int item){
    return (item % 2) == 0;
}

bool is_microchip(int item){
    return (item % 2) == 1;
}

/*
bool can_move_to_floor(int floor, int item){
    if(is_generator(item)){
        return true;
    }

    // must be microchip

    bool no_generators = std::none_of(items.begin(), items.end(), [&](auto& i){
        return i[1] == 'g';
    });

    if(no_generators){
        return true;
    }

    bool has_safe_generator = std::any_of(items.begin(), items.end(), [&](auto& i){
        return i[1] == 'g' && i[0] == item[0];
    });

    if(has_safe_generator){
        return true;
    }

    return false;
}*/


#if 0

bool all_generators(const floor_t& items){
    return std::all_of(items.begin(), items.end(), [](auto& i){
        return is_generator(i);
    });
}

bool all_microchips(const floor_t& items){
    return std::all_of(items.begin(), items.end(), [](auto& i){
        return is_microchip(i);
    });
}

bool all_empty(const floor_t& items){
    return items.empty();
}

bool floor_safe(const floor_t& items){
    for(auto& item : items){
        //std::cout << item << " ";
    }

    if(all_empty(items) || all_generators(items) || all_microchips(items)){
        //std::cout << "true" << std::endl;
        return true;
    }

    bool safe = true;
    for(auto& item : items){
        if(is_microchip(item)){
            // needs corresponding generator
            std::string gen = std::string(1,item[0]) + 'g';
            safe &= std::find(items.begin(),items.end(),gen) != items.end();
        }
    }

    if(safe){
        //std::cout << "true" << std::endl;
        return true;
    }

    //std::cout << "false" << std::endl;
    return false;
}

void print(const state_t& src){
    std::cout << std::endl;
    int f = 4;
    for(auto fit=src.floors.rbegin(); fit!=src.floors.rend(); ++fit){
        std::cout << "F" << f << "  ";
        auto it = fit->begin();
        for(int j=0; j<8; ++j){
            if(it != fit->end()){
                std::cout << *it << " ";
                it++;
            }else{
                std::cout << '.' << "  ";
            }
        }
        f--;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

auto bfs(const state_t& src, const state_t& dst)
{
    int num_paths = 0;
    std::priority_queue<state_t> q;

    //std::unordered_set<state_t,state_hash_t> visited;
    std::unordered_map<state_t,size_t,state_hash_t> dist;

    q.push(src);
    //visited.insert(src);

    while (!q.empty()) {
        auto curr = q.top();
        q.pop();

        //print(curr);

        if (curr == dst){     
            num_paths++;
            std::cout << "winner winner" << std::endl;
            //print(curr);
            return dist[dst];
        }

        /*if(dist.count(curr)){
            continue;
        }*/

        auto try_add_state = [&](int new_level){
            if(new_level >= 0 && new_level < 4){
                for(auto& item : curr.floors[curr.elevator]){
                    auto new_state = curr;
                    new_state.elevator = new_level;
                    new_state.floors[curr.elevator].erase(item);
                    new_state.floors[new_state.elevator].insert(item);
                    bool floora_safe = floor_safe(new_state.floors[curr.elevator]);
                    bool floorb_safe = floor_safe(new_state.floors[new_state.elevator]);
                    if(!dist.count(new_state) && floora_safe && floorb_safe){
                        int score = 0;
                        int i = 1;
                        for(auto& floor : new_state.floors){
                            score += floor.size() * i++;
                        }
                        new_state.dist = dist[curr] + 1;
                        q.push(new_state);

                        //visited.insert(new_state);
                        dist[new_state] = dist[curr] + 1;
                        //print(new_state);
                        /*if (new_state == dst){     
                            num_paths++;
                            return dist[dst];
                        }*/
                    }

                    for(auto& item2 : curr.floors[curr.elevator]){
                        if(item != item2){
                            auto new_state2 = new_state;
                            new_state2.elevator = new_level;
                            new_state2.floors[curr.elevator].erase(item2);
                            new_state2.floors[new_state2.elevator].insert(item2);
                            bool floora_safe2 = floor_safe(new_state2.floors[curr.elevator]);
                            bool floorb_safe2 = floor_safe(new_state2.floors[new_state2.elevator]);
                            if(!dist.count(new_state2) && floora_safe2 && floorb_safe2){
                                int score = 0;
                                int i = 1;
                                for(auto& floor : new_state2.floors){
                                    score += floor.size() * i++;
                                }
                                new_state2.dist = dist[curr] + 1;
                                q.push(new_state2);
                                //visited.insert(new_state2);
                                //print(new_state2);
                                dist[new_state2] = dist[curr] + 1;
                                /*if (new_state2 == dst){     
                                    num_paths++;
                                    std::cout << dist.count(dst) << std::endl;
                                    std::cout << dist.count(new_state2) << std::endl;
                                    return dist[new_state2];
                                }*/
                            }
                        }
                    }
                }
            }
        };

        try_add_state(curr.elevator+1);
        try_add_state(curr.elevator-1);
    }

    return 0ull;
}
#endif


auto bfs2(const bit_state_t& src, const bit_state_t& dst)
{
    int num_paths = 0;
    std::priority_queue<bit_state_t> q;

    //std::unordered_set<state_t,state_hash_t> visited;
    std::unordered_map<bit_state_t,size_t,bit_state_hash_t> dist;

    q.push(src);
    //visited.insert(src);

    while (!q.empty()) {
        auto curr = q.top();
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

    return 0ull;
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

auto part1(const state_t& state)
{
    //return bfs(state, dst_state(state));
    return 0;
}



auto part2(const state_t& state)
{
    scoped_timer t;


    auto to_bstate = [](const state_t& state){
        bit_state_t bstate;
        int f = 0;
        for(auto& floor : state.floors){
            for(auto& item : floor){
                bstate.insert(f, (item[0]-'a')*2 + (item[1]=='m' ? 1 : 0));
            }
            f++;
        }
        bstate.set_level(state.elevator);
        bstate.max_size = __popcnt64(bstate.bits);
        return bstate;
    };

    auto state2 = state;

    state2.floors[0].insert("fg");
    state2.floors[0].insert("fm");
    state2.floors[0].insert("gg");
    state2.floors[0].insert("gm");
    
    auto src = to_bstate(state2);
    auto dst = to_bstate(dst_state(state2));

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
