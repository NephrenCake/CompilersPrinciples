/**
 * @Author: NephrenCake
 * @Date: 2022/5/6
 * @Discription: 临时变量表
 */
#pragma once

#include "Utils.hpp"

// ********************临时变量表********************
class TempVarTable {
private:
    vector<string> table;
public:
    string getNewTempVarName() {
        string name = "T" + to_string(table.size() + 1);
        table.push_back(name);
        return name;
    }

    string dumpTable() {
        string res = "[";
        for (auto &item: table)
            res += item + ", ";
        return res + "]\n";
    }
};
