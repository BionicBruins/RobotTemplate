#ifndef SRC_MAIN
#define SRC_MAIN

namespace MotorRobot {
    class MotorSet;
    class Robot;
    class ControllerScreen;
}

namespace config {
  constexpr int max_speed = 100;
  constexpr double modifier = 2/3;
  constexpr double scale = (200/127);
}

namespace ports {
    // Change these depending on ports
    constexpr int left_ports_top = 20;
    constexpr int left_ports_bottom = 17;

    constexpr int right_ports_top = 19;
    constexpr int right_ports_bottom = 18;  

    std::vector<int> left_ports = std::vector<int> {ind_ports::left_ports_top, ind_ports::left_ports_bottom};
    std::vector<int> right_ports = std::vector<int> {ind_ports::right_ports_top, ind_ports::right_ports_bottom};
}
#endif
