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

// �����ּ��ϣ��Լ����ʷ�����ӳ��
const unordered_map<string, int> reserved_words = {{"int", WORD_INT},
                                                   {"begin", WORD_BEGIN},
                                                   {"end", WORD_END},
                                                   {"if", WORD_IF},
                                                   {"else", WORD_ELSE},
                                                   {"then", WORD_THEN},
                                                   {"while", WORD_WHILE},
                                                   {"do", WORD_DO}};
// �������ż��ϣ��Լ����ʷ�����ӳ�䣬�����ʷ��������ֵ���������
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

// ��ʶ����
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

// �ʷ���������
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
            if (source[ptr] != '#')
                cout << "[Warning]: no words any more!" << endl;
            res = '#';
            type = -1;
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
                    cout << "[Lexical Error]: Word at " << ptr << ", '" << sub_word <<
                         "' is not a reserved next_word! Will ignore it!" << endl;
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

// �﷨��������
/**
 * <����> �� <����˵������>;<��䲿��>
 * <����˵������> �� int<��ʶ���б�>
 * <��ʶ���б�> �� <��ʶ��><��ʶ���б�prime>
 * <��ʶ���б�prime> �� ,<��ʶ��><��ʶ���б�prime>|��
 * <��䲿��> �� <���>;<��䲿��prime>
 * <��䲿��prime> �� <���>;<��䲿��prime>|��
 * <���> �� <��ֵ���>|<�������>|<ѭ�����>
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
class SyntaxAnalyzer {
private:
    LexicalAnalyzer *lexicalAnalyzer;
    bool have_error = false;
    pair<string, int> next_word;  // ��һ���ȴ�ƥ��Ĵ� <��, ����>

public:
    explicit SyntaxAnalyzer(LexicalAnalyzer *lexicalAnalyzer) {
        this->lexicalAnalyzer = lexicalAnalyzer;
        this->lexicalAnalyzer->getNextWord(next_word.first, next_word.second);
    }

    void parseProgram() {
        cout << "����Ƶ���<����> -> <����˵������>;<��䲿��>" << endl;
        parseExplainVars();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSection();
        if (have_error) cout << "[Syntax error]: Parse program failed!" << endl;
    }

    void parseExplainVars() {
        cout << "����Ƶ���<����˵������> �� int<��ʶ���б�>" << endl;
        match_word(WORD_INT);
        parseIdentifierList();
    }

    void parseIdentifierList() {
        cout << "����Ƶ���<��ʶ���б�> �� <��ʶ��><��ʶ���б�prime>" << endl;
        match_word(IDENTIFIER);
        parseIdentifierListPrime();
    }

    void parseIdentifierListPrime() {
        cout << "����Ƶ���<��ʶ���б�prime> �� ,<��ʶ��><��ʶ���б�prime>|��" << endl;
        if (next_word.second != SYMBOL_COMMA)
            return;
        match_word(SYMBOL_COMMA);
        match_word(IDENTIFIER);
        parseIdentifierListPrime();
    }

    void parseStatementSection() {
        cout << "����Ƶ���<��䲿��> �� <���>;<��䲿��prime>" << endl;
        parseStatement();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSectionPrime();
    }

    void parseStatementSectionPrime() {
        cout << "����Ƶ���<��䲿��prime> �� <���>;<��䲿��prime>|��" << endl;
        if (next_word.second != IDENTIFIER && next_word.second != WORD_IF && next_word.second != WORD_WHILE)
            return;
        parseStatement();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSectionPrime();
    }

    void parseStatement() {
        cout << "����Ƶ���<���> �� <��ֵ���>|<�������>|<ѭ�����>" << endl;
        if (next_word.second == IDENTIFIER) parseAssignStatement();
        else if (next_word.second == WORD_IF) parseIfStatement();
        else if (next_word.second == WORD_WHILE) parseWhileStatement();
    }

    void parseAssignStatement() {
        cout << "����Ƶ���<��ֵ���> �� <��ʶ��>=<���ʽ>" << endl;
        match_word(IDENTIFIER);
        match_word(SYMBOL_ASSIGN);
        parseExpression();
    }

    void parseIfStatement() {
        cout << "����Ƶ���<�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����>" << endl;
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
        cout << "����Ƶ���<ѭ�����> �� while ��<����>�� do <Ƕ�����>" << endl;
        match_word(WORD_WHILE);
        match_word(SYMBOL_LPAREN);
        parseCondition();
        match_word(SYMBOL_RPAREN);
        match_word(WORD_DO);
        parseNestedStatement();
    }

    void parseExpression() {
        cout << "����Ƶ���<���ʽ> �� <��><���ʽprime>" << endl;
        parseItem();
        parseExpressionPrime();
    }

    void parseExpressionPrime() {
        cout << "����Ƶ���<���ʽprime> �� +<��><���ʽprime>|��" << endl;
        if (next_word.second != SYMBOL_ADD)
            return;
        match_word(SYMBOL_ADD);
        parseItem();
        parseExpressionPrime();
    }

    void parseItem() {
        cout << "����Ƶ���<��> �� <����><��prime>" << endl;
        parseFactor();
        parseItemPrime();
    }

    void parseItemPrime() {
        cout << "����Ƶ���<��prime> �� *<����><��prime>|��" << endl;
        if (next_word.second != SYMBOL_MUL)
            return;
        match_word(SYMBOL_MUL);
        parseFactor();
        parseItemPrime();
    }

    void parseFactor() {
        cout << "����Ƶ���<����> �� <��ʶ��>|<����>|(<���ʽ>)" << endl;
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
        cout << "����Ƶ���<����> �� <���ʽ><��ϵ�����><���ʽ>" << endl;
        parseExpression();
        if (SYMBOL_LT <= next_word.second && next_word.second <= SYMBOL_EQ)
            match_word(next_word.second);
        parseExpression();
    }

    void parseNestedStatement() {
        cout << "����Ƶ���<Ƕ�����> �� <���>|<�������>" << endl;
        if (next_word.second == WORD_BEGIN)
            parseCompoundStatement();
        else
            parseStatement();
    }

    void parseCompoundStatement() {
        cout << "����Ƶ���<�������> �� begin <��䲿��> end" << endl;
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

    // cout << "���س�����" << endl;
    // system("read");
    return 0;
}
