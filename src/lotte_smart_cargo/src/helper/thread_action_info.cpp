#include "thread_action_info.h"

frb::ThreadActionInfo::ThreadActionInfo(std::string file_name)
{
  _active = false;
  _hz     = 10;
  _sleep  = 0;
  load_option(file_name);
}

frb::ThreadActionInfo::~ThreadActionInfo()
{
  _active = false;
  _thread.join();
}

void frb::ThreadActionInfo::load_option(std::string file_name)
{
  try
  {
    // file open
    std::ifstream yaml_file(file_name);
    YAML::Node    yaml = YAML::Load(yaml_file);
    yaml_file.close();

    // data read
    std::string config_name = yaml["config_name"].as<std::string>();
    // std::cout << config_name << std::endl;

    // thread 동작 속도
    _hz = yaml["thread"]["hz"].as<int32_t>();
  }
  catch(YAML::Exception& e)
  {
    // ROS_ERROR_STREAM("YAML Exception" << e.what());
  }
  catch(std::exception& e)
  {
    // ROS_ERROR_STREAM("Standard Exception" << e.what());
  }
  catch(...)
  {
    // ROS_ERROR_STREAM("Unknown Exception");
  }

  // _thread_sleep 계산의 경우 설정되는 _thread_hz * 1.1 정도로 설정
  // 해줘야 실제 thread 동작 속도가 _thread_hz에 가까워진다.
  // thread 내에서 동작하는 각종 구현 함수의 overhead 때문에
  _sleep = 1000 / (_hz * 1.1);
}