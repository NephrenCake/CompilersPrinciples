#include <bits/stdc++.h>

using namespace std;

// 四个一级分类（保留字，常数，符号，标识符）；保留字、符号有二级分类。
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
const unordered_map<string, int> reserved_words = {{"int", WORD_INT},
                                                   {"begin", WORD_BEGIN},
                                                   {"end", WORD_END},
                                                   {"if", WORD_IF},
                                                   {"else", WORD_ELSE},
                                                   {"then", WORD_THEN},
                                                   {"while", WORD_WHILE},
                                                   {"do", WORD_DO}};
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

// 标识符表
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

// 词法分析部分
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
            if (source[ptr] != '#')
                cout << "[Warning]: no words any more!" << endl;
            res = '#';
            type = -1;
            return false;
        }

        // 枚举长度向后拼接，并输出二元式
        int len = 0, lv1_type = start_type(source[ptr]), lv2_type = 0;
        string sub_word;
        switch (lv1_type) {
            case WORD:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!is_reserved_words(sub_word) &&  // 如果还不是关键字，则需要继续拼下一个字符
                         !ptr_arrive_end(ptr + len) &&  // 防越界
                         is_letter(source[ptr + len]));  // 字符合法
                if (!is_reserved_words(sub_word)) {
                    cout << "[Lexical Error]: Word at " << ptr << ", '" << sub_word <<
                         "' is not a reserved next_word! Will ignore it!" << endl;
                    ptr += len;
                    return false;
                }  // 处理匹配结束但非法情况
                lv2_type = reserved_words.find(sub_word)->second;
                cout << "【词】(保留字" << sub_word << ", " << sub_word << ")" << endl;
                break;

            case NUM:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         is_digit(source[ptr + len]));  // 字符合法，可以拼成更长的数字序列
                cout << "【词】(常量, " << sub_word << ")" << endl;
                break;

            case SYMBOL:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         source[ptr + len] == '=');  // 字符合法，考虑 "!=", ">=", "<=", "=="
                lv2_type = reserved_symbols.find(sub_word)->second.first;
                cout << "【词】(" << reserved_symbols.find(sub_word)->second.second << ", " << sub_word << ")" << endl;
                break;

            case IDENTIFIER:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // 防越界
                         (is_letter(source[ptr + len]) ||
                          is_digit(source[ptr + len])));  // 变量名由$开头，并由字母或数字组成
                cout << "【词】(标识符, " << sub_word << ")" << endl;
                break;

            default:  // illegal
                cout << "[Lexical Error]: Word at" << ptr << ", " << source[ptr] << " is not legal!" << endl;
                ptr++;
                return false;
        }
        res = sub_word;
        type = lv2_type != 0 ? lv2_type : lv1_type;
        ptr += len;
        return true;
    }
};

// 语法分析部分
/**
 * <程序> → <变量说明部分>;<语句部分>
 * <变量说明部分> → int<标识符列表>
 * <标识符列表> → <标识符><标识符列表prime>
 * <标识符列表prime> → ,<标识符><标识符列表prime>|ε
 * <语句部分> → <语句>;<语句部分prime>
 * <语句部分prime> → <语句>;<语句部分prime>|ε
 * <语句> → <赋值语句>|<条件语句>|<循环语句>
 * <赋值语句> → <标识符>=<表达式>
 * <条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句>
 * <循环语句> → while （<条件>） do <嵌套语句>
 * <表达式> → <项><表达式prime>
 * <表达式prime> → +<项><表达式prime>|ε
 * <项> → <因子><项prime>
 * <项prime> → *<因子><项prime>|ε
 * <因子> → <标识符>|<常量>|(<表达式>)
 * <条件> → <表达式><关系运算符><表达式>
 * <嵌套语句> → <语句>|<复合语句>
 * <复合语句> → begin <语句部分> end
 */
class SyntaxAnalyzer {
private:
    LexicalAnalyzer *lexicalAnalyzer;
    bool have_error = false;
    pair<string, int> next_word;  // 下一个等待匹配的词 <词, 类型>

public:
    explicit SyntaxAnalyzer(LexicalAnalyzer *lexicalAnalyzer) {
        this->lexicalAnalyzer = lexicalAnalyzer;
        this->lexicalAnalyzer->getNextWord(next_word.first, next_word.second);
    }

    void parseProgram() {
        cout << "【语】推导：<程序> -> <变量说明部分>;<语句部分>" << endl;
        parseExplainVars();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSection();
        if (have_error) cout << "[Syntax error]: Parse program failed!" << endl;
    }

    void parseExplainVars() {
        cout << "【语】推导：<变量说明部分> → int<标识符列表>" << endl;
        match_word(WORD_INT);
        parseIdentifierList();
    }

    void parseIdentifierList() {
        cout << "【语】推导：<标识符列表> → <标识符><标识符列表prime>" << endl;
        match_word(IDENTIFIER);
        parseIdentifierListPrime();
    }

