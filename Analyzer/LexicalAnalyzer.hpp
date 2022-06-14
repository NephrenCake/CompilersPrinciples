/**
 * @Author: NephrenCake
 * @Date: 2022/5/6
 * @Discription: 词法分析
 */
#pragma once

#include "../Component/IdentifierTable.hpp"

// ********************词法分析部分********************
class LexicalAnalyzer {
private:
    int ptr{};
    string source;

    static inline bool is_letter(char ch) {
        return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
    }

    static inline bool is_digit(char ch) {
        return ch >= '0' && ch <= '9';
    }

    static inline bool is_legal_symbol(char ch) {
        return legal_symbols.count(ch) != 0;
    }

    static inline int start_type(char ch) {  // 指出由词首字符推理得出的当前词的可能类型
        if (is_letter(ch)) return WORD;
        if (is_digit(ch)) return NUM;
        if (is_legal_symbol(ch)) return SYMBOL;
        if (ch == '$') return IDENTIFIER;
        return -1;
    }

    inline bool ptr_arrive_end(int p) {
        return p == source.size() || source[p] == '#';
    }

    static inline bool is_reserved_words(const string &str) {
        return reserved_words.count(str) != 0;
    }

public:
    explicit LexicalAnalyzer(string &source) {
        this->source = source;
    }

    inline int getCur() const {
        return ptr;
    }

    /**
     * 解析代码中的标识符，并输出到标识符表
     * @param identifierTable 标识符表
     */
    void analyzeIdentifier(IdentifierTable &identifierTable) {
        identifierTable.clearTable();
        ptr = 0;
        string res;
        int type;
        while (!ptr_arrive_end(ptr)) {
            getNextWord(res, type);
            if (type == IDENTIFIER)
                identifierTable.addIdentifier(res);
            cout << res << " " << type << endl;
        }
        ptr = 0;
    }

    /**
     * 按顺序获取下一个词。若获取失败，不会改变传入的变量。
     * @param res 传入引用，用于传出下一个词
     * @param type 传入引用，用于传出下一个词的类型
     * @return 表示是否正确获取到下一个词
     */
    bool getNextWord(string &res, int &type) {
        while (!ptr_arrive_end(ptr) && ignore_symbols.count(source[ptr]) != 0)
            ptr++;  // 忽略可忽略字符，并且指针没到文末
        if (ptr_arrive_end(ptr)) {
            if (source[ptr] != '#') {
                cout << "[Warning]: no words any more!" << endl;
                return false;
            } else {
                res = '#';
                type = -1;
                cout << "【词】(#, #)" << endl;
                cout << "词法分析结束" << endl;
                return true;
            }
        }

        // 枚举长度向后拼接，并输出二元式
        int len = 0, lv1_type = start_type(source[ptr]), lv2_type = 0;
        string sub_word;
        switch (lv1_type) {
            case WORD: {
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!is_reserved_words(sub_word) &&  // 如果还不是关键字，则需要继续拼下一个字符
                         !ptr_arrive_end(ptr + len) &&  // 防越界
                         is_letter(source[ptr + len]));  // 字符合法
                if (!is_reserved_words(sub_word)) {
                    cout << "[Lexical Error]: Position " << ptr << ". '" << sub_word <<
                         "' is not a reserved next_word! Will ignore it!" << endl;
                    ptr += len;
                    return false;
                }  // 处理匹配结束但非法情况
                lv2_type = reserved_words.find(sub_word)->second;
                cout << "【词】(保留字" << sub_word << ", " << sub_word << ")" << endl;
                break;
            }
            case NUM: {
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         is_digit(source[ptr + len]));  // 字符合法，可以拼成更长的数字序列
                cout << "【词】(常量, " << sub_word << ")" << endl;
                break;
            }
            case SYMBOL: {
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         source[ptr + len] == '=');  // 字符合法，考虑 "!=", ">=", "<=", "=="
                lv2_type = reserved_symbols.find(sub_word)->second.first;
                cout << "【词】(" << reserved_symbols.find(sub_word)->second.second << ", " << sub_word << ")" << endl;
                break;
            }
            case IDENTIFIER: {
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         (is_letter(source[ptr + len]) ||
                          is_digit(source[ptr + len])));  // 变量名由$开头，并由字母或数字组成
                cout << "【词】(标识符, " << sub_word << ")" << endl;
                break;
            }
            default: {// illegal
                cout << "[Lexical Error]: Position " + to_string(ptr) + ". '" + source[ptr] + "' is not legal!" << endl;
                ptr++;
                return false;
            }
        }
        res = sub_word;
        type = lv2_type != 0 ? lv2_type : lv1_type;
        ptr += len;
        return true;
    }
};
