/**
 * @Author: NephrenCake
 * @Date: 2022/4/22
 * @Discription: 入口主程序
 */
#include "Analyzer/SyntaxAnalyzer.hpp"

int main() {
    string source = readFile("in.txt") + "#";
    cout << "源代码：\n" << source << endl;

    LexicalAnalyzer lexicalAnalyzer(source);
    IdentifierTable identifierTable;
    TempVarTable tempVarTable;
    CodeTable codeTable;

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
