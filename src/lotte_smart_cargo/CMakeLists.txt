cmake_minimum_required(VERSION 3.0.0)

project(
    lotte_smart_cargo # 프로젝트 이름
    VERSION 0.1.0 # 프로젝트 버전
    DESCRIPTION "Lotte Smart Cargo ROS package" # 프로젝트 설명
    LANGUAGES CXX # 사용할 언어
)

set(CMAKE_CXX_STANDARD 11) # C++ 표준 설정

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake") # cmake 모듈 경로 추가

find_package(Threads REQUIRED) # 쓰레드 라이브러리 찾기
find_package(yaml-cpp CONFIG REQUIRED) # YAML 라이브러리 찾기
find_package(Boost REQUIRED COMPONENTS filesystem system) # Boost 라이브러리 찾기

# ROS 패키지 의존성 설정
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  can_msgs
)

# 메시지 정의
add_message_files(
  FILES
)

# 서비스 정의
add_service_files(
    DEPENDENCIES
    std_msgs
)

# 메시지 및 서비스 생성
catkin_package(
    CATKIN_DEPENDS roscpp rospy std_msgs message_runtime can_msgs # 의존성 패키지
    INCLUDE_DIRS include # 헤더 파일 경로
)

include_directories(
    include # 헤더 파일 경로
    include/communication # 통신 관련 헤더 파일 경로
    include/define # 정의 관련 헤더 파일 경로
    include/door
    include/helper
    include/io
    include/smart_cargo
    ${catkin_INCLUDE_DIRS} # catkin 포함 디렉토리
    ${Boost_INCLUDE_DIRS} # Boost 포함 디렉토리
)