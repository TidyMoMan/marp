#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "/home/moses/marp/src/serialib/lib/serialib.h"
#include "/home/moses/marp/src/serialib/lib/serialib.cpp"
#include <unistd.h>
#include <stdio.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//define serial port
#define SERIAL_PORT "/dev/ttyACM0"
serialib serial;

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      timer_ = this->create_wall_timer(
      10ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      char buffer[32];
      auto message = std_msgs::msg::String();
      serial.readString(buffer, '\n', sizeof(buffer), 2000);
      message.data = buffer;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}