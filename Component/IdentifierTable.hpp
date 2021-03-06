/**
 * @Author: NephrenCake
 * @Date: 2022/5/6
 * @Discription: 标识符表
 */
#pragma once

#include "Identifier.hpp"

// ********************标识符表********************
class IdentifierTable {
    unordered_map<string, Identifier> table;

public:
    bool addIdentifier(const string &name) {
        if (existIdentifier(name))
            return false;
        table[name] = {name, "", ""};
        return true;
    }

    void clearTable() {
        table.clear();
    }

    string dumpTable() {
        string res;
        for (auto &item: table)
            res += "name: " + item.second.name + "  " +
                   "type: " + item.second.type + "  " +
                   "value: " + item.second.value + "\n";
        return res;
    }

    bool existIdentifier(const string &name) {
        return table.find(name) != table.end();
    }

    bool updateIdentifierType(const string &name, const string &type) {
        if (!existIdentifier(name))
            return false;
        table[name].type = type;
        if (type == getTypenameByID(WORD_INT))
            table[name].value = "0";
        return true;
    }

    bool updateIdentifierValue(const string &name, const string &value) {
        if (!existIdentifier(name))
            return false;
        table[name].value = value;
        return true;
    }

    Identifier getIdentifier(const string &name) {
        if (!existIdentifier(name))
            return {"", "", ""};
        return table[name];
    }
};
