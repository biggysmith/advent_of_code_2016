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
#include "timer.hpp"

template<typename T,typename It>
auto circular_next(T& list,It iter){
    if(iter == list.end()){
        return list.begin();
    }
    auto next = std::next(iter);
    if(next == list.end()){
        next = list.begin();
    }
    return next;
}

template<typename T,typename It>
auto circular_erase(T& list,It iter){
    auto next = list.erase(iter);
    if(next == list.end()){
        next = list.begin();
    }
    return next;
}

auto part1(int num_elfs)
{
    std::list<int> elfs;
    for(int i=0; i<num_elfs; ++i){
        elfs.push_back(i+1);
    }

    auto current = elfs.begin();

    while(elfs.size() > 1){
        auto it = circular_next(elfs,current);
        current = circular_erase(elfs,it);
    }

    return elfs.front();
}

auto part2(int num_elfs)
{
    std::list<int> elfs;
    for(int i=0; i<num_elfs; ++i){
        elfs.push_back(i+1);
    }

    auto mid = elfs.begin();
    std::advance(mid,elfs.size()/2);

    while(elfs.size() > 1){
        mid = circular_erase(elfs,mid);
        if((elfs.size() % 2) == 0){
            mid = circular_next(elfs,mid);
        }
    }

    return elfs.front();
}

void main()
{
    std::cout << "part1: " << part1(5) << std::endl;
    std::cout << "part1: " << part1(3005290) << std::endl;

    std::cout << "part2: " << part2(5) << std::endl;
    std::cout << "part2: " << part2(3005290) << std::endl;
}
