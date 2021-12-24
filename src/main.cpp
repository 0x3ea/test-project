#include "mobile_client.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "path_constants.hpp"

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

bool registerSubscriber(const std::string& subscriber_number) {
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
  std::string command_line;
  std::string exit_command = "exit";
  Command command;
  Action action;
  MobileClient::MobileClient mobile_client(kModuleName);
  while (command.name != exit_command) {
    std::cout << "mobile-network > ";
    std::getline(std::cin, command_line);
    command = ParseCommandLine(command_line);
    action = MapStringToAction(command.name);
    switch (action) {
      case Action::Register: {
        mobile_client.registerSubscriber(command.argument);
        break;
      }
      case Action::Unregister: {
        mobile_client.unregister();
        break;
      }
      case Action::SetName: {
        mobile_client.setName(command.argument);
        break;
      }
      case Action::Call: {
        mobile_client.call(command.argument);
        break;
      }
      case Action::CallEnd: {
        mobile_client.callEnd();
        break;
      }
      case Action::Answer: {
        mobile_client.answer();
        break;
      }
      case Action::Reject: {
        mobile_client.reject();
        break;
      }
      case Action::Exit: {
        return 0;
      }
      default: {
        std::cout << command.name << ": command not found\n";
      }
    }
  }
  return 0;
}
