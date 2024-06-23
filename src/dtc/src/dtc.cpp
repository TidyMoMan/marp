
#include <memory>
#include <string>

#include "/home/moses/marp/src/serial/serialib.h"
#include "/home/moses/marp/src/serial/serialib.cpp"
#include <unistd.h>
#include <stdio.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//define serial port
#define SERIAL_PORT "/dev/tty0"
serialib serial;


using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("minimal_subscriber")
    {
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 0, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const std_msgs::msg::String & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "Wrote '%s'", msg.data.c_str());
      serial.writeString(msg.data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  char errorOpening = serial.openDevice(SERIAL_PORT, 19200);

  if (errorOpening!=1) return errorOpening;
  printf ("Successful connection to %s\n",SERIAL_PORT);

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}