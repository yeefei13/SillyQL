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
#include"TableEntry.h"
using namespace std;


struct SQL {
    bool quiet = false;

    void setup(int argc, char *argv[]);
    
    struct index_condition{
        bool e_hash;
        bool e_bst;
    };

    struct table 
    {   
        vector<vector<TableEntry> > data;
        unordered_map<string,size_t> column;
        vector<string> data_type;
        unordered_map<string, vector<TableEntry> &> hash;
        map<string, vector<TableEntry> &>  bst;
        index_condition table_index;
        vector<index_condition> col_condition;
    }; 
    unordered_map<string,table> SillQL;

    void Running();

    void Create();

    void Delete();

    void Gindex();

    void print();

    void Join();

    void Rmove();

    void quit();

    void insert();



    bool type_convert_bool(string& i);
    double type_convert_double(string& i);
    int type_convert_int(string& i);
};