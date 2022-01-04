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

struct pos_t{
    int x,y;
};

bool operator<(const pos_t& a,const pos_t& b){
    return std::make_tuple(a.x,a.y) < std::make_tuple(b.x,b.y);
}

bool operator==(const pos_t& a,const pos_t& b){
    return std::make_tuple(a.x,a.y) == std::make_tuple(b.x,b.y);
}

pos_t operator+(const pos_t& a,const pos_t& b){
    return { a.x + b.x, a.y + b.y };
}

struct grid_t{
    std::vector<char> points;
    int width, height;

    char& get(const pos_t& p){ return points[p.y*width+p.x]; }
    const char& get(const pos_t& p)const { return points[p.y*width+p.x]; }
};

struct node_t{
    pos_t pos;
    int dist = 0;
};

bool operator<(const node_t& a,const node_t& b){
    return a.dist > b.dist;
}

grid_t load_input(const std::string& file){
    grid_t ret;
    std::ifstream fs(file);
    std::string line;
    ret.width = ret.height = 0;
    while(std::getline(fs, line)){
        ret.points.insert(ret.points.end(), line.begin(), line.end());
        ret.height++;
        ret.width = std::max(ret.width, (int)line.size());
    }   
    return ret;
}

auto find_shortest_path(const grid_t& grid, const pos_t& src, const pos_t& dst)
{
    std::priority_queue<node_t> q;
    q.push({src,0});

    std::set<pos_t> visited;

    std::map<pos_t,int> dist;
    dist[src] = 0;

    while (!q.empty()) {
        auto curr = q.top();
        q.pop();

        if(visited.count(curr.pos)){
            continue;
        }

        visited.insert(curr.pos);

        for(auto& dir : std::vector<pos_t>{{0,-1}, {1,0}, {0,1}, {-1,0}}){
            pos_t new_pos = curr.pos + dir;
            if(grid.get(new_pos) != '#'){ 
                node_t node = { new_pos, curr.dist+1 };
                q.push(node);

                if(new_pos == dst){
                    return node.dist;
                }
            }
        }
    }

    return -1;
}

struct edge_t{
    int a,b,dist;
};

using graph_t = std::vector<std::vector<edge_t>>;

void minimum_distance(const graph_t& graph, int v, int current_dist, int& dist_part1, int& dist_part2, std::set<int>& path, const std::set<int>& dst)
{
    if(path == dst){
        dist_part1 = std::min(dist_part1, current_dist);
        dist_part2 = std::min(dist_part2, current_dist + graph[v][0].dist);
    }else{
        for(auto& edge : graph[v]){
            if(!path.count(edge.b)){
                path.insert(edge.b);
                minimum_distance(graph, edge.b, current_dist + edge.dist, dist_part1, dist_part2, path, dst);
                path.erase(edge.b);
            }
        }
    }
}

auto process(const grid_t& grid)
{
    std::map<int,pos_t> locations;
    std::set<int> locations_set;
    for(int y=0; y<grid.height; ++y){
        for(int x=0; x<grid.width; ++x){
            if(grid.get({x,y}) != '.' && grid.get({x,y}) != '#'){
                locations[grid.get({x,y})-'0'] = {x,y};
                locations_set.insert(grid.get({x,y})-'0');
            }
        }
    }

    graph_t graph(locations.size());

    for(int i=0; i<locations.size(); ++i){
        for(int j=i+1; j<locations.size(); ++j){
            int dist = find_shortest_path(grid, locations[i], locations[j]);
            graph[i].push_back({i,j,dist});
            graph[j].push_back({j,i,dist});
        }
    }

    int dist_part1 = INT_MAX;
    int dist_part2 = INT_MAX;
    std::set<int> path{0};
    minimum_distance(graph, 0, 0, dist_part1, dist_part2, path, locations_set);

    return std::make_pair(dist_part1, dist_part2);
}

void main()
{
    auto test_values = load_input("../src/day24/example_input.txt");
    auto actual_values = load_input("../src/day24/input.txt");

    auto test_results = process(test_values);
    auto results = process(actual_values);

    std::cout << "part1: " << test_results.first << std::endl;
    std::cout << "part1: " << results.first << std::endl;

    std::cout << "part2: " << test_results.second << std::endl;
    std::cout << "part2: " << results.second << std::endl;
}
