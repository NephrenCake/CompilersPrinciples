/**
 * @Auther: 刘亦飞
 * @Date: 2022/5/6
 * @Discription: 类型定义
 */
#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>

using namespace std;

// ********************类型定义********************
#define WORD 100
#define WORD_INT 101
#define WORD_BEGIN 102
#define WORD_END 103
#define WORD_IF 104
#define WORD_ELSE 105
#define WORD_THEN 106
#define WORD_WHILE 107
#define WORD_DO 108
#define NUM 200
#define SYMBOL 300
#define SYMBOL_ADD 301
#define SYMBOL_MUL 302
#define SYMBOL_LT 303
#define SYMBOL_GT 304
#define SYMBOL_LE 305
#define SYMBOL_GE 306
#define SYMBOL_NE 307
#define SYMBOL_EQ 308
#define SYMBOL_COMMA 309
#define SYMBOL_SEMICOLON 310
#define SYMBOL_LPAREN 311
#define SYMBOL_RPAREN 312
#define SYMBOL_ASSIGN 313
#define IDENTIFIER 400

// 保留字集合，以及到词法类型映射
const unordered_map<string, int> reserved_words = {{"int",   WORD_INT},
                                                   {"begin", WORD_BEGIN},
                                                   {"end",   WORD_END},
                                                   {"if",    WORD_IF},
                                                   {"else",  WORD_ELSE},
                                                   {"then",  WORD_THEN},
                                                   {"while", WORD_WHILE},
                                                   {"do",    WORD_DO}};
// 保留符号集合，以及到词法类型映射，附赠词法分析部分的文字描述
const unordered_map<string, pair<int, string>> reserved_symbols = {{"+",  {SYMBOL_ADD,       "加法运算符"}},
                                                                   {"*",  {SYMBOL_MUL,       "乘法运算符"}},
                                                                   {"<",  {SYMBOL_LT,        "关系运算符"}},
                                                                   {">",  {SYMBOL_GT,        "关系运算符"}},
                                                                   {"<=", {SYMBOL_LE,        "关系运算符"}},
                                                                   {">=", {SYMBOL_GE,        "关系运算符"}},
                                                                   {"!=", {SYMBOL_NE,        "关系运算符"}},
                                                                   {"==", {SYMBOL_EQ,        "关系运算符"}},
                                                                   {",",  {SYMBOL_COMMA,     "分隔符逗号"}},
                                                                   {";",  {SYMBOL_SEMICOLON, "分隔符分号"}},
                                                                   {"(",  {SYMBOL_LPAREN,    "左括号"}},
                                                                   {")",  {SYMBOL_RPAREN,    "右括号"}},
                                                                   {"=",  {SYMBOL_ASSIGN,    "赋值号"}}};
const unordered_set<char> legal_symbols = {'+', '*', '<', '>', '!', ',', ';', '(', ')', '='};
const unordered_set<char> ignore_symbols = {' ', '\n', '\t'};

// ********************工具函数********************
string getTypenameByID(int ID) {
    switch (ID / 100) {
        case 1:
            for (auto &i: reserved_words)
                if (i.second == ID)
                    return i.first;
        case 2:
            return "Constant";
        case 3:
            for (auto &i: reserved_symbols)
                if (i.second.first == ID)
                    return i.first;
        case 4:
            return "Identifier";
        default:
            return "UnknownType";
    }
}

string readFile(const string &filePath) {
    ifstream ifs;
    ifs.open(filePath, ios::in);

    if (!ifs.is_open()) {
        cout << "[Error]: Open file failed!" << endl;
        return "";
    }

    string buf;
    string content;
    while (getline(ifs, buf))
        content += buf;

    ifs.close();
    return content;
}

void writeFile(const string &filePath, const string &content) {
    ofstream ofs;
    ofs.open(filePath, ios::out);
    if (!ofs.is_open()) {
        cout << "[Error]: Open file failed!" << endl;
        return;
    }
    ofs << content;
    ofs.close();
}
