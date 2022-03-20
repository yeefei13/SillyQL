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
#include "project3.h"
using namespace std;

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    SQL sql;
    sql.setup(argc, argv);
    sql.Running();
    return 0;
}

// get opt long in class
void SQL::setup(int argc, char *argv[])
{

    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    string cap;
    option long_options[] = {
        {"quiet", no_argument, nullptr, 'q'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'}};

    //  Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "qh", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
        case 'h':
            // printHelp();
            exit(0);

        case 'q':
            quiet = true;
            break;

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch
    }     // while
}

void SQL::Running()
{
    string decider;
    while (cin >> decider)
    {
        cout << '%';
        switch (decider[0])
        {
        case 'C':
            Create();
            break;
        case 'I':
            insert();
            break;
        case 'D':
            Delete();
            break;
        case 'G':
            Gindex();
            break;
        case 'P':
            print();
            break;
        case 'J':
            Join();
            break;
        case 'R':
            Rmove();
            break;
        case '#':
            getline(cin, decider);
            break;
        default:
        }
    }
}

void SQL::Create()
{
    string tablename;
    string temp;
    string colname;
    cin >> tablename;
    table cur;
    cout << "New table " << tablename;
    cin >> temp;
    cout << " with colum(s) ";
    vector<string> &cur_data = cur.data_type;
    cur_data.resize(stoi(temp));
    for (size_t i = 0; i < stoi(temp); i++)
    {
        cin >> colname;
        cur_data[i] = colname;
    }
    for (size_t i = 0; i < stoi(temp); i++)
    {
        cin >> colname;
        // cur_data.emplace_back(colname);
        cur.column[colname] = i;
        cout << colname << " ";
    }
    cout << "created\n";
    if (SillQL.find(tablename) == SillQL.end())
    {
        SillQL[tablename] = cur;
    }
    else
    {
        // output error
    }
}

void SQL::insert()
{
    string temp;
    string tablename;
    cin >> temp;
    cin >> tablename;
    cin >> temp;
    int row_num = stoi(temp);
    cin >> temp;
    if (SillQL.find(tablename) == SillQL.end())
    {
        // output error
    }
    else
    {
        table cur_table = SillQL[tablename];
        cout << "Added " << row_num << " rows to " << tablename << " from position " 
        << cur_table.data[0].size() << " to " << cur_table.data[0].size() + row_num - 1;
        for (size_t i = 0; i < row_num; i++)
        {
            for (auto j : cur_table.data_type)
            {
                switch (j[0])
                {
                case 'i':
                    cin >> temp;
                    vector<TableEntry> &cur = cur_table.data[i];
                    cur.emplace_back(type_convert_int(temp));
                    break;
                case 'd':
                    cin >> temp;
                    vector<TableEntry> &cur = cur_table.data[i];
                    cur.emplace_back(type_convert_double(temp));
                    break;
                case 's':
                    cin >> temp;
                    vector<TableEntry> &cur = cur_table.data[i];
                    cur.emplace_back(temp);
                    break;
                case 'b':
                    cin >> temp;
                    vector<TableEntry> &cur = cur_table.data[i];
                    cur.emplace_back(type_convert_bool(temp));
                    break;
                default:
                }
            }
        }
    }
}

void SQL::Delete(){

}

void SQL::Gindex(){
    
}


bool type_convert_bool(string& i){
    if(i[0]=='t'){
        return true;
    }
    return false;
}
double type_convert_double(string& i){
    return stod(i);
}
int type_convert_int(string& i){
    return stoi(i);
}