// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <getopt.h>
#include "xcode_redirect.hpp"
using namespace std;


struct SQL {
    bool quiet = false;

    void setup(int argc, char *argv[]);
    
    struct table 
    {   
        vector<vector<int> > data;
        unordered_map<string, unordered_map<string, vector<int> &> > hash;
        unordered_map<string, map<string, vector<int> &> > bst;
    }; 
    map<string,table> SillQL;

};