//
// Created by 朱熙 on 2018/6/4.
//

#ifndef PARSERSERVER_PARSER_H
#define PARSERSERVER_PARSER_H

#include "../../include/global.h"
#include "./lexer/Lexer.h"
#include "./vm/Vm.h"
#include "../ServerManager/ServerManager.h"

class ServerManager;

/**
 * 脚本解释器
 * 通过词法分析器分析后，进行语法分析并生成相应语法树，由Vm执行
 */
class Parser {

public:
    Parser(ServerManager * pServerManager);
    ~Parser();

    /**
     * 解释脚本
     */
    void parse();

    /**
     * 设置解释脚本
     * @param str
     */
    void setScript(string str);

    /**
     * 保存脚本，用于下次启动时加载
     * @param str
     */
    void saveScript(string str);

private:
    /**
     * 词法分析器
     */
    Lexer m_lexer;
    /**
     * 当前语法类型
     */
    Token* m_curToken;
public:
    /**
     * 执行语法树的虚拟机
     */
    Vm m_vm;

    ServerManager *m_pServerManager;

private:
    /**
     * 判断m_curToken是否匹配
     * @param Tag Tag枚举表示
     * @return true：匹配 false：不匹配
     */
    bool match(int Tag);

    /**
     * 生成语法树节点
     * @return 语法节点
     */
    Node* stmt();
    /**
     * 生成if语法树节点
     * @return
     */
    Node* stmt_if();
    Node* stmt_wait();
    Node* stmt_while();
    Node* stmt_set();

    Node* stmt_Movej();
    Node* stmt_Movel();
    Node* stmt_Speedj();
    Node* stmt_Speedl();
    Node* stmt_Stopj();
    Node* stmt_Stopl();
    Node* stmt_TeachMode();
    Node* stmt_EndTeachMode();
    Node* stmt_MoveCamera();
    Node* stmt_Stop();
private:
    src::severity_channel_logger<severity_level, std::string> scl;


};


#endif //PARSERSERVER_PARSER_H
