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
#include "TableEntry.h"
using namespace std;

struct SQL
{
    bool quiet = false;
    string colname;
    string tablename;
    string temp;
    string op;
    string value;
    string decider;

    struct compare_less
    {
        TableEntry anchor;
        size_t index;
        compare_less(TableEntry a,size_t i) : anchor(a),index(i)
        {
        }
        bool operator()(vector<TableEntry> b)
        {
            return b[index] < anchor;
        }
    };
    struct compare_eq
    {
        TableEntry anchor;
        size_t index;
        compare_eq(TableEntry a,size_t i) : anchor(a),index(i)
        {
        }
        bool operator()(vector<TableEntry> b)
        {
            return b[index] == anchor;
        }
    };
    struct compare_lar
    {
        TableEntry anchor;
        size_t index;
        compare_lar(TableEntry a,size_t i) : anchor(a),index(i)
        {
        }
        bool operator()(vector<TableEntry> b)
        {
            return b[index] > anchor;
        }
    };

    struct table
    {
        vector<vector<TableEntry>> data;
        unordered_map<string, size_t> column;
        vector<string> data_type;
        unordered_map<TableEntry, vector<size_t>> hash;
        map<TableEntry, vector<size_t>> bst;
        // 0 -> both no 1-> hash yes 2-> bst yes 3-> both yes
        uint8_t table_condition = 0;
        size_t col = 0;
        // 0 -> both no 1-> hash yes 2-> bst yes
        vector<uint8_t> col_condition;

        void GenerateHashTable(size_t &index);
        void GenerateBstTable(size_t &index);
    };
    unordered_map<string, table> SillQL;

    void setup(int argc, char *argv[]);
    void Running(); // done

    void Create(); // done

    void Delete(); // done

    void Gindex(); // done

    void print();

    void Join();

    void Rmove(); // done

    void quit(); // done

    void insert(); // done

    bool type_convert_bool(string &i)
    {
        if (i[0] == 't')
        {
            return true;
        }
        return false;
    }
    double type_convert_double(string &i)
    {
        return stod(i);
    }
    int type_convert_int(string &i)
    {
        return stoi(i);
    }
    TableEntry type_convert(string &i, string &type)
    {
        if (type[0] == 'i')
        {
            TableEntry cur_value(type_convert_int(i));
            return cur_value;
        }

        else if (type[0] == 'd')
        {
            TableEntry cur_value(type_convert_double(i));
            return cur_value;
        }
        else if (type[0] == 's')
        {
            TableEntry cur_value(i);
            return cur_value;
        }
        TableEntry cur_value(type_convert_bool(i));
        return cur_value;
    }
};