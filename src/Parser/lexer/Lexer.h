//
// Created by 朱熙 on 2018/6/4.
//

#ifndef PARSERSERVER_LEXER_H
#define PARSERSERVER_LEXER_H

#include "../../../include/global.h"

//token type
enum Tag{
    tag_IF = 128, tag_ELSE, tag_WHILE, tag_WAIT, tag_SET, tag_STOP,
    tag_MOVEJ, tag_MOVEL, tag_SPEEDJ, tag_SPEEDL, tag_STOPJ, tag_STOPL, tag_TEACH_MODE, tag_END_TEACH_MODE, tag_MOVECAMERA,
    tag_ID, tag_STRING,
    tag_INT, tag_DOUBLE, tag_SHORT,
    EQ, NE, GT, LT, GE, LE, tag_UNKNOW
};


struct Token{
    int Tag;		//种别码
    Token(){Tag = tag_UNKNOW;}
    Token(int Tag) :Tag(Tag){}
};

struct Word :Token{
    string word;	//字符
    Word(){
        this->Tag = tag_UNKNOW;
        this->word = "";
    }
    Word(int Tag, string word) :Token(Tag){
        this->word = word;
    }
};

// 短整数
struct Short :Token{
    short value;		//数字
    Short() : value(-1){ Tag = tag_SHORT; }
    Short(int val) : value(val){ Tag = tag_SHORT; }
};

// 整数
struct Integer :Token{
    int value;		//数字
    Integer() : value(-1){ Tag = tag_INT; }
    Integer(int val) : value(val){ Tag = tag_INT; }
};

// 浮点数
struct Double :Token{
    double value;		//数字
    Double() : value(-1){ Tag = tag_DOUBLE; }
    Double(double val) : value(val){ Tag = tag_DOUBLE; }
};

class Lexer {

public:
    Lexer();
    ~Lexer();

    Token* next();

    int m_column;


    void setScript(string script) {m_strScript = script;}
    string getScript() {return m_strScript;}


private:
    Token* match_id();
    Token* match_number();
    Token* match_string();
    Token* match_other();

private:
    string m_strScript;             //脚本

    map<string, Word*> m_words;       //关键字

private:
    src::severity_channel_logger<severity_level, std::string> scl;



};


#endif //PARSERSERVER_LEXER_H