    void parseIdentifierListPrime() {
        cout << "【语】推导：<标识符列表prime> → ,<标识符><标识符列表prime>|ε" << endl;
        if (next_word.second != SYMBOL_COMMA)
            return;
        match_word(SYMBOL_COMMA);
        match_word(IDENTIFIER);
        parseIdentifierListPrime();
    }

    void parseStatementSection() {
        cout << "【语】推导：<语句部分> → <语句>;<语句部分prime>" << endl;
        parseStatement();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSectionPrime();
    }

    void parseStatementSectionPrime() {
        cout << "【语】推导：<语句部分prime> → <语句>;<语句部分prime>|ε" << endl;
        if (next_word.second != IDENTIFIER && next_word.second != WORD_IF && next_word.second != WORD_WHILE)
            return;
        parseStatement();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSectionPrime();
    }

    void parseStatement() {
        cout << "【语】推导：<语句> → <赋值语句>|<条件语句>|<循环语句>" << endl;
        if (next_word.second == IDENTIFIER) parseAssignStatement();
        else if (next_word.second == WORD_IF) parseIfStatement();
        else if (next_word.second == WORD_WHILE) parseWhileStatement();
    }

    void parseAssignStatement() {
        cout << "【语】推导：<赋值语句> → <标识符>=<表达式>" << endl;
        match_word(IDENTIFIER);
        match_word(SYMBOL_ASSIGN);
        parseExpression();
    }

    void parseIfStatement() {
        cout << "【语】推导：<条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句>" << endl;
        match_word(WORD_IF);
        match_word(SYMBOL_LPAREN);
        parseCondition();
        match_word(SYMBOL_RPAREN);
        match_word(WORD_THEN);
        parseNestedStatement();
        match_word(SYMBOL_SEMICOLON);
        match_word(WORD_ELSE);
        parseNestedStatement();
    }

    void parseWhileStatement() {
        cout << "【语】推导：<循环语句> → while （<条件>） do <嵌套语句>" << endl;
        match_word(WORD_WHILE);
        match_word(SYMBOL_LPAREN);
        parseCondition();
        match_word(SYMBOL_RPAREN);
        match_word(WORD_DO);
        parseNestedStatement();
    }

    void parseExpression() {
        cout << "【语】推导：<表达式> → <项><表达式prime>" << endl;
        parseItem();
        parseExpressionPrime();
    }

    void parseExpressionPrime() {
        cout << "【语】推导：<表达式prime> → +<项><表达式prime>|ε" << endl;
        if (next_word.second != SYMBOL_ADD)
            return;
        match_word(SYMBOL_ADD);
        parseItem();
        parseExpressionPrime();
    }

    void parseItem() {
        cout << "【语】推导：<项> → <因子><项prime>" << endl;
        parseFactor();
        parseItemPrime();
    }

    void parseItemPrime() {
        cout << "【语】推导：<项prime> → *<因子><项prime>|ε" << endl;
        if (next_word.second != SYMBOL_MUL)
            return;
        match_word(SYMBOL_MUL);
        parseFactor();
        parseItemPrime();
    }

    void parseFactor() {
        cout << "【语】推导：<因子> → <标识符>|<常量>|(<表达式>)" << endl;
        if (next_word.second == IDENTIFIER)
            match_word(IDENTIFIER);
        else if (next_word.second == NUM)
            match_word(NUM);
        else {
            match_word(SYMBOL_LPAREN);
            parseExpression();
            match_word(SYMBOL_RPAREN);
        }
    }

    void parseCondition() {
        cout << "【语】推导：<条件> → <表达式><关系运算符><表达式>" << endl;
        parseExpression();
        if (SYMBOL_LT <= next_word.second && next_word.second <= SYMBOL_EQ)
            match_word(next_word.second);
        parseExpression();
    }

    void parseNestedStatement() {
        cout << "【语】推导：<嵌套语句> → <语句>|<复合语句>" << endl;
        if (next_word.second == WORD_BEGIN)
            parseCompoundStatement();
        else
            parseStatement();
    }

    void parseCompoundStatement() {
        cout << "【语】推导：<复合语句> → begin <语句部分> end" << endl;
        match_word(WORD_BEGIN);
        parseStatementSection();
        match_word(WORD_END);
    }

    void match_word(int expected_type) {
        if (next_word.second != expected_type) {
            cout << "[Syntax error]: Expect type " << expected_type << ", but got '" << next_word.first << "'" << endl;
            have_error = true;
        }
        lexicalAnalyzer->getNextWord(next_word.first, next_word.second);
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
    string source = readFile("in.txt") + "#";
    if (source.empty() || source == "#") {
        cout << "[Error]: Read Nothing!" << endl;
        return -1;
    }

    LexicalAnalyzer lexicalAnalyzer(source);
    IdentifierTable identifierTable;
    // lexicalAnalyzer.analyzeIdentifier(identifierTable);
    SyntaxAnalyzer syntaxAnalyzer(&lexicalAnalyzer);
    syntaxAnalyzer.parseProgram();

    // cout << "按回车继续" << endl;
    // system("read");
    return 0;
}
