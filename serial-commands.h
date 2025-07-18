#include <StaticSerialCommands.h>
#include "display.h"
#include "config.h"

namespace serial_commands {
void cmd_help(SerialCommands& sender, Args& args) {
  sender.listCommands();
}

void cmd_set_display_period(SerialCommands& sender, Args& args) {
  unsigned long period = args[0].getInt();
  UARTDisplay::printPeriodMs = period;
}

void cmd_config_print(SerialCommands& sender, Args& args) {
  sender.getSerial().print("throttle_min: ");
  sender.getSerial().println(config.throttle_min);
  sender.getSerial().print("throttle_max: ");
  sender.getSerial().println(config.throttle_max);
  sender.getSerial().print("pas_detection_time_ms: ");
  sender.getSerial().println(config.pas_detection_time_ms);
  sender.getSerial().print("pas_rising_edge_count: ");
  sender.getSerial().println(config.pas_rising_edge_count);
  sender.getSerial().print("foot_mode_timeout_sec: ");
  sender.getSerial().println(config.foot_mode_timeout_sec);
}

void cmd_config_read(SerialCommands& sender, Args& args) {
  config.eepromRead();
  cmd_config_print(sender, args);
}

void cmd_config_save(SerialCommands& sender, Args& args) {
  config.eepromUpdate();
  cmd_config_print(sender, args);
}

void cmd_config_set_default(SerialCommands& sender, Args& args) {
  config.setDefault();
  cmd_config_print(sender, args);
}

void cmd_config_set_throttle_max(SerialCommands& sender, Args& args) {
  decltype(config.throttle_max) value = args[0].getInt();
  config.throttle_max = value;
}

void cmd_config_set_throttle_min(SerialCommands& sender, Args& args) {
  decltype(config.throttle_min) value = args[0].getInt();
  config.throttle_min = value;
}

void cmd_config_set_pas_detection_time_ms(SerialCommands& sender, Args& args) {
  decltype(config.pas_detection_time_ms) value = args[0].getInt();
  config.pas_detection_time_ms = value;
}

void cmd_config_set_pas_rising_edge_count(SerialCommands& sender, Args& args) {
  int value = args[0].getInt();
  if (value < 0 || value >= 32) {
    sender.getSerial().print("cannot set rising edge count. Illegal value provided: ");
    sender.getSerial().println(value);
  }
  config.pas_rising_edge_count = decltype(config.pas_rising_edge_count) (value);
}

void cmd_config_set_foot_mode_timeout_sec(SerialCommands& sender, Args& args) {
  decltype(config.foot_mode_timeout_sec) value = args[0].getInt();
  config.foot_mode_timeout_sec = value;
}

Command cfgCommands[]{
  COMMAND(cmd_config_print, "print"),
  COMMAND(cmd_config_read, "read", nullptr, "Read from EEPROM"),
  COMMAND(cmd_config_save, "save", nullptr, "Save to EEPROM"),
  COMMAND(cmd_config_set_default, "set_default", nullptr, "Reset config to default value"),
  COMMAND(cmd_config_set_throttle_min, "set_throttle_min", ArgType::Int, nullptr, ""),
  COMMAND(cmd_config_set_throttle_max, "set_throttle_max", ArgType::Int, nullptr, ""),
  COMMAND(cmd_config_set_pas_detection_time_ms, "set_pas_detection_time_ms", ArgType::Int, nullptr, ""),
  COMMAND(cmd_config_set_pas_rising_edge_count, "set_pas_rising_edge_count", ArgType::Int, nullptr, ""),
  COMMAND(cmd_config_set_foot_mode_timeout_sec, "foot_mode_timeout_sec", ArgType::Int, nullptr, ""),
};

void cmd_config(SerialCommands& sender, Args& args) {
  sender.listAllCommands(cfgCommands, sizeof(cfgCommands) / sizeof(Command));
}

Command mainCommands[]{
  COMMAND(cmd_help, "help"),
  COMMAND(cmd_set_display_period, "set_display_period", ArgType::Int, nullptr, "Set the UART update period"),
  COMMAND(cmd_config, "config", cfgCommands),
};
}

SerialCommands serialCommands(Serial, serial_commands::mainCommands, sizeof(serial_commands::mainCommands) / sizeof(Command));