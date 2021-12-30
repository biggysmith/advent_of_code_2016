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

struct vec2_t{
    int x, y;
};

vec2_t& operator+=(vec2_t& a,const vec2_t& b){ a.x += b.x; a.y += b.y; return a; }
vec2_t operator*(const vec2_t& a,int d){ return { a.x*d, a.y*d }; }
bool operator<(const vec2_t& a,const vec2_t& b){ return std::make_tuple(a.x,a.y) < std::make_tuple(b.x,b.y); }

int manhatten(const vec2_t& a){
    return std::abs(a.x) + std::abs(a.y);
}

struct instruction_t{
    char dir;
    int dist;
};

std::vector<instruction_t> load_input(const std::string& file){
    std::vector<instruction_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line, ',')){
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        ret.push_back({ line[0], std::stoi(line.substr(1)) });
    }   

    return ret;
}

int mod(int a,int b){
    return (b + (a%b)) % b;
}

auto part1(const std::vector<instruction_t>& instructions)
{
    vec2_t pos = { 0, 0 };
    std::vector<vec2_t> dirs { {0,-1}, {1,0}, {0,1}, {-1,0} };
    int d = 0;

    for(auto& ins : instructions){
        d = mod(d + ((ins.dir == 'R') ? 1 : -1), 4);
        pos += dirs[d] * ins.dist;
    }
    return manhatten(pos);
}

auto part2(const std::vector<instruction_t>& instructions)
{
    vec2_t pos = { 0, 0 };
    std::vector<vec2_t> dirs { {0,-1}, {1,0}, {0,1}, {-1,0} };
    int d = 0;

    std::set<vec2_t> visited { pos };
    for(auto& ins : instructions){
        d = mod(d + ((ins.dir == 'R') ? 1 : -1), 4);
        for(int i=0; i<ins.dist; ++i){
            pos += dirs[d];
            if(visited.count(pos)){
                return manhatten(pos);
            }
            visited.insert(pos);
        }
    }

    return manhatten(pos);
}

void main()
{
    auto actual_values = load_input("../src/day01/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
