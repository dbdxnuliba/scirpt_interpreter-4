//
// Created by 朱熙 on 2018/6/4.
//

#include "Parser.h"
#include "../util/StringUtil.hpp"


Parser::Parser(ServerManager *pServerManager) : m_curToken(nullptr), scl(keywords::channel = "parser_class") {
    m_pServerManager = pServerManager;

    ifstream t("./config/parser.script");
    stringstream buffer;
    buffer << t.rdbuf();
    string strScript(buffer.str());
    //setScript(strScript);
    m_lexer.setScript(strScript);
}

Parser::~Parser() {
    if (m_curToken) {
        delete m_curToken;
        m_curToken = nullptr;
    }
}

void Parser::parse() {
    m_lexer.m_column = 0;
    m_vm.stop();
    m_vm.reset();
    if (m_curToken) {
        delete m_curToken;
        m_curToken = nullptr;
    }
    m_curToken = m_lexer.next();
    bool isSuccess = true;
    while (m_curToken->Tag != EOF) {
        Node* pNode = stmt();
        if (pNode)
            m_vm.m_nodeList.push_back(pNode);
        else
        {
            isSuccess = false;
            break;
        }

    }
    if (isSuccess)
        m_vm.start();
}

Node *Parser::stmt_if() {

    Ifelse_Node* node = new Ifelse_Node();

    node->pGlobalParams = &m_vm.m_globalParams;
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;

    match(tag_IF);
    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    if (match('{')) {
        if (m_curToken->Tag == tag_STRING && ((Word*)m_curToken)->word == "option") {
            match(tag_STRING);
            match(':');
            node->ifIoRadio = (bool)atoi(((Word*)m_curToken)->word.c_str());
            match(tag_STRING);
            match(',');
            if (((Word*)m_curToken)->word == "dis") {
                match(tag_STRING);
                match(':');
                vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                for (auto str : stringList) {
                    std::vector<std::string> disItem = StringUtil::splitWithFlag(str);
                    if (disItem.length > 2);
                        node->disMap[atoi(disItem[0].c_str())] = atoi(disItem[2].c_str());
                    else
                        return nullptr;         //防止脚本格式錯誤，返回nullptr
                }
                match(tag_STRING);
            }

            match(',');

            if (((Word*)m_curToken)->word == "dos") {
                match(tag_STRING);
                match(':');
                vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                for (auto str : stringList) {
                    std::vector<std::string> dosItem = StringUtil::splitWithFlag(str);
                    if (dosItem.length > 2);
                        node->dosMap[atoi(dosItem[0].c_str())] = atoi(dosItem[2].c_str());
                    else
                        return nullptr;         //防止脚本格式錯誤，返回nullptr
                }
                match(tag_STRING);
            }

            match('}');
            match(',');
            match('{');

            if (m_curToken->Tag == tag_STRING && ((Word*)m_curToken)->word == "option") {
                match(tag_STRING);
                match(':');
                node->ifParamRadio = (bool)atoi(((Word*)m_curToken)->word.c_str());
                match(tag_STRING);
                match(',');
                if (((Word*)m_curToken)->word == "params") {
                    match(tag_STRING);
                    match(':');
                    vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                    for (auto str : stringList) {
                        node->ariths.push_back(Arith(str, &(m_vm.m_globalParams.global_variable)));
                    }

                }
                match(tag_STRING);
            }
            match('}');
            match(')');
        }

    }

    match('{');
    while(m_curToken->Tag != '}') {
        node->ifNodeList.push_back(stmt());
    }
    match('}');
    match(tag_ID);
    match('{');
    while(m_curToken->Tag != '}') {
        node->elseNodeList.push_back(stmt());
    }
    match('}');

    return node;
}

