//
// Created by 朱熙 on 2018/6/4.
//

#ifndef PARSERSERVER_LEXER_H
#define PARSERSERVER_LEXER_H

//关键字
enum Tag{
    IF = 256, ELSE, WHILE, WAIT, SET, MOVEJ, MOVEL,
    ID, NUM, REAL,
    INT, DOUBLE, SHORT,
    AND, OR, EQ, NE, GT, LT, GE, LE, TRUE, FALSE

};


class Lexer {

};


#endif //PARSERSERVER_LEXER_H
