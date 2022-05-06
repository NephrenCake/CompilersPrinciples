/**
 * @Auther: 刘亦飞
 * @Date: 2022/5/6
 * @Discription: 四元式表
 */
#pragma once

#include "Utils.hpp"

// ********************四元式表********************
class CodeTable {
private:
    struct Quaternary {
        string op, arg1, arg2, result;
    };

    vector<Quaternary> quaternaries;

public:
    bool addQuaternary(const string &op, const string &arg1, const string &arg2, const string &result) {
        quaternaries.push_back({op, arg1, arg2, result});
        // cout << "(" + op + ", " + arg1 + ", " + arg2 + ", " + result + ")" << endl;
        return true;
    }

    inline int NXQ() {
        return (int) quaternaries.size();
    }

    bool updateResultByIndex(int idx, const string &result) {
        if (idx < 0 || idx >= quaternaries.size())
            return false;
        quaternaries[idx].result = result;
        return true;
    }

    void clearTable() {
        quaternaries.clear();
    }

    string dumpTable() {
        string res;
        for (int i = 0; i < quaternaries.size(); i++) {
            auto &item = quaternaries[i];
            res += "(" + to_string(i + 1) + ") ("
                   + item.op + ", " + item.arg1 + ", " + item.arg2 + ", " + item.result + ")\n";
        }
        return res;
    }
};