bool Parser::match(int Tag) {
    if (m_curToken->Tag == Tag){
        if (m_curToken) {
            delete m_curToken;
            m_curToken = nullptr;
        }
        m_curToken = m_lexer.next();
        return true;
    }


    BOOST_LOG_SEV(scl, severity_level::error) << __FUNCTION__ << ":" << __LINE__ << "column "<< m_lexer.m_column << " : " << Tag <<" not match.";
    if (m_curToken) {
        delete m_curToken;
        m_curToken = nullptr;
    }
    m_curToken = m_lexer.next();

    return false;
}

Node *Parser::stmt() {

    switch (m_curToken->Tag) {
        case tag_WAIT:
            return stmt_wait();
        case tag_IF:
            return stmt_if();
        case tag_WHILE:
            return stmt_while();
        case tag_SET:
            return stmt_set();
        case tag_SET_DIGITAL_OUT:
            return stmt_set_digital_out();
        case tag_STOP:
            return stmt_Stop();
        case tag_MOVEJ:
            return stmt_Movej();
        case tag_MOVEL:
            return stmt_Movel();
        case tag_SPEEDJ:
            return stmt_Speedj();
        case tag_SPEEDL:
            return stmt_Speedl();
        case tag_STOPJ:
            return stmt_Stopj();
        case tag_STOPL:
            return stmt_Stopl();
        case tag_TEACH_MODE:
            return stmt_TeachMode();
        case tag_END_TEACH_MODE:
            return stmt_EndTeachMode();
        case tag_MOVECAMERA:
            return stmt_MoveCamera();
        default:
            BOOST_LOG_SEV(scl, severity_level::error) << __FUNCTION__ << ":" << __LINE__ << "  unknow tag： column: "<< m_lexer.m_column << "- tag:" << m_curToken->Tag;
    }

    return nullptr;
}

Node *Parser::stmt_wait() {
    Wait_Node* node = new Wait_Node();

    node->pGlobalParams = &m_vm.m_globalParams;
    node->bStop = &m_vm.m_bStop;


    match(tag_WAIT);
    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    if (match('{')) {
        if (m_curToken->Tag == tag_STRING && ((Word*)m_curToken)->word == "option") {
            match(tag_STRING);
            match(':');
            node->ifIoRadio = (bool)atoi(((Word*)m_curToken)->word.c_str());
            match(tag_STRING);
            match(',');
            if (((Word*)m_curToken)->word == "dis") {
                match(tag_STRING);
                match(':');
                vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                for (auto str : stringList) {
                    std::vector<std::string> disItem = StringUtil::splitWithFlag(str);
                    if (disItem.length > 2);
                        node->disMap[atoi(disItem[0].c_str())] = atoi(disItem[2].c_str());
                    else
                        return nullptr;         //防止脚本格式錯誤，返回nullptr
                }
                match(tag_STRING);
            }

            match(',');

            if (((Word*)m_curToken)->word == "dos") {
                match(tag_STRING);
                match(':');
                vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                for (auto str : stringList) {
                    std::vector<std::string> dosItem = StringUtil::splitWithFlag(str);
                    if (dosItem.length > 2);
                        node->dosMap[atoi(dosItem[0].c_str())] = atoi(dosItem[2].c_str());
                    else
                        return nullptr;         //防止脚本格式錯誤，返回nullptr
                }
                match(tag_STRING);
            }

            match('}');
            match(',');
            match('{');

            if (m_curToken->Tag == tag_STRING && ((Word*)m_curToken)->word == "option") {
                match(tag_STRING);
                match(':');
                node->ifParamRadio = (bool)atoi(((Word*)m_curToken)->word.c_str());
                match(tag_STRING);
                match(',');
                if (((Word*)m_curToken)->word == "params") {
                    match(tag_STRING);
                    match(':');
                    vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
                    for (auto str : stringList) {
                        node->ariths.push_back(Arith(str, &(m_vm.m_globalParams.global_variable)));
                    }

                }
                match(tag_STRING);
            }
            match('}');
            match(',');
            if (m_curToken->Tag == tag_INT) {
                node->sleepMs = ((Integer*)m_curToken)->value;
                match(tag_INT);
            }
            match(')');
        }

    }


    return node;

}

