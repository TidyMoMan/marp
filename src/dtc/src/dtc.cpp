#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "/home/moses/marp/src/serialib/lib/serialib.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//define serial port
#define SERIAL_PORT "/dev/ttyACM0"

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
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  serialib serial;
  char errorOpening = serial.openDevice(SERIAL_PORT, 19200);

  if (errorOpening!=1) return errorOpening;
  printf ("Successful connection to %s\n",SERIAL_PORT);



  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}