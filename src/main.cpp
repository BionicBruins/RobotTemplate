#include "main.h"
#include "srcMain.h"
#include <vector>

// TODO reduce power to all motors by 1/3 (motor * 2/3)
// TODO change from voltage to velocity

void initialize() {}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	// Put auto code here
}

namespace MotorRobot {

class MotorSet {
private:
  std::vector<int> left_motors;
  std::vector<int> right_motors;

public:
  MotorSet() {}

  MotorSet(std::vector<int> left_motors_ports,
           std::vector<int> right_motors_ports) {
    	left_motors = left_motors_ports;
    	right_motors = right_motors_ports;
  }

  void move_left_wheels(int32_t voltage) {
    for (int &port_num : left_motors) {
		// Uses C lib
		pros::c::motor_move(port_num, -voltage * config::modifier);
	}
  }

  void move_right_wheels(int32_t voltage) {
    for (int &port_num : right_motors) {
      pros::c::motor_move(port_num, voltage*config::modifier);
    }
  }
};

enum class ControlType {
  Tank,
  Arcade,
};

class Robot {
private:
  MotorSet binded;
  pros::Controller master{ pros::Controller(CONTROLLER_MASTER) };

public:
  ControlType control_type = ControlType::Tank;

public:
  Robot(MotorSet motor_set, pros::controller_id_e_t controller_binding) {
    binded = motor_set;
    master = pros::Controller(controller_binding);
  }

  Robot(std::vector<int> left_motors_ports, std::vector<int> right_motors_ports,
        pros::controller_id_e_t controller_binding) {
    binded = MotorSet(left_motors_ports, right_motors_ports);
    master = pros::Controller(controller_binding);
  }

  void tank_control() {
    binded.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
    binded.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));
  }

  void arcade_control() {
    int power = master.get_analog(ANALOG_LEFT_Y);
    int turn = master.get_analog(ANALOG_RIGHT_X);
    int left = power + turn;
    int right = power - turn;
    binded.move_left_wheels(left);
    binded.move_right_wheels(right);
  }

  /**
   * Uses the control scheme as mapped by the control_type.
   * 1 - tank control
   * 0 - arcade control
   */
  void selected_control() {
    switch (control_type) {
    case ControlType::Arcade:
      return arcade_control();
    case ControlType::Tank:
      return tank_control();
    }
  }

  void switch_control_type() {
    switch (control_type) {
    case ControlType::Arcade:
      control_type = ControlType::Tank;
      break;
    case ControlType::Tank:
      control_type = ControlType::Arcade;
      break;
    }
  }

  pros::Controller &get_controller() { return master; }

  MotorSet &get_motor_set() { return binded; }
};

class ControllerScreen {
private:
  pros::Controller master{ pros::Controller(CONTROLLER_MASTER) };
  int count = 0;

public:
  ControllerScreen() {}

  ControllerScreen(pros::controller_id_e_t controller_binding)
      : master{controller_binding} {}

  ControllerScreen(pros::Controller controller) : master(controller) {}

  void draw(int row, int column, std::string text) {
    if (!(count % 25)) {
      master.set_text(row, column, text);
    }
    count++;
  }

  void draw(std::string text) {
	static int myCount;
	if (!(myCount % 25)) {
    		master.set_text(0, 0, text);
	}
	myCount ++;
  }
};
}; // namespace motorrobot

void opcontrol() {
  MotorRobot::MotorSet motors(ports::left_ports, ports::right_ports);
  MotorRobot::Robot robot(motors, CONTROLLER_MASTER);
  MotorRobot::ControllerScreen lcd(batmobile.get_controller());

  while (true) {
    	robot.selected_control();

	if (batmobile.get_controller().get_digital(DIGITAL_Y)) {
  	      batmobile.switch_control_type();
	}
	  
    	pros::delay(2);
  }
}
