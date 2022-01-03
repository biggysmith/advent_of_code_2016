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
#include <optional>

struct pos_t{
    int x,y;
};

bool operator<(const pos_t& a,const pos_t& b){
    return std::make_tuple(a.x,a.y) < std::make_tuple(b.x,b.y);
}

bool operator!=(const pos_t& a,const pos_t& b){
    return std::make_tuple(a.x,a.y) != std::make_tuple(b.x,b.y);
}

bool operator==(const pos_t& a,const pos_t& b){
    return std::make_tuple(a.x,a.y) == std::make_tuple(b.x,b.y);
}

pos_t operator+(const pos_t& a,const pos_t& b){
    return { a.x + b.x, a.y + b.y };
}

struct node_t{
    int size;
    int used;
    int avail;
    int use;
};

std::map<pos_t,node_t> load_input(const std::string& file){
    std::map<pos_t,node_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        if(line[0] != '/'){
            continue;
        }
        pos_t pos;
        node_t node;
        sscanf(line.c_str(),"/dev/grid/node-x%d-y%d     %dT   %dT    %dT   %d", &pos.x, &pos.y, &node.size, &node.used, &node.avail, &node.use);
        ret[pos] = node; 
    }   
    return ret;
}

auto part1(const std::map<pos_t,node_t>& nodes)
{
    int count = 0;
    for(auto& [pos0,node0] : nodes){
        for(auto& [pos1,node1] : nodes){
            if(pos0 != pos1){
                if(node0.used != 0 && node0.used <= node1.avail){
                    count++;
                }
            }
        }
    }
    return count;
}

bool valid_pos(const pos_t& pos,const pos_t& max_pos){
    return pos.x >= 0 && pos.y >=0 && pos.x <= max_pos.x && pos.y <= max_pos.y;
}

auto bfs(const std::map<pos_t,node_t>& nodes, const pos_t& src, const pos_t& dst, const pos_t max_pos, std::optional<pos_t> skip=std::nullopt)
{
    std::queue<pos_t> q;
    q.push(src);

    std::map<pos_t,int> dist;
    std::map<pos_t,pos_t> prev;

    dist[src] = 0;

    while (!q.empty()) {
        auto curr = q.front();
        q.pop();

        for(auto& dir : std::vector<pos_t>{{0,-1}, {1,0}, {0,1}, {-1,0}}){
            pos_t new_pos = curr + dir;
            if(valid_pos(new_pos, max_pos) && nodes.at(new_pos).size<300){ // ignore the mega nodes
                if(skip){
                    if(new_pos == *skip){
                        continue; // sometime we wanna travel back over dst
                    }
                }
                dist.try_emplace(new_pos, INT_MAX);
                if (dist[new_pos] > dist[curr] + 1){
                    dist[new_pos] = dist[curr] + 1;
                    prev[new_pos] = curr;
                    q.push(new_pos);
                }

                if(new_pos == dst){
                    std::vector<pos_t> path{new_pos};
                    while(prev.count(path.back())){
                        path.push_back(prev[path.back()]);
                    }
                    return std::vector<pos_t>(path.rbegin()+1,path.rend());
                }
            }
        }
    }

    return std::vector<pos_t>();
}

auto part2(const std::map<pos_t,node_t>& in)
{
    auto nodes = in;

    pos_t max_pos {0,0};
    for(auto& [pos,node] : nodes){
        max_pos.x = std::max(max_pos.x, pos.x);
        max_pos.y = std::max(max_pos.y, pos.y);
    }

    auto find_empty_node = [&]{
        for(int y=0; y<=max_pos.y; ++y){
            for(int x=0; x<=max_pos.x; ++x){
                if(nodes[{x,y}].use == 0){
                    return pos_t{x,y};
                }
            }    
        }
        return pos_t{-1,-1};
    };

    auto empty = find_empty_node();

    pos_t src {0,0};
    pos_t dst {max_pos.x,0};

    auto first_path = bfs(nodes, dst, src, max_pos); // top right to top left

    int node = 0;
    int steps = 0;
    while(dst != src)
    {
        // sliding puzzle, travel back and forth from empty node to path points until we hit last target
        auto next_path = bfs(nodes, empty, first_path[node++], max_pos, dst); 
        steps += (int)next_path.size() + 1;

        empty = dst;
        dst = next_path.back();
    }

    return steps;
}

void main()
{
    auto test_values = load_input("../src/day22/example_input.txt");
    auto actual_values = load_input("../src/day22/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
