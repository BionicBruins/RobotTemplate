#pragma once

#include <vector>

namespace MotorRobot
{
	enum class ControlType
	{
		Tank,
		Arcade,
	};

	class MotorSet
	{
	private:
		std::vector<int> left_motors;
		std::vector<int> right_motors;

	public:
		MotorSet();

		MotorSet(std::vector<int> &left_motors_ports, std::vector<int> &right_motors_ports);

		void move_left_wheels(int32_t voltage);

		void move_right_wheels(int32_t voltage);
	};

	class Robot
	{
	private:
		MotorSet binded;
		pros::Controller master{pros::Controller(CONTROLLER_MASTER)};

	public:
		ControlType control_type = ControlType::Tank;

		Robot();

		Robot(MotorSet &motor_set, pros::controller_id_e_t controller_binding);

		Robot(std::vector<int> &left_motors_ports, std::vector<int> &right_motors_ports, pros::controller_id_e_t controller_binding);

		void tank_control();

		void arcade_control();

		/**
			 * Uses the control scheme as mapped by the control_type.
			 * 1 - tank control
			 * 0 - arcade control
			 */
		void selected_control();

		void switch_control_type();

		pros::Controller &get_controller();

		MotorSet &get_motor_set();
	};

	class ControllerScreen
	{
	private:
		pros::Controller master{pros::Controller(CONTROLLER_MASTER)};

	public:
		ControllerScreen();

		ControllerScreen(pros::controller_id_e_t controller_binding);

		ControllerScreen(pros::Controller &controller);

		void draw(int row, int column, std::string &text);

		void draw(std::string &text);

		const std::string &control_type_to_string(ControlType &control_type);
	};
}

namespace config
{
	constexpr int max_speed = 100;
	constexpr double modifier = 2 / 3;
	constexpr double scale = (200 / 127);
}

namespace ports
{
	// Change these depending on ports
	constexpr int left_ports_top = 20;
	constexpr int left_ports_bottom = 17;

	constexpr int right_ports_top = 19;
	constexpr int right_ports_bottom = 18;

	std::vector<int> left_ports = std::vector<int>{left_ports_top, left_ports_bottom};
	std::vector<int> right_ports = std::vector<int>{right_ports_top, right_ports_bottom};
}
