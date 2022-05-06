#include "Analyzer/SyntaxAnalyzer.hpp"

int main() {
    string source = readFile("in.txt") + "#";
    cout << "源代码：\n" << source << endl;

    LexicalAnalyzer lexicalAnalyzer(source);
    IdentifierTable identifierTable;
    CodeTable codeTable;
    TempVarTable tempVarTable;

    cout << "\n开始编译~" << endl;
    SyntaxAnalyzer syntaxAnalyzer(&lexicalAnalyzer, &identifierTable, &codeTable, &tempVarTable);
    syntaxAnalyzer.parseProgram();

    cout << "\n标识符表：\n" << identifierTable.dumpTable();
    cout << "临时变量表：\n" << tempVarTable.dumpTable();
    cout << "四元式表：\n" << codeTable.dumpTable();

    writeFile("out.txt", codeTable.dumpTable());

    cout << "按回车继续" << endl;
    system("read");
    return 0;
}
