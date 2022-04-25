#include <bits/stdc++.h>
#include <cstdlib>

using namespace std;

/**
 * <程序> →<变量说明部分>;<语句部分>
 * <变量说明部分> → <变量说明><标识符列表>
 * <变量说明> → int
 * <标识符列表> → <标识符列表>,<标识符>|<标识符>
 * <标识符> → $<字母>|<标识符><字母>|<标识符><数字>
 * <语句部分> → <语句部分><语句>;|<语句>;
 * <语句> → <赋值语句>|<条件语句>|<循环语句>|
 * <赋值语句> → <标识符>=<表达式>
 * <表达式> → <项>|<表达式><加法运算符><项>
 * <项> → <因子>|<项><乘法运算符><因子>
 * <因子> → <标识符>|<常量>|(<表达式>)
 * <常量> → <无符号整数>
 * <无符号整数> → <数字序列>
 * <数字序列> → <数字序列><数字>|<数字>
 * <加法运算符> → +
 * <乘法运算符> → *
 * <关系运算符> → <|>|!= |>=|<= |==
 * <条件> → <表达式><关系运算符><表达式>
 * <复合语句> → begin <语句部分> end
 * <条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句>
 * <循环语句> → while （<条件>） do <嵌套语句>
 * <嵌套语句> → <语句>|<复合语句>
 * <字母> → a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
 * <数字> → 0|1|2|3|4|5|6|7|8|9
 */

#define WORD 1
#define NUM 2
#define SYMBOL 3
#define IDENTIFIER 4

// 标识符      $字母+数字
const unordered_set<string> reserved_words = {"begin", "end", "int", "if", "else", "then", "while", "do"};
const unordered_set<string> reserved_symbols = {"+", "*", "<", ">", "!=", ">=", "<=", "==", ",", ";", "(", ")", "="};
const unordered_set<char> legal_symbols = {'+', '*', '<', '>', '!', ',', ';', '(', ')', '='};
const unordered_set<char> ignore_symbols = {' ', '\n', '\t'};

class IdentifierTable {
    struct Identifier {
        string name;
        string type;
        string value;
    };

    unordered_map<string, Identifier> table;

    bool existIdentifier(string &name) {
        return table.find(name) != table.end();
    }

public:
    bool addIdentifier(string name) {
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
};

class LexicalAnalyzer {
private:
    IdentifierTable identifierTable;
    int ptr{}, state{};
    string temp, source;

    inline bool is_letter(char ch) {
        return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
    }

    inline bool is_digit(char ch) {
        return ch >= '0' && ch <= '9';
    }

    inline bool is_legal_symbol(char ch) {
        return legal_symbols.count(ch) != 0;
    }

    inline int start_type(char ch) {  // 指出由词首字符推理得出的当前词的可能类型
        if (is_letter(ch)) return WORD;
        if (is_digit(ch)) return NUM;
        if (is_legal_symbol(ch)) return SYMBOL;
        if (ch == '$') return IDENTIFIER;
        return -1;
    }

    inline bool ptr_arrive_end(int p) {
        return p == source.size() || source[p] == '#';
    }

    inline bool is_reserved_words(string str) {
        return reserved_words.count(str) != 0;
    }

public:
    explicit LexicalAnalyzer(string &source) {
        this->source = source;
    }

    void analyzeIdentifier() {
        identifierTable.clearTable();
        string res;
        while (!ptr_arrive_end(ptr))
            getNextWord(res);
    }

    bool getNextWord(string &res) {
        while (!ptr_arrive_end(ptr) && ignore_symbols.count(source[ptr]) != 0)
            ptr++;  // 忽略可忽略字符，并且指针没到文末
        if (ptr_arrive_end(ptr)) {
            cout << "Warning: no words any more!" << endl;
            return false;
        }

        // 获取下一个词
        res = "";
        int len = 0;
        string sub_word;
        switch (start_type(source[ptr])) {
            case WORD:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!is_reserved_words(sub_word) &&  // 如果还不是关键字，则需要继续拼下一个字符
                         !ptr_arrive_end(ptr + len) &&  // 防越界
                         is_letter(source[ptr + len]));  // 字符合法

                if (!is_reserved_words(sub_word)) {
                    cout << "[Error]: Word at " << ptr << ", " << sub_word << " is not a reserved word!" << endl;
                    ptr += len;
                    return false;
                }
                // todo 非期望类型
                // if (sub_word != next_word.type)
                //     cout << "[Error]: Word at" << ptr << ", " << sub_word << " unexpected type!" << endl;
                cout << "(关键字, " << sub_word << ")" << endl;
                res = sub_word;
                break;
            case NUM:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         is_digit(source[ptr + len]));  // 字符合法，可以拼成更长的数字序列
                cout << "(常数, " << sub_word << ")" << endl;
                res = sub_word;
                break;
            case SYMBOL:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         source[ptr + len] == '=');  // 字符合法，考虑 "!=", ">=", "<=", "=="
                cout << "(符号, " << sub_word << ")" << endl;
                res = sub_word;
                break;
            case IDENTIFIER:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         (is_letter(source[ptr + len]) ||
                          is_digit(source[ptr + len])));  // 变量名由$开头，并由字母或数字组成
                cout << "(标识符, " << sub_word << ")" << endl;
                identifierTable.addIdentifier(sub_word);
                res = sub_word;
                break;
            default:  // illegal
                cout << "[Error]: Word at" << ptr << ", " << source[ptr] << " is not legal!" << endl;
                ptr++;
                return false;
        }
        ptr += len;
        return true;
    }

    inline string dumpIdentifierTable() {
        return identifierTable.dumpTable();
    }
};

string readFile(const string &filePath) {
    ifstream ifs;
    ifs.open(filePath, ios::in);

    if (!ifs.is_open())
        return "";

    string buf;
    string content;
    while (getline(ifs, buf))
        content += buf;

    ifs.close();
    return content;
}

int main() {
    string source = readFile("source.txt") + "#";
    if (!source.empty()) {
        cout << "==========Source Code==========" << endl;
        cout << source << endl;
        cout << "==========Source Code==========" << endl;
    } else {
        cout << "Read Nothing!" << endl;
        return -1;
    }

    LexicalAnalyzer lexicalAnalyzer(source);
    lexicalAnalyzer.analyzeIdentifier();
    cout << lexicalAnalyzer.dumpIdentifierTable();

    cout << "按回车继续" << endl;
    system("read");
    return 0;
}