Node *Parser::stmt_while() {
    While_Node* node = new While_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;

    match(tag_WHILE);
    match('{');
    while (m_curToken->Tag != '}') {
        node->nodeList.push_back(stmt());
    }
    match('}');
    return node;
}

Node *Parser::stmt_set() {
    Set_Node* node = new Set_Node();

    node->pGlobalParams = &m_vm.m_globalParams;
    node->pVm = &m_vm;
    node->bStop = &m_vm.m_bStop;


    match(tag_SET);
    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    match('{');
    match(tag_STRING);
    match(':');
    vector<string> stringList = StringUtil::split(((Word*)m_curToken)->word, ",");
    for (auto str : stringList) {
        node->dos.push_back(atoi(str.c_str()));
    }
    match(tag_STRING);
    match('}');

    match(',');

    match('{');
    match(tag_STRING);
    match(':');
    vector<string> strParams = StringUtil::split(((Word*)m_curToken)->word, ",");
    for (auto str : strParams) {
        vector<string> param = StringUtil::splitWithFlag(str);
        NUMBER_STRUCT numberStruct;
        if (param[0] == "int") {numberStruct.numberType = INT; numberStruct.number.num_int = atoi(param[4].c_str());}
        else if (param[0] == "short") {numberStruct.numberType = SHORT; numberStruct.number.num_short = (short)atoi(param[4].c_str());}
        else {numberStruct.numberType = DOUBLE; numberStruct.number.num_double = atof(param[4].c_str());}
        node->params[param[2]] = numberStruct;
    }
    match(tag_STRING);
    match('}');

    match(')');

    return node;
}

Node* stmt_set_digital_out() {

    Set_Digital_Out_Node* node = new Set_Digital_Out_Node();

    node->pGlobalParams = &m_vm.m_globalParams;
    node->pVm = &m_vm;
    node->bStop = &m_vm.m_bStop;


    match(tag_SET_DIGITAL_OUT);
    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->setDigitalOutIndex = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    if (m_curToken->Tag == tag_STRING) {
        node->value = ((((Word*)m_curToken)->word == "True") || (((Word*)m_curToken)->word == "true")) ? 1 : 0;
        match(tag_STRING);
    }

    match(')');

    return node;
}

Node *Parser::stmt_Movej() {

    Movej_Node* node = new Movej_Node;
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_MOVEJ);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    match('[');
    for (int i=0;i<7;i++){
        if (m_curToken->Tag == tag_DOUBLE) {
            node->axis[i] = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
            match(tag_DOUBLE);
            if (i<6) match(',');
        }
    }
    match(']');

    match(',');
    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->v = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->t = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->r = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');


    return node;
}

Node *Parser::stmt_Movel() {
    Movel_Node* node = new Movel_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_MOVEL);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    match('[');
    for (int i=0;i<7;i++){
        if (m_curToken->Tag == tag_DOUBLE) {
            node->pose[i] = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
            match(tag_DOUBLE);
            if (i<6) match(',');
        }
    }
    match(']');

    match(',');
    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->v = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->t = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->r = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');

    return node;
}

Node *Parser::stmt_Speedj() {
    Speedj_Node* node = new Speedj_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_SPEEDJ);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    match('[');
    for (int i=0;i<7;i++){
        if (m_curToken->Tag == tag_DOUBLE) {
            node->axis[i] = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
            match(tag_DOUBLE);
            if (i<6) match(',');
        }
    }
    match(']');

    match(',');
    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->t_min = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');

    return node;
}

