//
// Created by 朱熙 on 2018/6/6.
//

#ifndef PARSERSERVER_XMLREADER_H
#define PARSERSERVER_XMLREADER_H

#include "../../../include/global.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;


typedef map<string, string> PARAMS;

struct sub_node {
    string mold;
    int min;
    int max;
    int defalut;
};

struct sub_node_double {
    string mold;
    double min;
    double max;
    double defalut;
};

struct MOVEJ {
    string type;
    string modal;
    sub_node_double paxis[7];
    sub_node_double a;
    sub_node_double v;
    sub_node t;
    sub_node_double r;
};

struct MOVEL {
    string type;
    string modal;
    sub_node_double pose[7];
    sub_node_double a;
    sub_node_double v;
    sub_node t;
    sub_node_double r;
};

struct SPEEDJ {
    string type;
    string modal;
    sub_node_double pd_axis[6];
    sub_node_double a;
    sub_node t_min;
};

struct SPEEDL {
    string type;
    string modal;
    sub_node_double xd[6];
    sub_node_double a;
    sub_node t_min;
};

struct STOPJ {
    string type;
    string modal;
    sub_node_double a;
};

struct STOPL {
    string type;
    string modal;
    sub_node_double a;
};

struct TEACH_MODE {
    string type;
    string modal;
    sub_node x;
    sub_node y;
    sub_node z;
    sub_node rx;
    sub_node ry;
    sub_node rz;
};

struct END_TEACH_MODE {
    string type;
    string modal;
};


struct MECHANICALARM {
    PARAMS params;
    MOVEJ movej;
    MOVEL movel;
    SPEEDJ speedj;
    SPEEDL speedl;
    STOPJ stopj;
    STOPL stopl;
    TEACH_MODE teach_mode;
    END_TEACH_MODE end_teach;
};

class XmlReader {
public:
    XmlReader();
    ~XmlReader();

public:
    MECHANICALARM* m_mechanicalarm;

    void readXml(string xmlFile);

private:
    void setSubNode(ptree* p, sub_node* node);
    void setSubNodeDouble(ptree* p, sub_node_double* node);
};


#endif //PARSERSERVER_XMLREADER_H
