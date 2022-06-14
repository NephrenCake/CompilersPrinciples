/**
 * @Author: NephrenCake
 * @Date: 2022/5/6
 * @Discription: 标识符、临时变量
 */
#pragma once

#include "Utils.hpp"

// ********************标识符、临时变量********************
class Identifier {
public:
    string name, type, value;

    static bool isAbleToCal(const Identifier &i1, const Identifier &i2) {
        bool is_ok = (i1.type == getTypenameByID(WORD_INT) || i1.type == getTypenameByID(NUM)) &&
                     (i2.type == getTypenameByID(WORD_INT) || i2.type == getTypenameByID(NUM));
        if (!is_ok)
            cout << "[Semantic error]: Can only use override operator on two INT" << endl;
        return is_ok;
    }

    Identifier operator+(const Identifier &i2) {
        if (!isAbleToCal(*this, i2))
            return {"", "", ""};
        return {"", this->type, to_string(stoi(this->value) + stoi(i2.value))};
    }

    Identifier operator*(const Identifier &i2) {
        if (!isAbleToCal(*this, i2))
            return {"", "", ""};
        return {"", this->type, to_string(stoi(this->value) * stoi(i2.value))};
    }

    bool operator==(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) == stoi(i2.value);
    }

    bool operator!=(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) != stoi(i2.value);
    }

    bool operator<(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) < stoi(i2.value);
    }

    bool operator>(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) > stoi(i2.value);
    }

    bool operator<=(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) <= stoi(i2.value);
    }

    bool operator>=(const Identifier &i2) const {
        if (!isAbleToCal(*this, i2))
            return false;
        return stoi(this->value) >= stoi(i2.value);
    }
};
