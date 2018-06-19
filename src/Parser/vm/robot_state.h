/*
 * robot_state.h
 *
 * Copyright 2017 DrZ @ JKTech
 * Copyright 2015 Thomas Timm Andersen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ROBOT_STATE_H_
#define ROBOT_STATE_H_

#include <inttypes.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>

namespace message_types
{
enum message_type
{
    ROBOT_STATE = 16, ROBOT_MESSAGE = 20, PROGRAM_STATE_MESSAGE = 25
};
}
typedef message_types::message_type messageType;

namespace package_types
{
enum package_type
{
    ROBOT_MODE_DATA = 0,
    JOINT_DATA = 1,
    TOOL_DATA = 2,
    MASTERBOARD_DATA = 3,
    CARTESIAN_INFO = 4,
    KINEMATICS_INFO = 5,
    CONFIGURATION_DATA = 6,
    FORCE_MODE_DATA = 7,
    ADDITIONAL_INFO = 8,
    CALIBRATION_DATA = 9
};
}
typedef package_types::package_type packageType;

namespace robot_message_types
{
enum robot_message_type
{
    ROBOT_MESSAGE_TEXT = 0,
    ROBOT_MESSAGE_PROGRAM_LABEL = 1,
    PROGRAM_STATE_MESSAGE_VARIABLE_UPDATE = 2,
    ROBOT_MESSAGE_VERSION = 3,
    ROBOT_MESSAGE_SAFETY_MODE = 5,
    ROBOT_MESSAGE_ERROR_CODE = 6,
    ROBOT_MESSAGE_KEY = 7,
    ROBOT_MESSAGE_REQUEST_VALUE = 9,
    ROBOT_MESSAGE_RUNTIME_EXCEPTION = 10
};
}
typedef robot_message_types::robot_message_type robotMessageType;

namespace robot_state_type_v18
{
enum robot_state_type
{
    ROBOT_RUNNING_MODE = 0,
    ROBOT_FREEDRIVE_MODE = 1,
    ROBOT_READY_MODE = 2,
    ROBOT_INITIALIZING_MODE = 3,
    ROBOT_SECURITY_STOPPED_MODE = 4,
    ROBOT_EMERGENCY_STOPPED_MODE = 5,
    ROBOT_FATAL_ERROR_MODE = 6,
    ROBOT_NO_POWER_MODE = 7,
    ROBOT_NOT_CONNECTED_MODE = 8,
    ROBOT_SHUTDOWN_MODE = 9,
    ROBOT_SAFEGUARD_STOP_MODE = 10
};
}
typedef robot_state_type_v18::robot_state_type robotStateTypeV18;
namespace robot_state_type_v30
{
enum robot_state_type
{
    ROBOT_MODE_DISCONNECTED = 0,
    ROBOT_MODE_CONFIRM_SAFETY = 1,
    ROBOT_MODE_BOOTING = 2,
    ROBOT_MODE_POWER_OFF = 3,
    ROBOT_MODE_POWER_ON = 4,
    ROBOT_MODE_IDLE = 5,
    ROBOT_MODE_BACKDRIVE = 6,
    ROBOT_MODE_RUNNING = 7,
    ROBOT_MODE_UPDATING_FIRMWARE = 8
};
}

typedef robot_state_type_v30::robot_state_type robotStateTypeV30;

struct version_message
{
    uint64_t timestamp;
    int8_t source;
    int8_t robot_message_type;
    int8_t project_name_size;
    char project_name[15];
    uint8_t major_version;
    uint8_t minor_version;
    int svn_revision;
    char build_date[25];
};

struct robot_mode_data //ROBOT_MODE_DATA = 0
{
    uint64_t timestamp;
    bool isRobotConnected;
    bool isRealRobotEnabled;
    bool isPowerOnRobot;
    bool isEmergencyStopped;
    bool isProtectiveStopped;
    bool isProgramRunning;
    bool isProgramPaused;
    unsigned char robotMode;
    unsigned char controlMode;
    double targetSpeedFraction;
    double speedScaling;
};


struct joint_data //JOINT_DATA = 1
{
  double  q_actual_1;
  double  q_target_1;
  double qd_actual_1;
  float  I_actual_1;
  float  V_actual_1;
  float  T_motor_1;
  float  T_micro_1;
  unsigned char  jointMode_1;
  double  q_actual_2;
  double  q_target_2;
  double qd_actual_2;
  float  I_actual_2;
  float  V_actual_2;
  float  T_motor_2;
  float  T_micro_2;
  unsigned char  jointMode_2;
  double  q_actual_3;
  double  q_target_3;
  double qd_actual_3;
  float  I_actual_3;
  float  V_actual_3;
  float  T_motor_3;
  float  T_micro_3;
  unsigned char  jointMode_3;
  double  q_actual_4;
  double  q_target_4;
  double qd_actual_4;
  float  I_actual_4;
  float  V_actual_4;
  float  T_motor_4;
  float  T_micro_4;
  unsigned char  jointMode_4;
  double  q_actual_5;
  double  q_target_5;
  double qd_actual_5;
  float  I_actual_5;
  float  V_actual_5;
  float  T_motor_5;
  float  T_micro_5;
  unsigned char  jointMode_5;
  double  q_actual_6;
  double  q_target_6;
  double qd_actual_6;
  float  I_actual_6;
  float  V_actual_6;
  float  T_motor_6;
  float  T_micro_6;
  unsigned char  jointMode_6;
  double  q_actual_7;
  double  q_target_7;
  double qd_actual_7;
  float  I_actual_7;
  float  V_actual_7;
  float  T_motor_7;
  float  T_micro_7;
  unsigned char  jointMode_7;
};


struct tool_data// TOOL_DATA = 2
{
  char analogInputRange2;
  char analogInputRange3;
  double analogInput2;
  double analogInput3;
  float toolVoltage48V;
  unsigned char toolOutputVoltage;
  float toolCurrent;
  float toolTemperature;
  unsigned char toolMode;
};

struct masterboard_data //MASTERBOARD_DATA = 3
{
    int digitalInputBits;
    int digitalOutputBits;
    char analogInputRange0;
    char analogInputRange1;
    double analogInput0;
    double analogInput1;
    char analogOutputDomain0;
    char analogOutputDomain1;
    double analogOutput0;
    double analogOutput1;
    float masterBoardTemperature;
    float robotVoltage48V;
    float robotCurrent;
    float masterIOCurrent;
    unsigned char safetyMode;
    unsigned char masterOnOffState;
    char euromap67InterfaceInstalled;
    int euromapInputBits;
    int euromapOutputBits;
    float euromapVoltage;
    float euromapCurrent;
};


struct cartesian_info //CARTESIAN_INFO = 4
{
  double X;
  double Y;
  double Z;
  double Rx;
  double Ry;
  double Rz;
  double TCPOffsetX;
  double TCPOffsetY;
  double TCPOffsetZ;
  double TCPOffsetRx;
  double TCPOffsetRy;
  double TCPOffsetRz;
};


class RobotState
{
private:
    version_message version_msg_;
    masterboard_data mb_data_;
    robot_mode_data robot_mode_;
	joint_data joint_data_;
	cartesian_info cartesian_info_;

    std::recursive_mutex val_lock_; // Locks the variables while unpack parses data;

    std::condition_variable* pMsg_cond_; //Signals that new vars are available
    bool new_data_available_; //to avoid spurious wakes
    unsigned char robot_mode_running_;

    double ntohd(uint64_t nf);
    double htond(uint64_t nf);

public:
    RobotState(std::condition_variable& msg_cond);
    ~RobotState();
    double getVersion();
    double getTime();
    std::vector<double> getQTarget(); //TODO
	
    int getDigitalInputBits();
    int getDigitalOutputBits();
    char getAnalogInputRange0();
    char getAnalogInputRange1();
    double getAnalogInput0();
    double getAnalogInput1();
    char getAnalogOutputDomain0();
    char getAnalogOutputDomain1();
    double getAnalogOutput0();
    double getAnalogOutput1();
    std::vector<double> getVActual(); //TODO
    float getMasterBoardTemperature();
    float getRobotVoltage48V();
    float getRobotCurrent();
    float getMasterIOCurrent();
    unsigned char getSafetyMode();
    unsigned char getInReducedMode();
    char getEuromap67InterfaceInstalled();
    int getEuromapInputBits();
    int getEuromapOutputBits();
    float getEuromapVoltage();
    float getEuromapCurrent();

    void setDigitalInputBits(int digitalInputBits);
    void setDigitalOutputBits(int digitalOutputBits);
    void setAnalogInputRange0(char analogInput0);
    void setAnalogInputRange1(char analogInput1);
    void setAnalogInput0(double analogInput0);
    void setAnalogInput1(double analogInput1);
    void setAnalogOutputDomain0(char analogOutputDomain0);
    void setAnalogOutputDomain1(char analogOutputDomain1);
    void setAnalogOutput0(double analogOutput0);
    void setAnalogOutput1(double analogOutput1);
    void setMasterBoardTemperature(float masterBoardTemperature);
    void setRobotVoltage48V(float robotVoltage48V);
    void setRobotCurrent(float robotCurrent);
    void setMasterIOCurrent(float masterIOCurrent);
    void setSafetyMode(unsigned char safetyMode);
    void setInReducedMode(unsigned char masterOnOffState);
    void setEuromap67InterfaceInstalled(char euromap67InterfaceInstalled);
    void setEuromapInputBits(int euromapInputBits);
    void setEuromapOutputBits(int euromapOutputBits);
    void setEuromapVoltage(float euromapVoltage);
    void setEuromapCurrent(float euromapCurrent);

	/* Joint data get_ */
	double get_q_actual_1();
	double get_q_target_1();
	double get_qd_actual_1();
	float get_I_actual_1();
	float get_V_actual_1();
	float get_T_motor_1();
	float get_T_micro_1();
	unsigned char get_jointMode_1();
	double get_q_actual_2();
	double  get_q_target_2();
	double get_qd_actual_2();
	float  get_I_actual_2();
	float  get_V_actual_2();
	float  get_T_motor_2();
	float  get_T_micro_2();
	unsigned char get_jointMode_2();
	double get_q_actual_3();
	double get_q_target_3();
	double get_qd_actual_3();
	float get_I_actual_3();
	float get_V_actual_3();
	float get_T_motor_3();
	float get_T_micro_3();
	unsigned char get_jointMode_3();
	double get_q_actual_4();
	double get_q_target_4();
	double get_qd_actual_4();
	float get_I_actual_4();
	float get_V_actual_4();
	float get_T_motor_4();
	float get_T_micro_4();
	unsigned char  get_jointMode_4();
	double get_q_actual_5();
	double get_q_target_5();
	double get_qd_actual_5();
	float get_I_actual_5();
	float get_V_actual_5();
	float get_T_motor_5();
	float get_T_micro_5();
	unsigned char  get_jointMode_5();
	double  get_q_actual_6();
	double  get_q_target_6();
	double get_qd_actual_6();
	float get_I_actual_6();
	float get_V_actual_6();
	float get_T_motor_6();
	float get_T_micro_6();
	unsigned char get_jointMode_6();
	double get_q_actual_7();
	double get_q_target_7();
	double get_qd_actual_7();
	float get_I_actual_7();
	float get_V_actual_7();
	float get_T_motor_7();
	float get_T_micro_7();
	unsigned char  get_jointMode_7();
  
  // Joint data set_
	void set_q_actual_1(double q_actual_1);
	void set_q_target_1(double q_target_1);
	void set_qd_actual_1(double qd_actual_1);
	void set_I_actual_1(float I_actual_1);
	void set_V_actual_1(float V_actual_1);
	void set_T_motor_1(float T_motor_1);
	void set_T_micro_1(float T_micro_1);
	void set_jointMode_1(unsigned char jointMode_1);
	void set_q_actual_2(double q_actual_2);
	void set_q_target_2(double q_target_2);
	void set_qd_actual_2(double qd_actual_2);
	void set_I_actual_2(float I_actual_2);
	void set_V_actual_2(float V_actual_2);
	void set_T_motor_2(float T_motor_2);
	void set_T_micro_2(float T_micro_2);
	void set_jointMode_2(unsigned char jointMode_2);
	void set_q_actual_3(double q_actual_3);
	void set_q_target_3(double q_target_3);
	void set_qd_actual_3(double qd_actual_3);
	void set_I_actual_3(float I_actual_3);
	void set_V_actual_3(float V_actual_3);
	void set_T_motor_3(float T_motor_3);
	void set_T_micro_3(float T_micro_3);
	void set_jointMode_3(unsigned char jointMode_3);
	void set_q_actual_4(double q_actual_4);
	void set_q_target_4(double q_target_4);
	void set_qd_actual_4(double qd_actual_4);
	void set_I_actual_4(float I_actual_4);
	void set_V_actual_4(float V_actual_4);
	void set_T_motor_4(float T_motor_4);
	void set_T_micro_4(float T_micro_4);
	void set_jointMode_4(unsigned char jointMode_4);
	void set_q_actual_5(double q_actual_5);
	void set_q_target_5(double q_target_5);
	void set_qd_actual_5(double qd_actual_5);
	void set_I_actual_5(float I_actual_5);
	void set_V_actual_5(float V_actual_5);
	void set_T_motor_5(float T_motor_5);
	void set_T_micro_5(float T_micro_5);
	void set_jointMode_5(unsigned char jointMode_5);
	void set_q_actual_6(double q_actual_6);
	void set_q_target_6(double q_target_6);
	void set_qd_actual_6(double qd_actual_6);
	void set_I_actual_6(float I_actual_6);
	void set_V_actual_6(float V_actual_6);
	void set_T_motor_6(float T_motor_6);
	void set_T_micro_6(float T_micro_6);
	void set_jointMode_6(unsigned char jointMode_6);
	void set_q_actual_7(double q_actual_7);
	void set_q_target_7(double q_target_7);
	void set_qd_actual_7(double qd_actual_7);
	void set_I_actual_7(float I_actual_7);
	void set_V_actual_7(float V_actual_7);
	void set_T_motor_7(float T_motor_7);
	void set_T_micro_7(float T_micro_7);
	void set_jointMode_7(unsigned char jointMode_7);

	/* Cartesian info get_ */
	double get_X();
	double get_Y();
	double get_Z();
	double get_Rx();
	double get_Ry();
	double get_Rz();
	double get_TCPOffsetX();
	double get_TCPOffsetY();
	double get_TCPOffsetZ();
	double get_TCPOffsetRx();
	double get_TCPOffsetRy();
	double get_TCPOffsetRz();

	/* Cartesian info set_ */
	void set_X(double X);
	void set_Y(double Y);
	void set_Z(double Z);
	void set_Rx(double Rx);
	void set_Ry(double Ry);
	void set_Rz(double Rz);
	void set_TCPOffsetX(double TCPOffsetX);
	void set_TCPOffsetY(double TCPOffsetY);
	void set_TCPOffsetZ(double TCPOffsetZ);
	void set_TCPOffsetRx(double TCPOffsetRx);
	void set_TCPOffsetRy(double TCPOffsetRy);
	void set_TCPOffsetRz(double TCPOffsetRz);

	
    bool isRobotConnected();
    bool isRealRobotEnabled();
    bool isPowerOnRobot();
    bool isEmergencyStopped();
    bool isProtectiveStopped();
    bool isProgramRunning();
    bool isProgramPaused();
    unsigned char getRobotMode();
    bool isReady();

    void setRobotConnected(bool isRobotConnected);
    void setRealRobotEnabled(bool isRealRobotEnabled);
    void setPowerOnRobot(bool isPowerOnRobot);
    void setEmergencyStopped(bool isEmergencyStopped);
    void setProtectiveStopped(bool isProtectiveStopped);
    void setProgramRunning(bool isProgramRunning);
    void setProgramPaused(bool isProgramPaused);
    void setRobotMode(unsigned char robotMode);

    void setDisconnected();

    bool getNewDataAvailable();
    void finishedReading();

    void unpack(uint8_t * buf, unsigned int buf_length);
    void unpackRobotMessage(uint8_t * buf, unsigned int offset, uint32_t len);
    void unpackRobotMessageVersion(uint8_t * buf, unsigned int offset, uint32_t len);
    void unpackRobotState(uint8_t * buf, unsigned int offset, uint32_t len);
    void unpackRobotStateMasterboard(uint8_t * buf, unsigned int offset);
    void unpackRobotMode(uint8_t * buf, unsigned int offset);
	void unpackJointData(uint8_t * buf, unsigned int offset);
	void unpackCartesianInfo(uint8_t * buf, unsigned int offset);

	unsigned int pack(uint8_t* buf);
	unsigned int packRobotMessage(uint8_t* buf, unsigned int offset);
	unsigned int packRobotMessageVersion(uint8_t* buf, unsigned int offset);
	unsigned int packRobotState(uint8_t* buf, unsigned int offset, uint8_t package_type);
	unsigned int packRobotStateMasterboard(uint8_t * buf, unsigned int offset);
	unsigned int packRobotMode(uint8_t * buf, unsigned int offset);
	unsigned int packJointData(uint8_t * buf, unsigned int offset);
	unsigned int packCartesianInfo(uint8_t * buf, unsigned int offset);

	void unpackFromMem(uint8_t * buf, unsigned int buf_length);
	unsigned int packToMem(uint8_t * buf);

};

#endif /* ROBOT_STATE_H_ */
