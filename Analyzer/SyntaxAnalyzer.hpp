/**
 * @Auther: 刘亦飞
 * @Date: 2022/5/6
 * @Discription: 语法+语义分析
 */
#pragma once

#include "LexicalAnalyzer.hpp"
#include "../Component/CodeTable.hpp"
#include "../Component/TempVarTable.hpp"

// ********************语法语义分析部分********************
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
    IdentifierTable *identifierTable;
    CodeTable *codeTable;
    TempVarTable *tempVarTable;
    bool have_error = false;
    pair<string, int> next_word;  // 下一个等待匹配的词 <词, 类型>

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
        cout << "【语】推导：<程序> -> <变量说明部分>;<语句部分>" << endl;
        parseExplainVars();
        match_word(SYMBOL_SEMICOLON);
        parseStatementSection();
        if (have_error) cout << "[Syntax Error]: Parse program failed!" << endl;
        cout << "语法分析结束" << endl;
    }

    void parseExplainVars() {
        cout << "【语】推导：<变量说明部分> → int<标识符列表>" << endl;
        match_word(WORD_INT);
        parseIdentifierList(WORD_INT);
    }

    void parseIdentifierList(int identifierType) {
        cout << "【语】推导：<标识符列表> → <标识符><标识符列表prime>" << endl;
        if (next_word.second == IDENTIFIER) {
            identifierTable->addIdentifier(next_word.first);
            identifierTable->updateIdentifierType(next_word.first, getTypenameByID(identifierType));
            match_word(IDENTIFIER);
        }
        parseIdentifierListPrime(identifierType);
    }

    void parseIdentifierListPrime(int identifierType) {
        cout << "【语】推导：<标识符列表prime> → ,<标识符><标识符列表prime>|ε" << endl;
        if (next_word.second != SYMBOL_COMMA)
            return;
        match_word(SYMBOL_COMMA);
        if (next_word.second == IDENTIFIER) {
            string identifierName = next_word.first;
            if (identifierTable->existIdentifier(identifierName))
                cout << "[Syntax Error]: Position " << lexicalAnalyzer->getCur()
                     << ". Identifier '" << identifierName << "' is multi defined!" << endl;
            else {
                identifierTable->addIdentifier(identifierName);
                identifierTable->updateIdentifierType(identifierName, getTypenameByID(identifierType));
            }
            match_word(IDENTIFIER);
        }
        parseIdentifierListPrime(identifierType);
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
        // 由于只有 next_word.second == IDENTIFIER 才会调用该函数，故不需要再次判断
        string identifierName = next_word.first;
        string identifierType = identifierTable->getIdentifier(next_word.first).type;
        if (!identifierTable->existIdentifier(identifierName))
            cout << "[Syntax error]: Position " << lexicalAnalyzer->getCur()
                 << ". Identifier '" << identifierName << "' is not defined!" << endl;
        match_word(IDENTIFIER);
        match_word(SYMBOL_ASSIGN);
        Identifier E = parseExpression();
        // 赋值并产生四元式，若类型不符则报错
        if (E.type == identifierType) {
            codeTable->addQuaternary("=", E.name, "null", identifierName);
            identifierTable->updateIdentifierValue(identifierName, E.value);
        } else
            cout << "[Semantic error]: Position " << lexicalAnalyzer->getCur()
                 << ". When assigning a value to " << identifierName
                 << ". Expect type '" << identifierType
                 << "', but got '" << E.type << "'" << endl;
    }

    void parseIfStatement() {
        cout << "【语】推导：<条件语句> → if （<条件>） then <嵌套语句>; else <嵌套语句>" << endl;
        match_word(WORD_IF);
        match_word(SYMBOL_LPAREN);
        Identifier E = parseCondition();
        match_word(SYMBOL_RPAREN);
        match_word(WORD_THEN);

        codeTable->addQuaternary("jnz", E.name, "null", to_string(codeTable->NXQ() + 2));
        // 假出口
        int falseExitIndex = codeTable->NXQ();
        codeTable->addQuaternary("j", "null", "null", "0");

        parseNestedStatement();
        int exitIndex = codeTable->NXQ();
        codeTable->addQuaternary("j", "null", "null", "0");
        // 回填假出口
        codeTable->updateResultByIndex(falseExitIndex, to_string(codeTable->NXQ()));
        match_word(SYMBOL_SEMICOLON);
        match_word(WORD_ELSE);
        parseNestedStatement();
        // 回填真出口
        codeTable->updateResultByIndex(exitIndex, to_string(codeTable->NXQ()));
    }

    void parseWhileStatement() {
        cout << "【语】推导：<循环语句> → while （<条件>） do <嵌套语句>" << endl;
        match_word(WORD_WHILE);
        match_word(SYMBOL_LPAREN);

        int nextIndex = codeTable->NXQ();
        Identifier E = parseCondition();
        codeTable->addQuaternary("jnz", E.name, "null", to_string(codeTable->NXQ() + 2));
        // 假出口，莫忘回填
        int falseExitIndex = codeTable->NXQ();
        codeTable->addQuaternary("j", "null", "null", "0");

        match_word(SYMBOL_RPAREN);
        match_word(WORD_DO);
        parseNestedStatement();

        codeTable->addQuaternary("j", "null", "null", to_string(nextIndex));
        codeTable->updateResultByIndex(falseExitIndex, to_string(codeTable->NXQ()));
    }

    Identifier parseExpression() {
        cout << "【语】推导：<表达式> → <项><表达式prime>" << endl;
        Identifier E = parseItem();
        return parseExpressionPrime(E);
    }

    Identifier parseExpressionPrime(const Identifier &E1) {
        cout << "【语】推导：<表达式prime> → +<项><表达式prime>|ε" << endl;
        if (next_word.second != SYMBOL_ADD)
            return E1;

        match_word(SYMBOL_ADD);
        Identifier E2 = parseItem();
        Identifier E3 = E2 + E1;
        E3.name = tempVarTable->getNewTempVarName();  // E3是申请的一个新临时变量
        codeTable->addQuaternary("+", E1.name, E2.name, E3.name);
        return parseExpressionPrime(E3);
    }

    Identifier parseItem() {
        cout << "【语】推导：<项> → <因子><项prime>" << endl;
        Identifier E = parseFactor();
        return parseItemPrime(E);
    }

    Identifier parseItemPrime(const Identifier &E1) {
        cout << "【语】推导：<项prime> → *<因子><项prime>|ε" << endl;
        if (next_word.second != SYMBOL_MUL)
            return E1;

        match_word(SYMBOL_MUL);
        Identifier E2 = parseFactor();
        Identifier E3 = E2 * E1;
        E3.name = tempVarTable->getNewTempVarName();  // E3是申请的一个新临时变量
        codeTable->addQuaternary("*", E1.name, E2.name, E3.name);
        return parseItemPrime(E3);
    }

    Identifier parseFactor() {
        cout << "【语】推导：<因子> → <标识符>|<常量>|(<表达式>)" << endl;

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

    Identifier parseCondition() {
        cout << "【语】推导：<条件> → <表达式><关系运算符><表达式>" << endl;
        string op = getTypenameByID(next_word.second), temp;

        Identifier E1 = parseExpression();
        if (SYMBOL_LT <= next_word.second && next_word.second <= SYMBOL_EQ)
            match_word(next_word.second);
        Identifier E2 = parseExpression();

        if (op == "==") {
            temp = E1 == E2 ? "true" : "false";
        } else if (op == "!=") {
            temp = E1 != E2 ? "true" : "false";
        } else if (op == "<") {
            temp = E1 < E2 ? "true" : "false";
        } else if (op == ">") {
            temp = E1 > E2 ? "true" : "false";
        } else if (op == "<=") {
            temp = E1 <= E2 ? "true" : "false";
        } else if (op == ">=") {
            temp = E1 >= E2 ? "true" : "false";
        }
        Identifier E3 = {tempVarTable->getNewTempVarName(), "bool", temp};
        codeTable->addQuaternary(op, E1.name, E2.name, E3.name);
        return E3;
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
            cout << "[Syntax Error]: Position " << lexicalAnalyzer->getCur()
                 << ". Expect type '" << getTypenameByID(expected_type)
                 << "', but got '" << next_word.first << "'" << endl;
            have_error = true;
        }
        lexicalAnalyzer->getNextWord(next_word.first, next_word.second);
    }
};
