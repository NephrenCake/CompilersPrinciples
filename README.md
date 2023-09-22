#  Njtech 2022 编译原理上机

1. 编程环境 Clion+Cygwin

2. 文件编码 UTF-8

3. 工程结构

   ```bash
   CompilerPrinciples/
       - Analyzer/
           - LexicalAnalyzer.hpp       # 词法分析
           - SyntaxAnalyzer.hpp        # 语法+语义分析
       - Component/
           - Utils.hpp                 # 类型定义与工具函数
           - Identifier.hpp            # 标识符类与临时变量
           - IdentifierTable.hpp       # 标识符表
           - CodeTable.hpp             # 中间代码表/四元式表
           - TempVarTable.hpp          # 临时变量表
       - main.cpp                      # 入口主程序
   ```

4. 编译运行该项目

   ```bash
   cmake -B build
   cmake --build build/
   ./build/CompilerPrinciples 
   ```
