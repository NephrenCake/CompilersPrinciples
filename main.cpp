#include <bits/stdc++.h>

using namespace std;

// ********************���Ͷ���********************
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
const unordered_map<string, int> reserved_words = {{"int",   WORD_INT},
                                                   {"begin", WORD_BEGIN},
                                                   {"end",   WORD_END},
                                                   {"if",    WORD_IF},
                                                   {"else",  WORD_ELSE},
                                                   {"then",  WORD_THEN},
                                                   {"while", WORD_WHILE},
                                                   {"do",    WORD_DO}};
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
                    return i.second.second;
        case 4:
            return "Identifier";
        default:
            return "UnknownType";
    }
}

// ********************��ʶ����********************
class Identifier {
public:
    string name, type, value;

    static inline bool isAbleToCal(const Identifier &i1, const Identifier &i2) {
        return (i1.type == getTypenameByID(WORD_INT) || i1.type == getTypenameByID(NUM)) &&
               (i2.type == getTypenameByID(WORD_INT) || i2.type == getTypenameByID(NUM));
    }

    Identifier operator+(const Identifier &i2) {
        if (isAbleToCal(*this, i2))
            return {"", this->type, to_string(stoi(this->value) + stoi(i2.value))};
        else {
            cout << "[Semantic error]: Can only use operator+ on two INT" << endl;
            return {"", "", ""};
        }
    }

    Identifier operator*(const Identifier &i2) {
        if (isAbleToCal(*this, i2))
            return {"", this->type, to_string(stoi(this->value) * stoi(i2.value))};
        else {
            cout << "[Semantic error]: Can only use operator* on two INT" << endl;
            return {"", "", ""};
        }
    }
};

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

// ********************��Ԫʽ��********************
class CodeTable {
private:
    struct Quaternary {
        string op, arg1, arg2, result;
    };

    vector<Quaternary> quaternaries;

public:
    bool addQuaternary(const string &op, const string &arg1, const string &arg2, const string &result) {
        quaternaries.push_back({op, arg1, arg2, result});
        cout << "(" + op + ", " + arg1 + ", " + arg2 + ", " + result + ")" << endl;
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
            res += "(" + to_string(i) + ") ("
                    + item.op + ", " + item.arg1 + ", " + item.arg2 + ", " + item.result + ")\n";
        }
        return res;
    }
};

// ********************��ʱ������********************
class TempVarTable {
private:
    vector<string> table;
public:
    string getNewTempVarName() {
        string name = "T" + to_string(table.size() + 1);
        table.push_back(name);
        return name;
    }

    string dumpTable() {
        string res = "[";
        for (auto &item: table)
            res += item + ", ";
        return res + "]\n";
    }
};

// ********************�ʷ���������********************
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
            if (source[ptr] != '#') {
                cout << "[Warning]: no words any more!" << endl;
                return false;
            } else {
                res = '#';
                type = -1;
                cout << "���ʡ�(#, #)" << endl;
                cout << "�ʷ���������" << endl;
                return true;
            }
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

// ********************�﷨�����������********************
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
    IdentifierTable *identifierTable;
    CodeTable *codeTable;
    TempVarTable *tempVarTable;
    bool have_error = false;
    pair<string, int> next_word;  // ��һ���ȴ�ƥ��Ĵ� <��, ����>

