#include <bits/stdc++.h>
#include <cstdlib>

using namespace std;

/**
 * <����> ��<����˵������>;<��䲿��>
 * <����˵������> �� <����˵��><��ʶ���б�>
 * <����˵��> �� int
 * <��ʶ���б�> �� <��ʶ���б�>,<��ʶ��>|<��ʶ��>
 * <��ʶ��> �� $<��ĸ>|<��ʶ��><��ĸ>|<��ʶ��><����>
 * <��䲿��> �� <��䲿��><���>;|<���>;
 * <���> �� <��ֵ���>|<�������>|<ѭ�����>|
 * <��ֵ���> �� <��ʶ��>=<���ʽ>
 * <���ʽ> �� <��>|<���ʽ><�ӷ������><��>
 * <��> �� <����>|<��><�˷������><����>
 * <����> �� <��ʶ��>|<����>|(<���ʽ>)
 * <����> �� <�޷�������>
 * <�޷�������> �� <��������>
 * <��������> �� <��������><����>|<����>
 * <�ӷ������> �� +
 * <�˷������> �� *
 * <��ϵ�����> �� <|>|!= |>=|<= |==
 * <����> �� <���ʽ><��ϵ�����><���ʽ>
 * <�������> �� begin <��䲿��> end
 * <�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����>
 * <ѭ�����> �� while ��<����>�� do <Ƕ�����>
 * <Ƕ�����> �� <���>|<�������>
 * <��ĸ> �� a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
 * <����> �� 0|1|2|3|4|5|6|7|8|9
 */

#define WORD 1
#define NUM 2
#define SYMBOL 3
#define IDENTIFIER 4

// ��ʶ��      $��ĸ+����
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

    bool existIdentifier(string &name) {
        return table.find(name) != table.end();
    }
};

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

    static inline int start_type(char ch) {  // ָ���ɴ����ַ�����ó��ĵ�ǰ�ʵĿ�������
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

    /**
     * ��������ָ��
     */
    void resetCur() {
        ptr = 0;
    }

    /**
     * ���������еı�ʶ�������������ʶ����
     * @param identifierTable ��ʶ����
     */
    void analyzeIdentifier(IdentifierTable& identifierTable) {
        identifierTable.clearTable();
        resetCur();
        string res;
        int type;
        while (!ptr_arrive_end(ptr)) {
            getNextWord(res, type);
            switch (type) {
                case WORD:
                    cout << "(�ؼ���, " << res << ")" << endl;
                    break;
                case NUM:
                    cout << "(����, " << res << ")" << endl;
                    break;
                case SYMBOL:
                    cout << "(����, " << res << ")" << endl;
                    break;
                case IDENTIFIER:
                    cout << "(��ʶ��, " << res << ")" << endl;
                    identifierTable.addIdentifier(res);
                    break;
                default: break;
            }
        }
        resetCur();
    }

    /**
     * ��˳���ȡ��һ����
     * @param res �������ã����ڴ�����һ����
     * @param type �������ã����ڴ�����һ���ʵ�����
     * @return ��ʾ�Ƿ���ȷ��ȡ����һ����
     */
    bool getNextWord(string &res, int &type) {
        while (!ptr_arrive_end(ptr) && ignore_symbols.count(source[ptr]) != 0)
            ptr++;  // ���Կɺ����ַ�������ָ��û����ĩ
        if (ptr_arrive_end(ptr)) {
            cout << "[Warning]: no words any more!" << endl;
            return false;
        }

        // ö�ٳ������ƴ�ӣ��������Ԫʽ
        int len = 0;
        string sub_word;
        switch (start_type(source[ptr])) {
            case WORD:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!is_reserved_words(sub_word) &&  // ��������ǹؼ��֣�����Ҫ����ƴ��һ���ַ�
                         !ptr_arrive_end(ptr + len) &&  // ��Խ��
                         is_letter(source[ptr + len]));  // �ַ��Ϸ�
                if (!is_reserved_words(sub_word)) {
                    cout << "[Lexical Error]: Word at " << ptr << ", " << sub_word <<
                         " is not a reserved word! Will ignore it!" << endl;
                    ptr += len;
                    return false;
                }
                break;

            case NUM:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         is_digit(source[ptr + len]));  // �ַ��Ϸ�������ƴ�ɸ�������������
                break;

            case SYMBOL:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         source[ptr + len] == '=');  // �ַ��Ϸ������� "!=", ">=", "<=", "=="
                break;

            case IDENTIFIER:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         (is_letter(source[ptr + len]) ||
                          is_digit(source[ptr + len])));  // ��������$��ͷ��������ĸ���������
                break;

            default:  // illegal
                cout << "[Error]: Word at" << ptr << ", " << source[ptr] << " is not legal!" << endl;
                ptr++;
                return false;
        }
        res = sub_word;
        type = start_type(source[ptr]);
        ptr += len;
        return true;
    }
};

class SyntaxAnalyzer {
private:
    bool have_error = false;
    LexicalAnalyzer *lexicalAnalyzer;

public:
    explicit SyntaxAnalyzer(LexicalAnalyzer &lexicalAnalyzer) {

    }

    // void parse_PROGRAM() {
    //     cout << "<����> -> <����˵������>;<��䲿��>" << endl;
    //     parse_DATA();
    //     parse_CODE();
    // }

private:
    // void parse_DATA() {
    //     match_word()
    // }
    //
    // void parse_CODE() {
    //
    // }
    //
    // void match_word(int expected_type) {
    //     if (lookahead_type != expected_type) {
    //         cout << "Line " << cur_line << ": " << endl;
    //         cout << "[Syntax error]: Expect ";
    //         if (expected_type == 0 || expected_type == 24) {
    //             cout << error_check[expected_type] << " \"" << next_word.value << "\"";
    //         } else {
    //             cout << "\"" << error_check[expected_type] << "\"";
    //         }
    //         cout << ", but get \"" << next_word.value << "\"." << endl;
    //         // exit(0);
    //         have_error = true;
    //     }
    //     take_word();
    //     lookahead_type = next_word.type;
    //
    // }
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
    if (source.empty() || source == "#") {
        cout << "[Error]: Read Nothing!" << endl;
        return -1;
    }

    cout << "========== Source Code ==========" << endl;
    cout << source << endl;
    cout << "========== Source Code ==========" << endl << endl;

    cout << "========== Lexical Analyze ==========" << endl;
    LexicalAnalyzer lexicalAnalyzer(source);
    IdentifierTable identifierTable;
    lexicalAnalyzer.analyzeIdentifier(identifierTable);
    cout << "========== Lexical Analyze ==========" << endl << endl;

    cout << "========== Identifier Table ==========" << endl;
    cout << identifierTable.dumpTable();
    cout << "========== Identifier Table ==========" << endl << endl;

    cout << "========== Syntax Analyze ==========" << endl;

    cout << "========== Syntax Analyze ==========" << endl << endl;

    cout << "���س�����" << endl;
    system("read");
    return 0;
}