Node *Parser::stmt_Speedl() {
    Speedl_Node* node = new Speedl_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_SPEEDL);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }

    match('[');
    for (int i=0;i<6;i++){
        if (m_curToken->Tag == tag_DOUBLE) {
            node->xd[i] = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
            match(tag_DOUBLE);
            if (i<5) match(',');
        }
    }
    match(']');

    match(',');
    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->t_min = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');

    return node;
}

Node *Parser::stmt_Stopj() {
    Stopj_Node* node = new Stopj_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_STOPJ);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }


    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');

    return node;
}

Node *Parser::stmt_Stopl() {
    Stopl_Node* node = new Stopl_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_STOPL);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
        match(',');
    }


    node->a = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(')');

    return node;
}

Node *Parser::stmt_TeachMode() {
    Teach_Mode_Node* node = new Teach_Mode_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_TEACH_MODE);

    match('(');
//    if (m_curToken->Tag == tag_INT) {
//        node->id = ((Integer*)m_curToken)->value;
//        match(tag_INT);
//        match(',');
//    }
    node->id = 0;


    node->x = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->y = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->z = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->rx = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->ry = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);

    match(',');
    node->rz = m_curToken->Tag == tag_INT ? ((Integer*)m_curToken)->value : ((Double*)m_curToken)->value;
    m_curToken->Tag == tag_INT ? match(tag_INT) : match(tag_DOUBLE);



    match(')');

    return node;
}

Node *Parser::stmt_EndTeachMode() {
    End_Teach_Mode_Node* node = new End_Teach_Mode_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;
    node->pMechanicalarm = m_vm.m_xmlReader.m_mechanicalarm;

    match(tag_END_TEACH_MODE);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
    }

    match(')');

    return node;
}

Node *Parser::stmt_Stop() {
    Stop_Node *node = new Stop_Node();
    node->bStop = &m_vm.m_bStop;

    match(tag_STOP);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
    }
    match(')');

    return node;
}

Node *Parser::stmt_MoveCamera() {
    MoveCamera_Node* node = new MoveCamera_Node();
    node->bStop = &m_vm.m_bStop;
    node->pVm = &m_vm;

    match(tag_MOVECAMERA);

    match('(');
    if (m_curToken->Tag == tag_INT) {
        node->id = ((Integer*)m_curToken)->value;
        match(tag_INT);
    }

    match(')');

    return node;
}

int nRecvScript = 0;
void Parser::setScript(string str) {
    nRecvScript++;
    BOOST_LOG_SEV(scl, info) << "receive script, total : " << nRecvScript;
    saveScript(str);
    if (str[0] != '#') {
        if (str == "power on") {
            m_vm.setPowerState(true);
        }else if (str == "power off") {
            m_vm.setPowerState(false);
        }else if (str == "play") {
            m_vm.setPlayState(true);
            m_vm.start();
        }else if (str == "stop") {
            m_vm.setPlayState(false);
            m_vm.stop();
        }else if (str == "pause") {
            m_vm.setPauseState(true);
            //todo vm暂停功能待实现
        }

        string strTcpRes = m_vm.m_xmlReader.m_mechanicalarm->params[str];

        unsigned int totalLength = sizeof(totalLength);
        totalLength += strTcpRes.size();
        char *buf = new char[totalLength];
        totalLength = htonl(totalLength);
        memcpy(buf, &totalLength, sizeof(totalLength));
        memcpy(buf + sizeof(totalLength), strTcpRes.c_str(), strTcpRes.size());

        m_pServerManager->m_pTcpServer->send(buf, ntohl(totalLength));

        delete[] buf;
        buf = nullptr;

    } else if (m_vm.m_bStop) {
        m_lexer.setScript(str);
        parse();
    } else if (m_lexer.getScript().substr(0, 4) == "#sec") {
        m_vm.stop();
        m_lexer.setScript(str);
        parse();
    } else {
        return;
    }


}

void Parser::saveScript(string str) {

    if (str.length()>0 && str.substr(0,4) == "#def") {
        ofstream t("./config/parser.script");
        t << str;
    }

}


