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
#include "silly.h"
using namespace std;

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    cin.tie(0);
    cin >> std::boolalpha; // add these two lines
    cout << std::boolalpha;
    SQL sql;
    sql.setup(argc, argv);
    sql.Running();
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
    bool stop = true;
    while (cin >> decider && stop)
    {
        cout << "% ";
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
        case 'Q':
            stop = false;
            break;
        case '#':
            getline(cin, decider);
            break;
        default:
            cout << "Error: unrecognized command\n";
            getline(cin, decider);
        }
    }
    cout << "Thanks for being silly!";
}

void SQL::Create()
{
    cin >> tablename >> temp;
    if (SillQL.find(tablename) != SillQL.end())
    {
        getline(cin, temp);
        cout << "Error during " << decider << ": Cannot create already existing table " << tablename << '\n';
        return;
    }
    table cur;
    cout << "New table " << tablename << " with column(s) ";
    vector<int> &cur_data = cur.data_type;
    cur_data.reserve((size_t)stoi(temp));
    cur.col_condition.resize((size_t)stoi(temp), 0);
    for (size_t i = 0; i < (size_t)stoi(temp); i++)
    {
        cin >> colname;
        if (colname[0] == 'i')
        {
            cur_data.emplace_back(0);
        }
        else if (colname[0] == 'd')
        {
            cur_data.emplace_back(1);
        }
        if (colname[0] == 's')
        {
            cur_data.emplace_back(2);
        }
        if (colname[0] == 'b')
        {
            cur_data.emplace_back(3);
        }
    }
    for (size_t i = 0; i < (size_t)stoi(temp); i++)
    {
        cin >> colname;
        cur.column.emplace(colname, i);
        cout << colname << " ";
    }
    cout << "created\n";

    cur.col--;
    SillQL.emplace(tablename, cur);
}

