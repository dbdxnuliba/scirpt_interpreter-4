#include <iostream>
#include "src/util/log.hpp"
#include "src/ServerManager/ServerManager.h"


int	main(int argc, char* argv[]) {
    init_log_environment("./config/log.config");

    //参数变量初始化
    string type;
    int time = -1;
    int col = -1; //get查询的列索引0～n

    //获取参数
    if (argc > 1) {
        type = string(argv[1]);
    }
    int ch;
    opterr = 0; //选项错误时不让报错
    while ((ch = getopt(argc, argv, "t:")) != -1) {
        switch (ch) {
            case 't':
                time = atoi(optarg);
                break;
        }
    }



    ServerManager serverManager;
    serverManager.createManager();

    serverManager.m_pRobotStateServer->m_sendTimestep = time;

    serverManager.startManager();

    return 0;
}
