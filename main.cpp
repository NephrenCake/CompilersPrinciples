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
 * <�������> �� if ��<����>�� then <Ƕ�����>; else <Ƕ�����>
 * <ѭ�����> �� while ��<����>�� do <Ƕ�����>
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
 * <Ƕ�����> �� <���>|<�������>
 * <��ĸ> �� a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
 * <����> �� 0|1|2|3|4|5|6|7|8|9
 */

/**
 * ����       ����
 * ��ʶ��      $��ĸ+����
 * �ֺ�       ;
 * ����       ,
 * ��ֵ��      =
 * ��ϵ�����    ==
 * �Ӻ�       +
 * ��ϵ�����    <
 * ��ϵ�����    >
 * ������      (
 * ������      )
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
                cout << "(��ʶ��, " << temp << "," << endl;
                resetCur();
                divert(ch);
            }
        else if (state == 1 && ch == ';') {  // 1->4
            identifierTable.addIdentifier(";");
            cout << "(�ֺ�, ;)" << endl;
            resetCur();
        } else if (state == 1 && ch == ',') {  // 1->5
            identifierTable.addIdentifier(",");
            cout << "(����, ,)" << endl;
            resetCur();
        } else if (state == 1 && ch == '=') {  // 1->6
            state = 6, temp += ch;
        } else if (state == 6) {  // 6->7 6->1
            if (ch == '=') {
                identifierTable.addIdentifier("==");
                cout << "(��ϵ�����==, ,)" << endl;
                resetCur();
            } else {
                identifierTable.addIdentifier("=");
                cout << "(��ֵ��=, =)" << endl;
                resetCur();
                divert(ch);
            }
        } else if (state == 1 && ch == '<') {  // 1->8
            identifierTable.addIdentifier("<");
            cout << "(��ϵ�����<, <)" << endl;
            resetCur();
        } else if (state == 1 && ch == '>') {  // 1->9
            identifierTable.addIdentifier(">");
            cout << "(��ϵ�����>, >)" << endl;
            resetCur();
        } else if (state == 1 && ch == '(') {  // 1->10
            identifierTable.addIdentifier("(");
            cout << "(������, ()" << endl;
            resetCur();
        } else if (state == 1 && ch == ')') {  // 1->11
            identifierTable.addIdentifier(")");
            cout << "(������, ))" << endl;
            resetCur();
        } else if (state == 1 && ch == 'i') {  // 1->12
            state = 12, temp += ch;
        } else if (state == 12 && ch == 'n') {  // 12->13
            state = 13, temp += ch;
        } else if (state == 13 && ch == 't') {  // 12->14
            identifierTable.addIdentifier("int");
            cout << "(�ؼ���int, int)" << endl;
            resetCur();
        } else if (state == 12 && ch == 'f') {  // 12->15
            identifierTable.addIdentifier("if");
            cout << "(�ؼ���if, if)" << endl;
            resetCur();
        } else if (state == 1 && ch == 't') {  // 1->16
            state = 16, temp += ch;
        } else if (state == 16 && ch == 'h') {  // 16->17
            state = 17, temp += ch;
        } else if (state == 17 && ch == 'e') {  // 17->18
            state = 18, temp += ch;
        } else if (state == 18 && ch == 'n') {  // 18->19
            identifierTable.addIdentifier("then");
            cout << "(�ؼ���then, then)" << endl;
            resetCur();
        } else if (state == 1 && ch == 'e') {  // 1->20
            state = 20, temp += ch;
        } else if (state == 20 && ch == 'l') {  // 20->21
            state = 21, temp += ch;
        } else if (state == 21 && ch == 's') {  // 21->22
            state = 22, temp += ch;
        } else if (state == 22 && ch == 'e') {  // 22->23
            identifierTable.addIdentifier("else");
            cout << "(�ؼ���else, else)" << endl;
            resetCur();
        } else if (state == 20 && ch == 'n') {  // 20->24
            state = 24, temp += ch;
        } else if (state == 24 && ch == 'd') {  // 24->25
            identifierTable.addIdentifier("end");
            cout << "(�ؼ���end, end)" << endl;
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
            cout << "(�ؼ���begin, begin)" << endl;
            resetCur();
        } else if (state == 1 && ch == '+') {  // todo 1->31
            identifierTable.addIdentifier("+");
            cout << "(�����+, +)" << endl;
            resetCur();
        } else if (state == 1 && ch == '-') {  // todo 1->32
            identifierTable.addIdentifier("-");
            cout << "(�����-, -)" << endl;
            resetCur();
        } else if (state == 1 && ch == '*') {  // todo 1->33
            identifierTable.addIdentifier("*");
            cout << "(�����*, *)" << endl;
            resetCur();
        } else if (state == 1 && ch == '/') {  // todo 1->34
            identifierTable.addIdentifier("/");
            cout << "(�����/, /)" << endl;
            resetCur();
        } else if (state == 1 && (ch >= '0' && ch <= '9')) {  // todo 1->35
            state = 35, temp += ch;
        } else if (state == 35) {  // todo 35->35 35->1
            if (ch >= '0' && ch <= '9')
                temp += ch;
            else {
                identifierTable.addIdentifier(temp);
                cout << "(����, " << temp << ")" << endl;
                resetCur();
                divert(ch);
            }
        } else if (state == 1 && ch == 'd') {  // todo 1->36
            state = 36, temp += ch;
        } else if (state == 36 && ch == 'o') {  // todo 36->37
            identifierTable.addIdentifier("do");
            cout << "(�ؼ���do, do)" << endl;
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
            cout << "(�ؼ���while, while)" << endl;
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

    cout << "���س�����" << endl;
    system("read");
    return 0;
}
