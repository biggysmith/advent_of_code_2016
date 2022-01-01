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
#include <intrin.h>

struct pos_t{
    int x,y;
};

pos_t operator+(const pos_t& a, const pos_t& b){
    return { a.x + b.x, a.y + b.y };
}

bool operator==(const pos_t& a, const pos_t& b){
    return std::make_tuple(a.x,a.y) == std::make_tuple(b.x,b.y);
}

bool operator<(const pos_t& a, const pos_t& b){
    return std::make_tuple(a.x,a.y) < std::make_tuple(b.x,b.y);
}

bool inside_building(const pos_t& p){
    return p.x >= 0 && p.y >= 0;
}

bool able_to_move(const pos_t& p,int fav){
    size_t value = p.x*p.x + 3*p.x + 2*p.x*p.y + p.y + p.y*p.y;
    value += fav;
    size_t num_bits = __popcnt64(value);
    return (num_bits % 2) == 0;
}

auto dijkstra(int fav_number, const pos_t& src, const pos_t& dst)
{
    std::queue<pos_t> q;

    std::set<pos_t> visited;
    std::map<pos_t,size_t> cost;

    cost[src] = 0;
    q.push(src);

    while (!q.empty()) {
        pos_t curr = q.front();
        q.pop();

        if(visited.count(curr)){
            continue;
        }

        visited.insert(curr);

        for(auto& d : std::vector<pos_t>{ {-1,0}, {1,0}, {0,-1}, {0,1} }){
            pos_t new_pos = curr + d;
            if(inside_building(new_pos) && able_to_move(new_pos,fav_number) && !visited.count(new_pos)){
                cost[new_pos] = cost[curr] + 1;
                q.push(new_pos);
            }
        }
    }

    size_t count = std::count_if(cost.begin(), cost.end(), [](auto& a){
        return a.second <= 50;
    });

    return std::make_pair(cost[dst], count);
}


auto process(int fav_number, const pos_t& dst)
{
    return dijkstra(fav_number, {1,1}, dst);
}

void main()
{

    auto p1 = process(10, {7,4});
    auto p2 = process(1350, {31,39});

    std::cout << "part1: " << p1.first << std::endl;
    std::cout << "part1: " << p2.first << std::endl;
    std::cout << "part2: " << p2.second << std::endl;
}
