//
// Created by 朱熙 on 2018/6/4.
//

#include "Lexer.h"
#include <cctype>

Lexer::Lexer() : m_column(0), scl(keywords::channel = "lexer_class") {
    m_words["if"] = new Word(tag_IF, "if");
    m_words["while"] = new Word(tag_WHILE, "while");
    m_words["wait"] = new Word(tag_WAIT, "wait");
    m_words["set"] = new Word(tag_SET, "set");
    m_words["stop"] = new Word(tag_STOP, "stop");
    m_words["movej"] = new Word(tag_MOVEJ, "movej");
    m_words["movel"] = new Word(tag_MOVEL, "movel");
    m_words["speedj"] = new Word(tag_SPEEDJ, "speedj");
    m_words["speedl"] = new Word(tag_SPEEDL, "speedl");
    m_words["stopj"] = new Word(tag_STOPJ, "stopj");
    m_words["stopl"] = new Word(tag_STOPL, "stopl");
    m_words["teach_mode"] = new Word(tag_TEACH_MODE, "teach_mode");
    m_words["end_teach_mode"] = new Word(tag_END_TEACH_MODE, "end_teach_mode");
    m_words["MoveCamera"] = new Word(tag_MOVECAMERA, "movecomera");
    m_words["set_digital_out"] = new Word(tag_SET_DIGITAL_OUT, "set_digital_out");
}

Lexer::~Lexer() {
    m_words.clear();
}

Token *Lexer::next() {
    while (m_column < m_strScript.length()) {
        //跳过 #def
        if (m_strScript[m_column] == '#') {
            m_column++;
            while(isalpha(m_strScript[m_column])) {
                m_column++;
            }
        } else if (m_strScript[m_column] == ' ') {
            m_column++;
        } else if (m_strScript[m_column] == '\\' && m_strScript[m_column+1] == 'n') {
            m_column += 2;
        } else
            break;

    }

    if (m_column >= m_strScript.length())
        return new Token(EOF);

    //关键字Token
    if (isalpha(m_strScript[m_column])) {
        return match_id();
    }
    //数字
    if (isdigit(m_strScript[m_column]) || m_strScript[m_column] == '-') {
        return match_number();
    }
    //字符串
    if (m_strScript[m_column] == '"') {
        return match_string();
    }
    //返回其他包括 [] （）, {}等
    return match_other();

}

Token *Lexer::match_id() {
    string str;
    while (isalnum(m_strScript[m_column]) || m_strScript[m_column] == '_'){
        str.push_back(m_strScript[m_column]);
        m_column++;
    }

    for (auto word : m_words) {
        if (word.first == str) {
            return new Word(word.second->Tag, word.second->word);
        }
    }

    return new Word(tag_ID, str);
}

Token *Lexer::match_other() {
    char tag = m_strScript[m_column];
    m_column++;
    return new Token(tag);
}

Token *Lexer::match_number() {
    int ivalue = 0;
    bool bNegative = false;
    if (m_strScript[m_column] == '-') {
        bNegative = true;
        m_column++;
    }
    while (isdigit(m_strScript[m_column])){
        ivalue = 10 * ivalue + m_strScript[m_column] - '0';// 123e-3
        m_column++;
    }
    if (m_strScript[m_column] == '.'){// 23.89
        double dvalue = ivalue;
        double power = 1;
        m_column++;
        while (isdigit(m_strScript[m_column])){
            power *= 10;
            dvalue += (m_strScript[m_column] - '0') / power;
            m_column++;
        }

        if (bNegative) dvalue = 0-dvalue;
        return new Double(dvalue);
    }
    if (bNegative) ivalue = 0-ivalue;
    return new Integer(ivalue);
}

Token *Lexer::match_string() {
    string str;
    m_column++;
    while (m_strScript[m_column] != '"'){
        str.push_back(m_strScript[m_column]);
        m_column++;
    }
    m_column++;
    return new Word(tag_STRING, str);
}

