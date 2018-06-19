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

class Parser {

public:
    Parser(ServerManager * pServerManager);
    ~Parser();

    void parse();
    void setScript(string str);
    void saveScript(string str);

private:
    Lexer m_lexer;
    Token* m_curToken;
public:
    Vm m_vm;

    ServerManager *m_pServerManager;

private:
    bool match(int Tag);

    Node* stmt();
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