public:
    explicit SyntaxAnalyzer(LexicalAnalyzer *lexicalAnalyzer,
                            IdentifierTable *identifierTable,
                            CodeTable *codeTable,
                            TempVarTable *tempVarTable) {
        this->lexicalAnalyzer = lexicalAnalyzer;
        this->identifierTable = identifierTable;
        this->codeTable = codeTable;
        this->tempVarTable = tempVarTable;
        this->lexicalAnalyzer->getNextWord(next_word.first, next_word.second);
    }

    void parseProgram() {
        cout << "����Ƶ���<����> -> <����˵������>;<��䲿��>" << endl;
        parseExplainVars();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSection();
        if (have_error) cout << "[Syntax error]: Parse program failed!" << endl;
        cout << "�﷨��������" << endl;
    }

    void parseExplainVars() {
        cout << "����Ƶ���<����˵������> �� int<��ʶ���б�>" << endl;
        match_word(WORD_INT);
        parseIdentifierList(WORD_INT);
    }

    void parseIdentifierList(int identifierType) {
        cout << "����Ƶ���<��ʶ���б�> �� <��ʶ��><��ʶ���б�prime>" << endl;
        if (next_word.second == IDENTIFIER) {
            identifierTable->addIdentifier(next_word.first);
            identifierTable->updateIdentifierType(next_word.first, getTypenameByID(identifierType));
            match_word(IDENTIFIER);
        }
        parseIdentifierListPrime(identifierType);
    }

    void parseIdentifierListPrime(int identifierType) {
        cout << "����Ƶ���<��ʶ���б�prime> �� ,<��ʶ��><��ʶ���б�prime>|��" << endl;
        if (next_word.second != SYMBOL_COMMA)
            return;
        match_word(SYMBOL_COMMA);
        if (next_word.second == IDENTIFIER) {
            identifierTable->addIdentifier(next_word.first);
            identifierTable->updateIdentifierType(next_word.first, getTypenameByID(identifierType));
            match_word(IDENTIFIER);
        }
        parseIdentifierListPrime(identifierType);
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
        // ����ֻ�� next_word.second == IDENTIFIER �Ż���øú������ʲ���Ҫ�ٴ��ж�
        string identifierName = next_word.first;
        string identifierType = identifierTable->getIdentifier(next_word.first).type;
        match_word(IDENTIFIER);
        match_word(SYMBOL_ASSIGN);
        Identifier E = parseExpression();
        // ��ֵ��������Ԫʽ�������Ͳ����򱨴�
        if (E.type == identifierType) {
            codeTable->addQuaternary("=", E.name, "null", identifierName);
            identifierTable->updateIdentifierValue(identifierName, E.value);
        } else
            cout << "[Semantic error]: When assigning a value to " << identifierName
                 << ". Expect type '" << identifierType
                 << "', but got '" << E.type << "'" << endl;
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

    Identifier parseExpression() {
        cout << "����Ƶ���<���ʽ> �� <��><���ʽprime>" << endl;
        Identifier E = parseItem();
        return parseExpressionPrime(E);
    }

    Identifier parseExpressionPrime(const Identifier &E1) {
        cout << "����Ƶ���<���ʽprime> �� +<��><���ʽprime>|��" << endl;
        if (next_word.second != SYMBOL_ADD)
            return E1;

        match_word(SYMBOL_ADD);
        Identifier E2 = parseItem();
        Identifier E3 = E2 + E1;
        E3.name = tempVarTable->getNewTempVarName();  // E3�������һ������ʱ����
        codeTable->addQuaternary("+", E1.name, E2.name, E3.name);
        return parseExpressionPrime(E3);
    }

    Identifier parseItem() {
        cout << "����Ƶ���<��> �� <����><��prime>" << endl;
        Identifier E = parseFactor();
        return parseItemPrime(E);
    }

    Identifier parseItemPrime(const Identifier &E1) {
        cout << "����Ƶ���<��prime> �� *<����><��prime>|��" << endl;
        if (next_word.second != SYMBOL_MUL)
            return E1;

        match_word(SYMBOL_MUL);
        Identifier E2 = parseFactor();
        Identifier E3 = E2 * E1;
        E3.name = tempVarTable->getNewTempVarName();  // E3�������һ������ʱ����
        codeTable->addQuaternary("*", E1.name, E2.name, E3.name);
        return parseItemPrime(E3);
    }

    Identifier parseFactor() {
        cout << "����Ƶ���<����> �� <��ʶ��>|<����>|(<���ʽ>)" << endl;

        Identifier E;
        if (next_word.second == IDENTIFIER) {
            E = identifierTable->getIdentifier(next_word.first);
            match_word(IDENTIFIER);
        } else if (next_word.second == NUM) {
            E = {next_word.first, getTypenameByID(WORD_INT), next_word.first};
            match_word(NUM);
        } else {
            match_word(SYMBOL_LPAREN);
            E = parseExpression();
            match_word(SYMBOL_RPAREN);
        }
        return E;
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

// ********************utils********************
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
    CodeTable codeTable;
    TempVarTable tempVarTable;

    SyntaxAnalyzer syntaxAnalyzer(&lexicalAnalyzer, &identifierTable, &codeTable, &tempVarTable);
    syntaxAnalyzer.parseProgram();

    cout << "\n��ʶ����\n" << identifierTable.dumpTable();
    cout << "��ʱ������\n" << tempVarTable.dumpTable();
    cout << "��Ԫʽ��\n" << codeTable.dumpTable();
    // cout << "���س�����" << endl;
    // system("read");
    return 0;
}
