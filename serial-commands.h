#include <StaticSerialCommands.h>
#include "display.h"

namespace serial_commands {
void cmd_help(SerialCommands& sender, Args& args) {
    sender.listCommands();
}

void cmd_set_display_period(SerialCommands& sender, Args& args) {
  unsigned long period = args[0].getInt();
  UARTDisplay::printPeriodMs = period;
}

void cmd_config(SerialCommands& sender, Args& args) {
  sender.getSerial().println("config");
}


Command commands[] {
  COMMAND(cmd_help, "help"),
  COMMAND(cmd_set_display_period, "set_display_period", ArgType::Int, nullptr, "Set the UART update period"),
  COMMAND(cmd_config, "config", nullptr, "Print config"),
};
}

SerialCommands serialCommands(Serial, serial_commands::commands, sizeof(serial_commands::commands) / sizeof(Command));