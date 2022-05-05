#include <bits/stdc++.h>

using namespace std;

// �ĸ�һ�����ࣨ�����֣����������ţ���ʶ�����������֡������ж������ࡣ
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

// �����ֵ��ʷ�����ӳ��
const unordered_map<string, int> reserved_words = {{"int", WORD_INT},
                                                   {"begin", WORD_BEGIN},
                                                   {"end", WORD_END},
                                                   {"if", WORD_IF},
                                                   {"else", WORD_ELSE},
                                                   {"then", WORD_THEN},
                                                   {"while", WORD_WHILE},
                                                   {"do", WORD_DO}};
// �������ŵ��ʷ�����ӳ�䣬�����ʷ��������ֵ���������
const unordered_map<string, pair<int, string>> reserved_symbols = {{"+",  {SYMBOL_ADD,       "�ӷ������"}},
                                                                   {"*",  {SYMBOL_MUL,       "�˷������"}},
                                                                   {"<",  {SYMBOL_LT,        "��ϵ�����"}},
                                                                   {">",  {SYMBOL_GT,        "��ϵ�����"}},
                                                                   {"<=", {SYMBOL_LE,        "��ϵ�����"}},
                                                                   {">=", {SYMBOL_GE,        "��ϵ�����"}},
                                                                   {"!=", {SYMBOL_NE,        "��ϵ�����"}},
                                                                   {"==", {SYMBOL_EQ,        "��ϵ�����"}},
                                                                   {",",  {SYMBOL_COMMA,     "�ָ�������"}},
                                                                   {";",  {SYMBOL_SEMICOLON, "�ָ����ֺ�"}},
                                                                   {"(",  {SYMBOL_LPAREN,    "������"}},
                                                                   {")",  {SYMBOL_RPAREN,    "������"}},
                                                                   {"=",  {SYMBOL_ASSIGN,    "��ֵ��"}}};
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
     * ���������еı�ʶ�������������ʶ����
     * @param identifierTable ��ʶ����
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
     * ��˳���ȡ��һ���ʡ�����ȡʧ�ܣ�����ı䴫��ı�����
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
        int len = 0, lv1_type = start_type(source[ptr]), lv2_type = 0;
        string sub_word;
        switch (lv1_type) {
            case WORD:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!is_reserved_words(sub_word) &&  // ��������ǹؼ��֣�����Ҫ����ƴ��һ���ַ�
                         !ptr_arrive_end(ptr + len) &&  // ��Խ��
                         is_letter(source[ptr + len]));  // �ַ��Ϸ�
                if (!is_reserved_words(sub_word)) {
                    cout << "[Lexical Error]: Word at " << ptr << ", " << sub_word <<
                         " is not a reserved next_word! Will ignore it!" << endl;
                    ptr += len;
                    return false;
                }  // ����ƥ��������Ƿ����
                lv2_type = reserved_words.find(sub_word)->second;
                cout << "���ʡ�(������" << sub_word << ", " << sub_word << ")" << endl;
                break;

            case NUM:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         is_digit(source[ptr + len]));  // �ַ��Ϸ�������ƴ�ɸ�������������
                cout << "���ʡ�(����, " << sub_word << ")" << endl;
                break;

            case SYMBOL:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         source[ptr + len] == '=');  // �ַ��Ϸ������� "!=", ">=", "<=", "=="
                lv2_type = reserved_symbols.find(sub_word)->second.first;
                cout << "���ʡ�(" << reserved_symbols.find(sub_word)->second.second << ", " << sub_word << ")" << endl;
                break;

            case IDENTIFIER:
                do {
                    len++;
                    sub_word = source.substr(ptr, len);
                } while (!ptr_arrive_end(ptr + len) &&  // ��Խ��
                         (is_letter(source[ptr + len]) ||
                          is_digit(source[ptr + len])));  // ��������$��ͷ��������ĸ���������
                cout << "���ʡ�(��ʶ��, " << sub_word << ")" << endl;
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

