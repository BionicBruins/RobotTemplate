#include "main.h"
#include "main.hpp"
#include <vector>

namespace MotorRobot
{

	MotorSet::MotorSet() {}

	MotorSet::MotorSet(std::vector<int> &left_motors_ports,
					   std::vector<int> &right_motors_ports)
	{
		left_motors = left_motors_ports;
		right_motors = right_motors_ports;
	}

	void MotorSet::move_left_wheels(int32_t voltage)
	{
		for (int &port_num : left_motors)
		{
			// Uses C lib
			pros::c::motor_move_velocity(port_num, -voltage * config::scale * config::modifier);
		}
	}

	void MotorSet::move_right_wheels(int32_t voltage)
	{
		for (int &port_num : right_motors)
		{
			// Uses C lib
			pros::c::motor_move_velocity(port_num, voltage * config::scale * config::modifier);
		}
	}

	Robot::Robot() {}

	Robot::Robot(MotorSet &motor_set, pros::controller_id_e_t controller_binding)
	{
		binded = motor_set;
		master = pros::Controller(controller_binding);
	}

	Robot::Robot(std::vector<int> &left_motors_ports, std::vector<int> &right_motors_ports,
				 pros::controller_id_e_t controller_binding)
	{
		binded = MotorSet(left_motors_ports, right_motors_ports);
		master = pros::Controller(controller_binding);
	}

	void Robot::tank_control()
	{
		binded.move_left_wheels(master.get_analog(ANALOG_LEFT_Y));
		binded.move_right_wheels(master.get_analog(ANALOG_RIGHT_Y));
	}

	void Robot::arcade_control()
	{
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);
		int left = power + turn;
		int right = power - turn;
		binded.move_left_wheels(left);
		binded.move_right_wheels(right);
	}

	void Robot::selected_control()
	{
		switch (control_type)
		{
		case ControlType::Arcade:
			return arcade_control();
		case ControlType::Tank:
			return tank_control();
		}
	}

	void Robot::switch_control_type()
	{
		switch (control_type)
		{
		case ControlType::Arcade:
			control_type = ControlType::Tank;
			break;
		case ControlType::Tank:
			control_type = ControlType::Arcade;
			break;
		}
	}

	pros::Controller &Robot::get_controller() { return master; }

	MotorSet &Robot::get_motor_set() { return binded; }

	ControllerScreen::ControllerScreen() {}

	ControllerScreen::ControllerScreen(pros::controller_id_e_t controller_binding)
		: master{controller_binding} {}

	ControllerScreen::ControllerScreen(pros::Controller &controller) : master(controller) {}

	void ControllerScreen::draw(int row, int column, std::string &text)
	{
		master.set_text(row, column, text);
		pros::delay(50);
	}

	void ControllerScreen::draw(std::string &text)
	{
		master.set_text(0, 0, text);
		pros::delay(50);
	}

	std::string &ControllerScreen::control_type_to_string(ControlType &control_type) const {
		switch (control_type) {
			case ControlType::Tank: return "Tank";
			case ControlType::Arcade: return "Arcade";
		}
	}
}; // namespace motorrobot


MotorRobot::MotorSet motors(ports::left_ports, ports::right_ports);
MotorRobot::Robot robot(motors, CONTROLLER_MASTER);
MotorRobot::ControllerScreen lcd(robot.get_controller());

void display() { 
	lcd.draw("DM (Y): " + lcd.control_type_to_string(robot.control_type));
}

void initialize() {
	pros::Task display_task (display);
}

void disabled() {}

void competition_initialize() {}

void autonomous()
{
	// Put auto code here
}


void opcontrol()
{
	while (true)
	{
		robot.selected_control();

		if (robot.get_controller().get_digital(DIGITAL_Y))
			robot.switch_control_type();

		pros::delay(2);
	}
}
