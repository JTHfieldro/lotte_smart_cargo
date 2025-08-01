#pragma once

#include <smart_cargo_libs/define/unit_action_index.h>
#include <smart_cargo_libs/define/unit_index.h>
#include <smart_cargo_libs/helper/global_helper.h>
#include <smart_cargo_libs/helper/logger.h>
#include <smart_cargo_libs/helper/thread_action_info.h>
#include <smart_cargo_msgs/UnitActionComplete.h>
#include <smart_cargo_msgs/UnitControl.h>

#include <map>
#include <string>

#include "ros/ros.h"

namespace frb  // fieldro_bot
{
/**
 * @author
 * @date
 * @brief
 * @note
 * @details
 * @see
 */
class RosHelper
{
public:
  RosHelper(const std::string& node_name, const std::string& config_path, Logger* logger)
      : _node_name(node_name), _config_path(config_path), _logger(logger)
  {
    _is_all_unit_init = false;
    _shut_down        = false;
  }

  virtual ~RosHelper()
  {
    if(_subscribe_unit_control) _subscribe_unit_control.shutdown();
    if(_subscribe_action_complete) _subscribe_action_complete.shutdown();

    // 약간의 지연을 두어 진행 중인 콜백이 완료되도록 함
    ros::Duration(0.1).sleep();

    safe_delete(_thread_info);
    delete_ros_node();
  }

public:
  /**
   * @brief      객체 소멸 예약 여부 반환
   * @return     객체 소멸 예약 flag
   */
  bool is_shutdown() { return _shut_down; }

protected:
  ros::NodeHandle*   _node_handle;  ///< ROS Node Handle
  ros::AsyncSpinner* _spinner;      ///< ROS Async Spinner
  frb::Logger*       _logger;       ///< Logger 객체

  std::map<ros::Subscriber, std::string>    _subscriber_map;      ///< Subscriber Map
  std::map<ros::Publisher, std::string>     _publisher_map;       ///< Publisher Map
  std::map<ros::ServiceServer, std::string> _service_server_map;  ///< ServiceServer Map
  std::map<ros::ServiceClient, std::string> _service_client_map;  ///< ServiceClient Map

  frb::ThreadActionInfo*         _thread_info;  ///< thread 동작 정보 객체
  std::map<std::string, int32_t> _command_map;  ///< 명령어 맵
  std::string                    _node_name;    ///< ROS Node 이름 저장 객체
  std::string                    _config_path;  ///< Config File 경로

  bool _is_all_unit_init;  ///< 모든 Unit 초기화 여부
  bool _shut_down;         ///< 종료 flag

  ros::Subscriber _subscribe_unit_control;     ///< smart_cargo/unit_control Subscriber
  ros::Subscriber _subscribe_action_complete;  ///< smart_cargo/action_complete Subscriber

  ros::Publisher _publish_unit_control;     ///< smart_cargo/unit_control Pulibsher
  ros::Publisher _publish_action_complete;  ///< smart_cargo/action_complete Pulibsher

protected:
  /**
   * @brief      객체 소멸 예약
   * @note       예약이 되면 object를 소유하고 있는 쪽에서 is_shutdown()을 확인하여 소멸시킨다.
   */
  void destroy() { _shut_down = true; }

  virtual void update()      = 0;  ///< 실제 Thread 동작 함수
  virtual void initialize()  = 0;  ///< 초기화 함수
  virtual void load_option() = 0;  ///< Config 설정 파일 로드 함수

  void initialize_ros_node();  ///< ROS Node 초기화 함수

  virtual void set_subscriber() = 0;  ///< ROS Subscriber 설정 함수
  virtual void set_publisher()  = 0;  ///< ROS Publisher 설정 함수
  virtual void set_service()    = 0;  ///< ROS Service 설정 함수

  virtual void subscribe_unit_control(const smart_cargo_msgs::UnitControl::ConstPtr& control_msg)            = 0;                            ///< ROS smart_cargo/unit_control Subscriber Callback 함수
  virtual void subscribe_action_complete(const smart_cargo_msgs::UnitActionComplete::ConstPtr& complete_msg) = 0;                            ///< ROS smart_cargo/action_complete Subscriber Callback 함수
  void         publish_unit_control(const frb::UnitIndex& target, const frb::UnitActionIndex& action, const std::string& msg_command = "");  ///< ROS smart_cargo/unit_control Publish 함수
  void         publish_action_complete(const int32_t& value, const UnitIndex& receive_object, const UnitIndex& action_object);               ///< ROS smart_cargo/action_complete Publish 함수

  void delete_ros_node();  ///< ROS Node 해제 함수
  void is_created();       ///< ROS Subscriber/Publisher 객체 생성 여부 Logging 함수
};
}  // namespace frb