class SyntaxAnalyzer {
private:
    LexicalAnalyzer *lexicalAnalyzer;
    bool have_error = false;
    pair<string, int> next_word;  // ��һ���ȴ�ƥ��Ĵ�

public:
    SyntaxAnalyzer(LexicalAnalyzer *lexicalAnalyzer) : lexicalAnalyzer(lexicalAnalyzer) {}

/**
 * <����> �� <����˵������>;<��䲿��>
 * <����˵������> �� int<��ʶ���б�>
 * <��ʶ���б�> �� <��ʶ��><��ʶ���б�prime>
 * <��ʶ���б�prime> �� ,<��ʶ��><��ʶ���б�prime>|��
 * <��䲿��> �� <���>;<��䲿��prime>
 * <��䲿��prime> �� <���>;<��䲿��prime>|��
 * <���> �� <��ֵ���>|<�������>|<ѭ�����>|
 * <��ֵ���> �� <��ʶ��>=<���ʽ>
 * <�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����>
 * <ѭ�����> �� while ��<����>�� do <Ƕ�����>
 * <���ʽ> �� <��><���ʽprime>
 * <���ʽprime> �� +<��><���ʽprime>|��
 * <��> �� <����><��prime>
 * <��prime> �� *<����><��prime>|��
 * <����> �� <��ʶ��>|<����>|(<���ʽ>)
 * <����> �� <���ʽ><��ϵ�����><���ʽ>
 * <Ƕ�����> �� <���>|<�������>
 * <�������> �� begin <��䲿��> end
 */

    bool parseProgram() {
        cout << "����Ƶ���<����> -> <����˵������>;<��䲿��>" << endl;
        parseExplainVars();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSection();
        return true;
    }

    bool parseExplainVars() {
        cout << "����Ƶ���<����˵������> �� int<��ʶ���б�>" << endl;
        match_word(WORD_INT);
        parseIdentifierList();
        return true;
    }

    bool parseIdentifierList() {
        cout << "����Ƶ���<��ʶ���б�> �� <��ʶ��><��ʶ���б�prime>" << endl;
        match_word(IDENTIFIER);
        parseIdentifierListPrime();
        return true;
    }

    bool parseIdentifierListPrime() {
        cout << "����Ƶ���<��ʶ���б�prime> �� ,<��ʶ��><��ʶ���б�prime>|��" << endl;
        match_word(SYMBOL_COMMA);
        match_word(IDENTIFIER);
        return true;
    }

    bool parseStatementSection() {
        cout << "����Ƶ���<��䲿��> �� <���>;<��䲿��prime>" << endl;
        return true;
    }

    bool parseStatementSectionPrime() {
        cout << "����Ƶ���<��䲿��prime> �� <���>;<��䲿��prime>|��" << endl;
        return true;
    }

    bool parseStatement() {
        cout << "����Ƶ���<���> �� <��ֵ���>|<�������>|<ѭ�����>|" << endl;
        return true;
    }

    bool parseAssignStatement() {
        cout << "����Ƶ���<��ֵ���> �� <��ʶ��>=<���ʽ>" << endl;
        return true;
    }

    bool parseIfStatement() {
        cout << "����Ƶ���<�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����>" << endl;
        return true;
    }

    bool parseWhileStatement() {
        cout << "����Ƶ���<ѭ�����> �� while ��<����>�� do <Ƕ�����>" << endl;
        return true;
    }

    bool parseExpression() {
        cout << "����Ƶ���<���ʽ> �� <��><���ʽprime>" << endl;
        return true;
    }

    bool parseExpressionPrime() {
        cout << "����Ƶ���<���ʽprime> �� <�ӷ������><��><���ʽprime>|��" << endl;
        return true;
    }

    bool parseItem() {
        cout << "����Ƶ���<���ʽprime> �� <�ӷ������><��><���ʽprime>|��" << endl;
        return true;
    }


    void match_word(int expected_type) {
        if (next_word.second != expected_type) {
            cout << "[Syntax error]: Expect ";
            // if (expected_type == 0 || expected_type == 24) {
            //     cout << error_check[expected_type] << " \"" << next_word.value << "\"";
            // } else {
            //     cout << "\"" << error_check[expected_type] << "\"";
            // }
            // cout << ", but get \"" << next_word.value << "\"." << endl;
            have_error = true;
        } else {
            cout << "success" << endl;

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
    lexicalAnalyzer.analyzeIdentifier(identifierTable);
    SyntaxAnalyzer syntaxAnalyzer(&lexicalAnalyzer);
    // syntaxAnalyzer.parseProgram();

    // cout << "���س�����" << endl;
    // system("read");
    return 0;
}
