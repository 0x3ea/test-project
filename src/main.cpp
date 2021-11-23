#include "netconf_agent.hpp"

#include <iostream>
#include <sstream>
#include <string>

struct Command {
  std::string name;
  std::string argument;
};

enum class Action {
  Register,
  Unregister,
  SetName,
  Call,
  CallEnd,
  Answer,
  Reject,
  Exit,
  Error,
};

bool Register(const std::string& subscriber_number) {
  std::cout << "Register(" << subscriber_number << ");\n";
  return true;
}

bool Unregister() {
  std::cout << "Unregister();\n";
  return true;
}

void SetName(const std::string& subscriber_name) {
  std::cout << "SetName(" << subscriber_name << ");\n";
}

bool Call(const std::string& receiver_number) {
  std::cout << "Call(" << receiver_number << ");\n";
  return true;
}

void CallEnd() {
  std::cout << "CallEnd();\n";
}

void Answer() {
  std::cout << "Answer();\n";
}

void Reject() {
  std::cout << "Reject();\n";
}

bool Exit() {
  std::cout << "Exit();\n";
  return true;
}

Command ParseCommandLine(const std::string& command_line) {
  Command command;
  std::istringstream command_line_stream(command_line);
  command_line_stream >> command.name >> command.argument;
  return command;
}

Action MapStringToAction(const std::string& command_name) {
  if (command_name == "register") return Action::Register;
  if (command_name == "unregister") return Action::Unregister;
  if (command_name == "setName") return Action::SetName;
  if (command_name == "call") return Action::Call;
  if (command_name == "callEnd") return Action::CallEnd;
  if (command_name == "answer") return Action::Answer;
  if (command_name == "reject") return Action::Reject;
  if (command_name == "exit") return Action::Exit;
  return Action::Error;
}

int main() {
  NetConfAgent netconf_agent("mobile_network");
  netconf_agent.SubscribeForModelChanges("mobile_network");
  std::string command_line;
  std::string exit_command = "exit";
  Command command;
  Action action;
  while (command.name != exit_command) {
    std::cout << "mobile-network > ";
    std::getline(std::cin, command_line);
    command = ParseCommandLine(command_line);
    action = MapStringToAction(command.name);
    switch (action) {
      case Action::Register: {
        Register(command.argument);
        break;
      }
      case Action::Unregister: {
        Unregister();
        break;
      }
      case Action::SetName: {
        SetName(command.argument);
        break;
      }
      case Action::Call: {
        Call(command.argument);
        break;
      }
      case Action::CallEnd: {
        CallEnd();
        break;
      }
      case Action::Answer: {
        Answer();
        break;
      }
      case Action::Reject: {
        Reject();
        break;
      }
      case Action::Exit: {
        Exit();
        break;
      }
      default: {
        std::cout << command.name << ": command not found\n";
      }
    }
  }
  return 0;
}
