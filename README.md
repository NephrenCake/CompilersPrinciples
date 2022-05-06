#  Njtech 2022 编译原理上机

1. 拒绝穷举逆天 `if else`，拒绝大量全局变量，拒绝用数组存集合，拒绝意义不明的命名方式，拒绝一万层嵌套，资瓷面向对象。

2. 编程环境 Clion+Cygwin

3. 工程结构

   ```bash
   Analyzer
   	- LexicalAnalyzer.hpp		# 词法分析
   	- SyntaxAnalyzer.hpp		# 语法+语义分析
   Component
   	- Utils.hpp					# 类型定义与工具函数
   	- Identifier.hpp			# 标识符类与临时变量
   	- IdentifierTable.hpp		# 标识符表
   	- CodeTable.hpp				# 中间代码表/四元式表
   	- TempVarTable.hpp			# 临时变量表
   ```

   
