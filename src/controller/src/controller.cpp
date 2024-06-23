#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "/home/moses/marp/src/serial/serialib.h"
#include "/home/moses/marp/src/serial/serialib.cpp"
#include <unistd.h>
#include <stdio.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//define serial port
#define SERIAL_PORT "/dev/ttyACM0"
serialib serial;
char buffer[32];

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
      0ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      serial.readString(buffer, '\n', sizeof(buffer));
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
  char errorOpening = serial.openDevice(SERIAL_PORT, 19200);
  int waiting = 0;

  while (errorOpening!=1){printf("waiting on connection for %d seconds...\n", waiting++); sleep(1); errorOpening = serial.openDevice(SERIAL_PORT, 19200);}
  printf ("Successful connection to %s\n",SERIAL_PORT);

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}