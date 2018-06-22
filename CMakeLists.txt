cmake_minimum_required(VERSION 3.10)
project(ParserServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(ParserServer main.cpp
        src/tcp/tcpServer.cpp src/tcp/tcpServer.h
        src/util/log.hpp src/ServerManager/ServerManager.cpp
        src/ServerManager/ServerManager.h include/global.h
        src/xmlReader.h
        include/xmlStructs.h src/Parser/lexer/Lexer.cpp src/Parser/lexer/Lexer.h
        src/Parser/Parser.cpp src/Parser/Parser.h src/Parser/vm/Vm.cpp
        src/Parser/vm/Vm.h src/robotStateServer/RobotStateServer.cpp
        src/robotStateServer/RobotStateServer.h
        src/Parser/vm/robot_state.h src/Parser/vm/robot_state.cpp include/endian.h src/Parser/xmlReader/XmlReader.cpp src/Parser/xmlReader/XmlReader.h src/util/StringUtil.hpp src/util/IntegerUtil.hpp src/util/memUtil.hpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.67.0 REQUIRED COMPONENTS log log_setup thread system filesystem)

#添加头文件搜索路径
include_directories(/usr/local/include)

#添加库文件搜索路径
link_directories(/usr/local/lib)

target_link_libraries(ParserServer ${Boost_LIBRARIES})