void SQL::insert()
{
    size_t row_num;
    cin >> temp >> tablename >> row_num >> temp;
    if (SillQL.find(tablename) == SillQL.end())
    {
        cout << "Error during INSERT: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    else
    {
        table &cur_table = SillQL[tablename];
        size_t size = cur_table.data.size();
        cout << "Added " << row_num << " rows to " << tablename << " from position "
             << size << " to " << size + row_num - 1 << '\n';
        cur_table.data.reserve(size + row_num);
        size_t forreserve = cur_table.data_type.size();
        for (size_t i = 0; i < row_num; i++)
        {
            vector<TableEntry> ccur;
            ccur.reserve(forreserve);
            for (auto j : cur_table.data_type)
            {
                cin >> temp;
                ccur.emplace_back(type_convert(temp, j));
            }
            cur_table.data.push_back(ccur);
        }
        cur_table.table_condition = 0;
    }
}

void SQL::Delete()
{
    cin >> temp >> tablename >> temp >> colname >> op >> value;
    if (SillQL.find(tablename) == SillQL.end())
    {
        cout << "Error during DELETE: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    table &cur_table = SillQL[tablename];
    if (cur_table.column.find(colname) == cur_table.column.end())
    {
        cout << "Error during DELETE: " << colname << " does not name a column in " << tablename << "\n";
        getline(cin, temp);
        return;
    }
    size_t cur_index = cur_table.column[colname];
    // vector<vector<TableEntry>> temp;

    int j = cur_table.data_type[cur_index];
    TableEntry cur_value(type_convert(value, j));
    size_t num = cur_table.data.size();
    // if(num != 0){
    //     cout<<"\n what happen here:  ";
    //     cout << j <<'\n'<<"dfadfasdfdsaf\n";
    //     cout << cur_table.data[1][cur_index] <<'\n';}

    if (op[0] == '<')
    {
        compare_less f(cur_value, cur_index);
        // num = cur_table.data.size();
        auto begin = remove_if(cur_table.data.begin(), cur_table.data.end(), f);
        cur_table.data.erase(begin, cur_table.data.end());
        num = num - cur_table.data.size();
    }
    else if (op[0] == '=')
    {
        compare_eq f(cur_value, cur_index);
        // num = cur_table.data.size();
        auto begin = remove_if(cur_table.data.begin(), cur_table.data.end(), f);
        cur_table.data.erase(begin, cur_table.data.end());
        num = num - cur_table.data.size();
    }
    else
    {

        compare_lar f(cur_value, cur_index);
        // num = cur_table.data.size();
        auto begin = remove_if(cur_table.data.begin(), cur_table.data.end(), f);
        cur_table.data.erase(begin, cur_table.data.end());
        num = num - cur_table.data.size();
    }

    // if (num != 0){
    //     // cur_table.data.clear();
    //     cur_table.data = temp;}
    cout << "Deleted " << num << " rows from " << tablename << '\n';
    cur_table.table_condition = 0;
}

void SQL::Gindex()
{
    string index_type;
    cin >> temp;
    cin >> tablename;
    cin >> index_type;
    cin >> temp;
    cin >> temp;
    cin >> colname;
    // check for error
    if (SillQL.find(tablename) == SillQL.end())
    {
        cout << "Error during GENERATE: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    table &cur_table = SillQL[tablename];
    if (cur_table.column.find(colname) == cur_table.column.end())
    {
        cout << "Error during GENERATE: " << colname << " does not name a column in " << tablename << "\n";
        getline(cin, temp);
        return;
    }
    size_t cur_index = cur_table.column[colname];
    if (index_type[0] == 'h')
    {
        cur_table.col_condition[cur_index] = 1;
    }
    else if (index_type[0] == 'b')
    {
        cur_table.col_condition[cur_index] = 2;
    }
    cout << "Created " << index_type << " index for table " << tablename << " on column " << colname << '\n';
}

void SQL::print()
{
    cin >> temp >> tablename >> temp;
    if (SillQL.find(tablename) == SillQL.end())
    {
        // error
        cout << "Error during PRINT: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    size_t j = (size_t)stoi(temp);
    table &cur_table = SillQL[tablename];
    vector<size_t> print_table;
    vector<string> print_colname;
    int count = 0;
    for (size_t i = j; i > 0; i--)
    {
        cin >> temp;
        auto k = cur_table.column.find(temp);
        if (k == cur_table.column.end())
        {
            // error
            cout << "Error during PRINT: " << temp << " does not name a column in " << tablename << "\n";
            getline(cin, temp);
            return;
        }
        print_table.push_back(k->second);
        print_colname.emplace_back(temp);
        // cout << temp << " ";
    }
    // cout << '\n';
    cin >> temp;
    // cout << temp <<'\n';
    if (temp[0] == 'W')
    {
        cin >> colname >> op >> value;
        auto k = cur_table.column.find(colname);
        if (k == cur_table.column.end())
        {
            // error
            cout << "Error during PRINT: " << colname << " does not name a column in " << tablename << "\n";
            getline(cin, temp);
            return;
        }
        for (auto i : print_colname)
        {
            if (!quiet)
                cout << i << " ";
        }
        if (!quiet)
            cout << '\n';
        size_t cur_index = k->second;

        // using bst table -> print bst order
        if (cur_table.col_condition[cur_index] == 2)
        {
            if (!(cur_table.table_condition == 2 && cur_table.col == cur_index))
            {
                // generate table
                cur_table.GenerateBstTable(cur_index);
            }
            if (op[0] == '>')
            {
                for (auto i = cur_table.bst.upper_bound(type_convert(value, cur_table.data_type[cur_index])); i != cur_table.bst.end(); i++)
                {
                    for (auto k : i->second)
                    {
                        if (!quiet)
                        {
                            for (auto j : print_table)
                            {

                                cout << cur_table.data[k][j] << " ";
                            }
                            cout << '\n';
                        }
                        count++;
                    }
                }
            }
            else if (op[0] == '<')
            {
                for (auto i = cur_table.bst.begin(); i != cur_table.bst.lower_bound(type_convert(value, cur_table.data_type[cur_index])); i++)
                {
                    for (auto k : i->second)
                    {
                        if (!quiet)
                        {
                            for (auto j : print_table)
                            {
                                cout << cur_table.data[k][j] << " ";
                            }
                            cout << '\n';
                        }
                        count++;
                    }
                }
            }
            else
            {
                for (auto k : cur_table.bst[type_convert(value, cur_table.data_type[cur_index])])
                {
                    if (!quiet)
                    {
                        for (auto j : print_table)
                        {
                            cout << cur_table.data[k][j] << " ";
                        }
                        cout << '\n';
                    }
                    count++;
                }
            }
            cout << "Printed " << count << " matching rows from " << tablename << '\n';
        }

        // using hash table with condition of = sign -> print input order
        else if (op[0] == '=')
        {

            if (!(cur_table.table_condition == 1 && cur_table.col == cur_index))
            {
                // generate table
                cur_table.GenerateHashTable(cur_index);
            }

            auto cur_vecitr = cur_table.hash.find(type_convert(value, cur_table.data_type[cur_index]));
            if (cur_vecitr != cur_table.hash.end())
            {
                vector<size_t> cur_vec = cur_vecitr->second;
                for (auto i : cur_vec)
                {
                    if (!quiet)
                    {
                        for (auto j : print_table)
                        {
                            cout << cur_table.data[i][j] << " ";
                        }
                        cout << '\n';
                    }
                    count++;
                }
            }

            cout << "Printed " << count << " matching rows from " << tablename << '\n';
        }
        else
        {
            if (op[0] == '>')
            {
                // cout << "inhere";
                compare_lar f(type_convert(value, cur_table.data_type[cur_index]), cur_index);
                for (auto i : cur_table.data)
                {
                    if (f(i))
                    {
                        if (!quiet)
                        {
                            for (auto j : print_table)
                            {
                                cout << i[j] << " ";
                            }
                            cout << '\n';
                        }
                        count++;
                    }
                }
            }
            else
            {
                compare_less f(type_convert(value, cur_table.data_type[cur_index]), cur_index);
                for (auto i : cur_table.data)
                {
                    if (f(i))
                    {
                        if (!quiet)
                        {
                            for (auto j : print_table)
                            {
                                cout << i[j] << " ";
                            }
                            cout << '\n';
                        }
                        count++;
                    }
                }
            }
            cout << "Printed " << count << " matching rows from " << tablename << '\n';
        }
    }
    else
    {
        // cout << cur_table.data.size();
        if (!quiet)
        {
            for (auto i : print_colname)
            {
                cout << i << ' ';
            }
            cout << '\n';

            for (auto i : cur_table.data)
            {
                // cout << "haapen here \n";
                for (auto j : print_table)
                {
                    cout << i[j] << " ";
                }
                cout << '\n';
            }
        }
        cout << "Printed " << cur_table.data.size() << " matching rows from " << tablename << '\n';
    }
}

void SQL::Join()
{
    string tablename2;
    string colname2;
    char table_num;
    int count = 0;
    cin >> tablename >> temp >> tablename2 >> temp >> colname >> temp >> colname2 >> temp >> temp >> value;
    if (SillQL.find(tablename) == SillQL.end() || SillQL.find(tablename2) == SillQL.end())
    {
        // error
        cout << "Error during JOIN: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    table &cur_table1 = SillQL[tablename];
    table &cur_table2 = SillQL[tablename2];
    auto curit1 = cur_table1.column.find(colname);
    auto curit2 = cur_table2.column.find(colname2);
    if (curit1 == cur_table1.column.end())
    {
        // error
        // cout << colname << " is not the name of a column in the table specified by " << tablename << '\n';
        cout << "Error during JOIN: " << colname << " does not name a column in " << tablename << "\n";
        getline(cin, temp);
        return;
    }
    if (curit2 == cur_table2.column.end())
    {
        // error
        // cout << colname2 << " is not the name of a column in the table specified by " << tablename2 << '\n';
        cout << "Error during JOIN: " << colname2 << " does not name a column in " << tablename2 << "\n";
        getline(cin, temp);
        return;
    }

    // pair of first = colnum second = table num where first pair is the compare col {1,2}
    vector<pair<size_t, size_t>> print_table;
    vector<string> print_colname;
    print_table.emplace_back((*curit1).second, (*curit2).second);
    for (size_t i = 0; i < (size_t)stoi(value); i++)
    {
        cin >> temp >> table_num;
        if (table_num == '1')
        {
            auto curit = cur_table1.column.find(temp);
            if (curit == cur_table1.column.end())
            {
                // error
                cout << "Error during JOIN: " << temp << " does not name a column in " << tablename << "\n";
                getline(cin, temp);
                return;
            }

            print_table.emplace_back((*curit).second, 1);
            print_colname.emplace_back(temp);
        }
        else
        {
            auto curit = cur_table2.column.find(temp);
            if (curit == cur_table2.column.end())
            {
                // error
                cout << "Error during JOIN: " << temp << " does not name a column in " << tablename2 << "\n";
                getline(cin, temp);
                return;
            }
            print_table.push_back({(*curit).second, 2});
            print_colname.push_back(temp);
        }
    }
    // print
    for (size_t i = 0; i < (size_t)stoi(value); i++)
    {
        if (!quiet)
            cout << print_colname[i] << " ";
    }
    if (!quiet)
        cout << '\n';
    // each row in table 1
    for (size_t i = 0; i < cur_table1.data.size(); i++)
    {
        // should print?
        TableEntry anchor(cur_table1.data[i][print_table[0].first]);
        if (!(cur_table2.table_condition == 1 && cur_table2.col == print_table[0].second))
        {
            // generate table
            cur_table2.GenerateHashTable(print_table[0].second);
        }
        auto cur_vecitr = cur_table2.hash.find(anchor);
        // can print
        if (cur_vecitr != cur_table2.hash.end())
        {
            vector<size_t> cur_vec2_index = cur_vecitr->second;
            if (!quiet)
            {
                vector<TableEntry> cur_vec1 = cur_table1.data[i];
                // each row in table two
                for (auto k : cur_vec2_index)
                {
                    // each column
                    for (size_t j = 1; j < print_table.size(); j++)
                    {

                        if (print_table[j].second == 1)
                        {
                                cout << cur_vec1[print_table[j].first] << ' ';
                        }
                        else
                        {
                                cout << cur_table2.data[k][print_table[j].first] << ' ';
                        }
                    }
                        cout << '\n';
                    count++;
                }
            }
            else
            {
                count += (int)cur_vec2_index.size();
            }
        }
    }
    cout << "Printed " << count << " rows from joining " << tablename << " to " << tablename2 << '\n';
}

void SQL::Rmove()
{
    cin >> tablename;
    if (SillQL.find(tablename) == SillQL.end())
    {
        // error
        cout << "Error during REMOVE: " << tablename << " does not name a table in the database\n";
        getline(cin, temp);
        return;
    }
    SillQL.erase(tablename);
    cout << "Table " << tablename << " deleted\n";
}

void SQL::table::GenerateHashTable(size_t &index)
{
    bst.clear();
    hash.clear();
    for (size_t i = 0; i < data.size(); i++)
    {
        // auto k = hash.find(data[i][index]);
        // if(k == hash.end()){
        //     vector<size_t> a;
        //     hash[data[i][index]] = a;
        // }
        hash[data[i][index]].emplace_back(i);
    }
    table_condition = 1;
    col = index;
}

void SQL::table::GenerateBstTable(size_t &index)
{
    bst.clear();
    hash.clear();
    for (size_t i = 0; i < data.size(); i++)
    {
        // auto k = bst.find(data[i][index]);
        // if(k == bst.end()){
        //     vector<size_t > a;
        //     bst[data[i][index]] = a;
        // }
        bst[data[i][index]].emplace_back(i);
    }
    table_condition = 2;
    col = index;
}
