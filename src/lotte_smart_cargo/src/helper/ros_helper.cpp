#include "helper/ros_helper.h"

/**
 * @brief	    ROS Node 초기화 함수
 * @param[in]	node_name : ROS Node 이름
 */
void frb::RosHelper::initialize_ros_node()
{
  int32_t argc = 0;
  ros::init(argc, nullptr, _node_name.c_str());
  _node_handle = new ros::NodeHandle();

  set_subscriber();
  _subscribe_unit_control                     = _node_handle->subscribe("smart_cargo/unit_control", 100, &frb::RosHelper::subscribe_unit_control, this);
  _subscribe_action_complete                  = _node_handle->subscribe("smart_cargo/action_complete", 100, &frb::RosHelper::subscribe_action_complete, this);
  _subscriber_map[_subscribe_unit_control]    = "smart_cargo/unit_control";
  _subscriber_map[_subscribe_action_complete] = "smart_cargo/action_complete";
  set_publisher();
  _publish_unit_control                    = _node_handle->advertise<smart_cargo_msgs::UnitControl>("fieldro_utility/unit_control", 10, true);
  _publish_action_complete                 = _node_handle->advertise<smart_cargo_msgs::UnitActionComplete>("fieldro_utility/action_complete", 10, true);
  _publisher_map[_publish_unit_control]    = "smart_cargo/unit_control";
  _publisher_map[_publish_action_complete] = "smart_cargo/action_complete";
  set_service();

  is_created();

  _spinner = new ros::AsyncSpinner(5);
  _spinner->start();

  _logger->push_log_info("PROC", "ROS Node Initialized");
}

/**
 * @brief			ROS smart_cargo/unit_control Publish 함수
 * @param[in]	target : UnitIndex 대상
 * @param[in]	action : UnitActionIndex 동작
 * @param[in]	msg_command : 명령어
 */
void frb::RosHelper::publish_unit_control(const frb::UnitIndex& target, const frb::UnitActionIndex& action, const std::string& msg_command)
{
  if(!_publish_unit_control) return;

  smart_cargo_msgs::UnitControl msg;
  msg.time_stamp    = ros::Time::now();
  msg.sequence      = 0;
  msg.target_object = static_cast<int32_t>(target);
  msg.action        = static_cast<int32_t>(action);
  msg.command       = msg_command;

  _publish_unit_control.publish(msg);
  std::stringstream ss;
  ss << "Target: " << unit_to_string(target) << ", Action: " << action_to_string(action) << ((msg_command != "") ? ", Command: " + msg_command : "");
  _logger->push_log_info("PUB", "smart_cargo/unit_control", ss.str());
}

/**
 * @brief		  ROS smart_cargo/action_complete Publish 함수
 * @param[in]	value : smart_cargo::UnitActionComplete value
 * @param[in] receive : smart_cargo::UnitActionComplete receive object
 * @param[in] action : smart_cargo::UnitActionComplete action object
 */
void frb::RosHelper::publish_action_complete(const int32_t& value, const UnitIndex& receive_object, const UnitIndex& action_object)
{
  if(!_publish_action_complete) return;

  smart_cargo_msgs::UnitActionComplete msg;
  msg.time_stamp      = ros::Time::now();
  msg.receive_object  = static_cast<int32_t>(receive_object);
  msg.action_object   = static_cast<int32_t>(action_object);
  msg.complete_action = value;

  _publish_action_complete.publish(msg);
  _logger->push_log_info("PUB", "smart_cargo/action_complete", action_to_string(static_cast<frb::UnitActionIndex>(value)));
}

/**
 * @brief		  ROS Node 해제 함수
 */
void frb::RosHelper::delete_ros_node()
{
  _spinner->stop();
  safe_delete(_spinner);

  _node_handle->shutdown();
  safe_delete(_node_handle);

  _logger->push_log_info("PROC", "ROS Node Deleted");
}

/**
 * @brief		  ROS Subscriber/Publisher 객체 생성 여부 Logging 함수
 */
void frb::RosHelper::is_created()
{
  if(!_subscriber_map.empty())
  {
    for(const std::pair<ros::Subscriber, std::string>& pair : _subscriber_map)
    {
      if(!pair.first) _logger->push_log_error("PROC", "Fail to Create Subscriber", pair.second);
      else _logger->push_log_info("PROC", "Success to Create Subscriber", pair.second);
    }
  }
  if(!_publisher_map.empty())
  {
    for(const std::pair<ros::Publisher, std::string>& pair : _publisher_map)
    {
      if(!pair.first) _logger->push_log_error("PROC", "Fail to Create Publisher", pair.second);
      else _logger->push_log_info("PROC", "Success to Create Publisher", pair.second);
    }
  }
  if(!_service_server_map.empty())
  {
    for(const std::pair<ros::ServiceServer, std::string>& pair : _service_server_map)
    {
      if(!pair.first) _logger->push_log_error("PROC", "Fail to Create ServiceServer", pair.second);
      else _logger->push_log_info("PROC", "Success to Create ServiceServer", pair.second);
    }
  }
  if(!_service_client_map.empty())
  {
    for(const std::pair<ros::ServiceClient, std::string>& pair : _service_client_map)
    {
      if(!pair.first) _logger->push_log_error("PROC", "Fail to Create ServiceClient", pair.second);
      else _logger->push_log_info("PROC", "Success to Create ServiceClient", pair.second);
    }
  }
}