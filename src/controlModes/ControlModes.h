// -*- mode:C++ { } tab-width:4 { } c-basic-offset:4 { } indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 iCub Facility
 * Authors: Marco Randazzo
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef _CONTROLMODES_H_
#define _CONTROLMODES_H_

#include <string>
#include <rtf/yarp/YarpTestCase.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>

/**
* \ingroup icub-tests
* The test checks if the joint is able to go in all the available control/interaction modes and if transition between the states is correct.
* The following control modes are test: VOCAB_CM_POSITION, VOCAB_CM_POSITION_DIRECT, VOCAB_CM_VELOCITY, VOCAB_CM_MIXED, VOCAB_CM_OPENLOOP, VOCAB_CM_IDLE, VOCAB_CM_FORCE_IDLE, VOCAB_HW_FAULT.
* The following interaction modes are tested: VOCAB_IM_STIFF, VOCAB_IM_COMPLIANT.
* For some modes (VOCAB_CM_TORQUE, VOCAB_IM_COMPLIANT) the test asks to robotInterface if the joint capabilities, skipping the test if those modes are not implemented.
* The test intentionally generates an hardware fault to test the transition between VOCAB_CM_HW_FAULT to VOCAB_CM_IDLE. The fault is generated by zeroing the max current limit.
* Check of the amplifier internal status (iAmplifier->getAmpStatus) has to be implemented yet. 
*
* Example: testRunner -v -t ControlModes.dll -p "--robot icub --part head --joints ""(0 1 2 3 4 5)"" --zero 0"
*
* Check the following functions:
* \li IControlMode2::getControlMode()/setControlMode()
* \li IInteractionMode::getInteractionMode()/setInteractionMode()

*  Accepts the following parameters:
* | Parameter name     | Type   | Units | Default Value | Required | Description | Notes |
* |:------------------:|:------:|:-----:|:-------------:|:--------:|:-----------:|:-----:|
* | robot              | string | -     | -             | Yes      | The name of the robot.     | e.g. icub |
* | part               | string | -     | -             | Yes      | The name of trhe robot part. | e.g. left_arm |
* | joints             | vector of ints | -             | Yes      | List of joints to be tested. | |
* | zero               | double | deg   | -             | Yes      | The home position for the tested joints. | |
*/

class ControlModes : public YarpTestCase {
public:
    ControlModes();
    virtual ~ControlModes();

    virtual bool setup(yarp::os::Property& property);

    virtual void tearDown();

    virtual void run();

    void goHome();
    void executeCmd();
    void setMode(int desired_control_mode, yarp::dev::InteractionModeEnum desired_interaction_mode);
    void verifyMode(int desired_control_mode, yarp::dev::InteractionModeEnum desired_interaction_mode, yarp::os::ConstString title);
    void verifyAmplifier(int desired_amplifier_mode, yarp::os::ConstString title);

    void zeroCurrentLimits();
    void getOriginalCurrentLimits();
    void resetOriginalCurrentLimits();
    void verifyModeSingle(int joint, int desired_control_mode, yarp::dev::InteractionModeEnum desired_interaction_mode, yarp::os::ConstString title);
    void setModeSingle(int joint, int desired_control_mode, yarp::dev::InteractionModeEnum desired_interaction_mode);
    void checkJointWithTorqueMode();
    void checkControlModeWithImCompliant(int desired_control_mode, yarp::os::ConstString title);

private:
    std::string robotName;
    std::string partName;
    int* jointsList;
    int* jointTorqueCtrlEnabled;

    double zero;
    int    n_part_joints;
    int    n_cmd_joints;
    enum cmd_mode_t
    { 
      single_joint = 0,
      all_joints = 1,
      some_joints =2
    } cmd_mode;

    yarp::dev::PolyDriver        *dd;
    yarp::dev::IPositionControl2 *ipos;
    yarp::dev::IAmplifierControl *iamp;
    yarp::dev::IControlMode2     *icmd;
    yarp::dev::IInteractionMode  *iimd;
    yarp::dev::IEncoders         *ienc;
    yarp::dev::IPositionDirect   *idir;
    yarp::dev::IVelocityControl  *ivel;
    yarp::dev::ITorqueControl    *itrq;
    yarp::dev::IRemoteVariables  *ivar;

    double  cmd_single;
    double* cmd_tot;
    double* cmd_some;

    double  prevcurr_single;
    double* prevcurr_tot;
    double* prevcurr_some;

    double* pos_tot;
};

#endif //_CONTROLMODES_H
