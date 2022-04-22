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
 * <条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句>
 * <循环语句> → while （<条件>） do <嵌套语句>
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
 * <嵌套语句> → <语句>|<复合语句>
 * <字母> → a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
 * <数字> → 0|1|2|3|4|5|6|7|8|9
 */

/**
 * 类型       符号
 * 标识符      $字母+数字
 * 分号       ;
 * 逗号       ,
 * 赋值号      =
 * 关系运算符    ==
 * 加号       +
 * 关系运算符    <
 * 关系运算符    >
 * 左括号      (
 * 右括号      )
 * int        int
 * if         if
 * then       then
 * else       else
 * begin      begin
 * end        end
 */

struct Identifier {
    string name;
    string type;
    string value;
};

class IdentifierTable {
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
    int ptr{}, state{};
    string temp, source;

public:
    IdentifierTable identifierTable;

    explicit LexicalAnalyzer(string &source) {
        this->source = source + "#";
    }

    void resetCur() {
        ptr = 0, state = 1, temp = "";
    }

    void startAnalyze() {
        identifierTable.clearTable();
        resetCur();
        for (auto &ch: source) {
            if (ch == '#') break;
            if (ch == ' ') continue;
            divert(ch);
        }
    }

    bool divert(char ch) {
        if (state == 1 && ch == '$')  // 1->2
            state = 2, temp += ch;
        else if (state == 2 || state == 3)  // 2->3 3->3 3->1
            if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9')
                state = 3, temp += ch;
            else {
                identifierTable.addIdentifier(temp);
                cout << "(标识符, " << temp << "," << endl;
                resetCur();
                divert(ch);
            }
        else if (state == 1 && ch == ';') {  // 1->4
            identifierTable.addIdentifier(";");
            cout << "(分号, ;)" << endl;
            resetCur();
        } else if (state == 1 && ch == ',') {  // 1->5
            identifierTable.addIdentifier(",");
            cout << "(逗号, ,)" << endl;
            resetCur();
        } else if (state == 1 && ch == '=') {  // 1->6
            state = 6, temp += ch;
        } else if (state == 6) {  // 6->7 6->1
            if (ch == '=') {
                identifierTable.addIdentifier("==");
                cout << "(关系运算符==, ,)" << endl;
                resetCur();
            } else {
                identifierTable.addIdentifier("=");
                cout << "(赋值号=, =)" << endl;
                resetCur();
                divert(ch);
            }
        } else if (state == 1 && ch == '<') {  // 1->8
            identifierTable.addIdentifier("<");
            cout << "(关系运算符<, <)" << endl;
            resetCur();
        } else if (state == 1 && ch == '>') {  // 1->9
            identifierTable.addIdentifier(">");
            cout << "(关系运算符>, >)" << endl;
            resetCur();
        } else if (state == 1 && ch == '(') {  // 1->10
            identifierTable.addIdentifier("(");
            cout << "(左括号, ()" << endl;
            resetCur();
        } else if (state == 1 && ch == ')') {  // 1->11
            identifierTable.addIdentifier(")");
            cout << "(右括号, ))" << endl;
            resetCur();
        } else if (state == 1 && ch == 'i') {  // 1->12
            state = 12, temp += ch;
        } else if (state == 12 && ch == 'n') {  // 12->13
            state = 13, temp += ch;
        } else if (state == 13 && ch == 't') {  // 12->14
            identifierTable.addIdentifier("int");
            cout << "(关键字int, int)" << endl;
            resetCur();
        } else if (state == 12 && ch == 'f') {  // 12->15
            identifierTable.addIdentifier("if");
            cout << "(关键字if, if)" << endl;
            resetCur();
        } else if (state == 1 && ch == 't') {  // 1->16
            state = 16, temp += ch;
        } else if (state == 16 && ch == 'h') {  // 16->17
            state = 17, temp += ch;
        } else if (state == 17 && ch == 'e') {  // 17->18
            state = 18, temp += ch;
        } else if (state == 18 && ch == 'n') {  // 18->19
            identifierTable.addIdentifier("then");
            cout << "(关键字then, then)" << endl;
            resetCur();
        } else if (state == 1 && ch == 'e') {  // 1->20
            state = 20, temp += ch;
        } else if (state == 20 && ch == 'l') {  // 20->21
            state = 21, temp += ch;
        } else if (state == 21 && ch == 's') {  // 21->22
            state = 22, temp += ch;
        } else if (state == 22 && ch == 'e') {  // 22->23
            identifierTable.addIdentifier("else");
            cout << "(关键字else, else)" << endl;
            resetCur();
        } else if (state == 20 && ch == 'n') {  // 20->24
            state = 24, temp += ch;
        } else if (state == 24 && ch == 'd') {  // 24->25
            identifierTable.addIdentifier("end");
            cout << "(关键字end, end)" << endl;
            resetCur();
        } else if (state == 1 && ch == 'b') {  // 1->26
            state = 26, temp += ch;
        } else if (state == 26 && ch == 'e') {  // 26->27
            state = 27, temp += ch;
        } else if (state == 27 && ch == 'g') {  // 27->28
            state = 28, temp += ch;
        } else if (state == 28 && ch == 'i') {  // 28->29
            state = 29, temp += ch;
        } else if (state == 29 && ch == 'n') {  // 29->30
            identifierTable.addIdentifier("begin");
            cout << "(关键字begin, begin)" << endl;
            resetCur();
        } else if (state == 1 && ch == '+') {  // todo 1->31
            identifierTable.addIdentifier("+");
            cout << "(运算符+, +)" << endl;
            resetCur();
        } else if (state == 1 && ch == '-') {  // todo 1->32
            identifierTable.addIdentifier("-");
            cout << "(运算符-, -)" << endl;
            resetCur();
        } else if (state == 1 && ch == '*') {  // todo 1->33
            identifierTable.addIdentifier("*");
            cout << "(运算符*, *)" << endl;
            resetCur();
        } else if (state == 1 && ch == '/') {  // todo 1->34
            identifierTable.addIdentifier("/");
            cout << "(运算符/, /)" << endl;
            resetCur();
        } else if (state == 1 && (ch >= '0' && ch <= '9')) {  // todo 1->35
            state = 35, temp += ch;
        } else if (state == 35) {  // todo 35->35 35->1
            if (ch >= '0' && ch <= '9')
                temp += ch;
            else {
                identifierTable.addIdentifier(temp);
                cout << "(常数, " << temp << ")" << endl;
                resetCur();
                divert(ch);
            }
        } else if (state == 1 && ch == 'd') {  // todo 1->36
            state = 36, temp += ch;
        } else if (state == 36 && ch == 'o') {  // todo 36->37
            identifierTable.addIdentifier("do");
            cout << "(关键字do, do)" << endl;
            resetCur();
        } else if (state == 1 && ch == 'w') {  // todo 1->36
            state = 36, temp += ch;
        } else if (state == 36 && ch == 'h') {  // todo 36->37
            state = 37, temp += ch;
        } else if (state == 37 && ch == 'i') {  // todo 37->38
            state = 38, temp += ch;
        } else if (state == 38 && ch == 'l') {  // todo 38->39
            state = 39, temp += ch;
        } else if (state == 39 && ch == 'e') {  // todo 39->40
            identifierTable.addIdentifier("while");
            cout << "(关键字while, while)" << endl;
            resetCur();
        } else {
            cout << "unknown " << ch << endl;
            resetCur();
        }
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
    string source = readFile("source.txt");
    cout << "==========Source Code==========\n" << source << endl << "==========Source Code==========" << endl;

    LexicalAnalyzer lexicalAnalyzer(source);
    lexicalAnalyzer.startAnalyze();
    cout << lexicalAnalyzer.identifierTable.dumpTable();

    cout << "按回车继续" << endl;
    system("read");
    return 0;
}
