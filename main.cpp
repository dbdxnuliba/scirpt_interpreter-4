#include <iostream>
#include "src/util/log.hpp"
#include "src/ServerManager/ServerManager.h"


int main() {
    init_log_environment("./config/log.config");
    ServerManager serverManager;
    serverManager.createManager();
    serverManager.startManager();

    return 0;
}