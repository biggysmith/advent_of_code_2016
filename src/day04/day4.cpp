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

struct room_t{
    std::string name;
    int sector_id;
    std::string checksum;
};

std::vector<room_t> load_input(const std::string& file){
    std::vector<room_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        room_t room;
        auto last_dash = line.find_last_of('-');
        auto first_bracket = line.find_first_of('[');
        auto last_bracket = line.find_first_of(']');
        room.name = line.substr(0, last_dash);
        room.sector_id = std::stoi(line.substr(last_dash+1, first_bracket-last_dash));
        room.checksum = line.substr(first_bracket+1, last_bracket-first_bracket-1);
        ret.push_back(room);
    }   

    return ret;
}

struct hist_t{
    char letter;
    int freq;
};

bool is_real_room(const room_t& room){
    std::vector<hist_t> alpha(26);
    for(int i=0; i<alpha.size(); ++i){
        alpha[i].letter = i+'a';
    }

    for(auto c : room.name){
        if(isalpha(c)){
            alpha[c-'a'].freq++;
        }
    }

    std::partial_sort(alpha.begin(), alpha.begin()+5, alpha.end(), [](auto& a,auto& b){
        return std::make_tuple(a.freq,-a.letter) > std::make_tuple(b.freq,-b.letter);
    });

    return std::equal(alpha.begin(), alpha.begin()+5, room.checksum.begin(), [](auto& a,auto& b){
        return a.letter == b;
    });
}

auto part1(const std::vector<room_t>& rooms)
{
    return std::accumulate(rooms.begin(), rooms.end(), 0, [](auto& a,auto& b){
        if(is_real_room(b)){
            a += b.sector_id;
        }
        return a;
    });
}

std::string decrypt(const room_t& room){
    std::string name = room.name;
    for(auto& c : name){
        if(c == '-'){
            c = ' ';
        }else{
            c = (((c-'a') + room.sector_id) % 26) + 'a';
        }
    }
    return name;
}

auto part2(const std::vector<room_t>& rooms)
{
    for(auto& room : rooms){
        if(is_real_room(room)){
            std::string name = decrypt(room);
            if(name.find("north") != std::string::npos){
                return room.sector_id;
            }
        }
    }
    return 0;
}

void main()
{
    auto actual_values = load_input("../src/day04/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
