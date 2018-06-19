//
// Created by 朱熙 on 2018/6/6.
//

#include "XmlReader.h"


XmlReader::XmlReader() {
    m_mechanicalarm = new MECHANICALARM;

}

XmlReader::~XmlReader() {
    delete m_mechanicalarm;
    m_mechanicalarm = nullptr;
}

void XmlReader::readXml(string xmlFile) {
    ptree pt;
    read_xml(xmlFile, pt);

    ptree params = pt.get_child("MechanicalArm.params");

    for(auto param : params) {
        if (param.first == "<xmlcomment>") continue;
        m_mechanicalarm->params[param.second.get<string>("request")] = param.second.get<string>("response");
    }

    ptree command = pt.get_child("MechanicalArm.command");
    for (auto pos : command) {
        if (pos.first == "<xmlcomment>") continue;

        string strNamespace = pos.second.get<string>("<xmlattr>.namespace");
        string strType = pos.second.get<string>("<xmlattr>.type");
        string strModal = pos.second.get<string>("<xmlattr>.modal");

        if (strNamespace == "movej") {
            m_mechanicalarm->movej.type = strType;
            m_mechanicalarm->movej.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("p.AXIS1"), &m_mechanicalarm->movej.paxis[0]);
            setSubNodeDouble(&pos.second.get_child("p.AXIS2"), &m_mechanicalarm->movej.paxis[1]);
            setSubNodeDouble(&pos.second.get_child("p.AXIS3"), &m_mechanicalarm->movej.paxis[2]);
            setSubNodeDouble(&pos.second.get_child("p.AXIS4"), &m_mechanicalarm->movej.paxis[3]);
            setSubNodeDouble(&pos.second.get_child("p.AXIS5"), &m_mechanicalarm->movej.paxis[4]);
            setSubNodeDouble(&pos.second.get_child("p.AXIS6"), &m_mechanicalarm->movej.paxis[5]);
            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->movej.a);
            setSubNodeDouble(&pos.second.get_child("v"), &m_mechanicalarm->movej.v);
            setSubNode(&pos.second.get_child("t"), &m_mechanicalarm->movej.t);
            setSubNodeDouble(&pos.second.get_child("r"), &m_mechanicalarm->movej.r);
        } else if (strNamespace == "movel") {
            m_mechanicalarm->movel.type = strType;
            m_mechanicalarm->movel.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("pose.X"), &m_mechanicalarm->movel.pose[0]);
            setSubNodeDouble(&pos.second.get_child("pose.Y"), &m_mechanicalarm->movel.pose[1]);
            setSubNodeDouble(&pos.second.get_child("pose.Z"), &m_mechanicalarm->movel.pose[2]);
            setSubNodeDouble(&pos.second.get_child("pose.RX"), &m_mechanicalarm->movel.pose[3]);
            setSubNodeDouble(&pos.second.get_child("pose.RY"), &m_mechanicalarm->movel.pose[4]);
            setSubNodeDouble(&pos.second.get_child("pose.RZ"), &m_mechanicalarm->movel.pose[5]);
            setSubNodeDouble(&pos.second.get_child("pose.RN"), &m_mechanicalarm->movel.pose[6]);
            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->movel.a);
            setSubNodeDouble(&pos.second.get_child("v"), &m_mechanicalarm->movel.v);
            setSubNode(&pos.second.get_child("t"), &m_mechanicalarm->movel.t);
            setSubNodeDouble(&pos.second.get_child("r"), &m_mechanicalarm->movel.r);

        } else if (strNamespace == "speedj") {
            m_mechanicalarm->speedj.type = strType;
            m_mechanicalarm->speedj.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("qd.AXIS1"), &m_mechanicalarm->speedj.pd_axis[0]);
            setSubNodeDouble(&pos.second.get_child("qd.AXIS2"), &m_mechanicalarm->speedj.pd_axis[1]);
            setSubNodeDouble(&pos.second.get_child("qd.AXIS3"), &m_mechanicalarm->speedj.pd_axis[2]);
            setSubNodeDouble(&pos.second.get_child("qd.AXIS4"), &m_mechanicalarm->speedj.pd_axis[3]);
            setSubNodeDouble(&pos.second.get_child("qd.AXIS5"), &m_mechanicalarm->speedj.pd_axis[4]);
            setSubNodeDouble(&pos.second.get_child("qd.AXIS6"), &m_mechanicalarm->speedj.pd_axis[5]);
            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->speedj.a);
            setSubNode(&pos.second.get_child("t_min"), &m_mechanicalarm->speedj.t_min);

        } else if (strNamespace == "speedl") {
            m_mechanicalarm->speedl.type = strType;
            m_mechanicalarm->speedl.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("xd.X"), &m_mechanicalarm->speedl.xd[0]);
            setSubNodeDouble(&pos.second.get_child("xd.Y"), &m_mechanicalarm->speedl.xd[1]);
            setSubNodeDouble(&pos.second.get_child("xd.Z"), &m_mechanicalarm->speedl.xd[2]);
            setSubNodeDouble(&pos.second.get_child("xd.RX"), &m_mechanicalarm->speedl.xd[3]);
            setSubNodeDouble(&pos.second.get_child("xd.RY"), &m_mechanicalarm->speedl.xd[4]);
            setSubNodeDouble(&pos.second.get_child("xd.RZ"), &m_mechanicalarm->speedl.xd[5]);
            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->speedl.a);
            setSubNode(&pos.second.get_child("t_min"), &m_mechanicalarm->speedl.t_min);

        } else if (strNamespace == "stopj") {
            m_mechanicalarm->stopj.type = strType;
            m_mechanicalarm->stopj.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->stopj.a);

        } else if (strNamespace == "stopl") {
            m_mechanicalarm->stopl.type = strType;
            m_mechanicalarm->stopl.modal = strModal;

            setSubNodeDouble(&pos.second.get_child("a"), &m_mechanicalarm->stopl.a);

        } else if (strNamespace == "teach_mode") {
            m_mechanicalarm->teach_mode.type = strType;
            m_mechanicalarm->teach_mode.modal = strModal;

            setSubNode(&pos.second.get_child("x"), &m_mechanicalarm->teach_mode.x);
            setSubNode(&pos.second.get_child("y"), &m_mechanicalarm->teach_mode.y);
            setSubNode(&pos.second.get_child("z"), &m_mechanicalarm->teach_mode.z);
            setSubNode(&pos.second.get_child("Rx"), &m_mechanicalarm->teach_mode.rx);
            setSubNode(&pos.second.get_child("Rx"), &m_mechanicalarm->teach_mode.ry);
            setSubNode(&pos.second.get_child("Rx"), &m_mechanicalarm->teach_mode.rz);

        } else if (strNamespace == "end_teach_mode") {
            m_mechanicalarm->end_teach.type = strType;
            m_mechanicalarm->end_teach.modal = strModal;
        }

    }
}

void XmlReader::setSubNode(ptree *p, sub_node *node) {
    string strMold = p->get<string>("<xmlattr>.mold");
    int iMin = p->get<int>("min");
    int iMax = p->get<int>("max");
    int iDefault = p->get<int>("default");

    node->mold = strMold;
    node->min = iMin;
    node->max = iMax;
    node->defalut = iDefault;

}

void XmlReader::setSubNodeDouble(ptree *p, sub_node_double *node) {
    string strMold = p->get<string>("<xmlattr>.mold");
    double iMin = p->get<double>("min");
    double iMax = p->get<double>("max");
    double iDefault = p->get<double>("default");

    node->mold = strMold;
    node->min = iMin;
    node->max = iMax;
    node->defalut = iDefault;
}
