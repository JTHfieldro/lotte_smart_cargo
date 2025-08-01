#include "rmd.h"

void frb::Rmd::get_position()
{
  _send_packet_callback(set_packet_cmd(frb::RmdDefine::MReadAbsPos));
  _logger->push_log_info("PROC", "Get Current Position");
}