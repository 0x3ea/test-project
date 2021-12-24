#include "mobile_client.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "netconf_agent.hpp"
#include "path_constants.hpp"
#include "status_enum.hpp"

namespace MobileClient {

MobileClient::MobileClient(const std::string& module_name)
    : _netconf_agent(std::make_unique<NetConfAgent>(module_name)),
      _is_registered(false) {
}

bool MobileClient::answer() {
  if (!_is_registered) {
    std::cout << "You are not registered. Register before doing any other action.\n";
    return false;
  }
  std::string callee_incoming_number_path = getPath(kIncomingNumberPath, _number);
  std::string callee_incoming_number;
  _netconf_agent->fetchData(callee_incoming_number_path, callee_incoming_number);
  if (callee_incoming_number.empty()) {
    std::cout << "No incoming call to answer.\n";
    return false;
  }
  std::string callee_status_path = getPath(kStatusPath, _number);
  std::string callee_status;
  _netconf_agent->fetchData(callee_status_path, callee_status);
  if (callee_status == mapStatusToString(Status::Active)) {
    std::cout << "Your call is already in progress.\n";
    return false;
  }
  std::string caller_status_path = getPath(kStatusPath, callee_incoming_number);
  _netconf_agent->changeData(callee_status_path, mapStatusToString(Status::Active));
  _netconf_agent->changeData(caller_status_path, mapStatusToString(Status::Active));
  return true;
}

bool MobileClient::call(const std::string& number) {
  if (!_is_registered) {
    std::cout << "You are not registered. Register before doing any other action.\n";
    return false;
  }
  std::string caller_status_path = getPath(kStatusPath, _number);
  std::string caller_status;
  _netconf_agent->fetchData(caller_status_path, caller_status);
  if (caller_status == mapStatusToString(Status::Busy) || caller_status == mapStatusToString(Status::Active)) {
    std::cout << "Your call is already in progress.\n";
    return false;
  }
  if (_number == number) {
    std::cout << "Invalid number.\n";
    return false;
  }
  std::string callee_status_path = getPath(kStatusPath, number);
  std::string callee_status;
  if (!_netconf_agent->fetchData(callee_status_path, callee_status)) {
    std::cout << "Subscriber with number '" << number << "' is not registered.\n";
    return false;
  }
  if (callee_status == mapStatusToString(Status::Busy) || callee_status == mapStatusToString(Status::Active)) {
    std::cout << "Subscriber with number '" << number << "' is busy at the moment. Please call later.\n";
    return false;
  }
  std::string caller_outgoing_number_path = getPath(kOutgoingNumberPath, _number);
  std::string callee_incoming_number_path = getPath(kIncomingNumberPath, number);
  _netconf_agent->changeData(caller_outgoing_number_path, number);
  _netconf_agent->changeData(callee_incoming_number_path, _number);
  _netconf_agent->changeData(caller_status_path, mapStatusToString(Status::Busy));
  _netconf_agent->changeData(callee_status_path, mapStatusToString(Status::Busy));
  return true;
}

bool MobileClient::callEnd() {
  if (!_is_registered) {
    std::cout << "You are not registered. Register before doing any other action.\n";
    return false;
  }
  std::string subscriber_status_path = getPath(kStatusPath, _number);
  std::string subscriber_status;
  _netconf_agent->fetchData(subscriber_status_path, subscriber_status);
  if (subscriber_status == mapStatusToString(Status::Idle) || subscriber_status == mapStatusToString(Status::Busy)) {
    std::cout << "No active call to end.\n";
    return false;
  }
  std::string subscriber_outgoing_number_path = getPath(kOutgoingNumberPath, _number);
  std::string subscriber_incoming_number_path = getPath(kIncomingNumberPath, _number);
  std::string subscriber_outgoing_number;
  std::string subscriber_incoming_number;
  _netconf_agent->fetchData(subscriber_outgoing_number_path, subscriber_outgoing_number);
  _netconf_agent->fetchData(subscriber_incoming_number_path, subscriber_incoming_number);
  if (!subscriber_outgoing_number.empty()) {
    std::string callee_incoming_number_path = getPath(kIncomingNumberPath, subscriber_outgoing_number);
    std::string callee_status_path = getPath(kStatusPath, subscriber_outgoing_number);
    _netconf_agent->changeData(callee_incoming_number_path, "");
    _netconf_agent->changeData(callee_status_path, mapStatusToString(Status::Idle));
  }
  if (!subscriber_incoming_number.empty()) {
    std::string caller_outgoing_number_path = getPath(kOutgoingNumberPath, subscriber_incoming_number);
    std::string caller_status_path = getPath(kStatusPath, subscriber_incoming_number);
    _netconf_agent->changeData(caller_outgoing_number_path, "");
    _netconf_agent->changeData(caller_status_path, mapStatusToString(Status::Idle));
  }
  _netconf_agent->changeData(subscriber_outgoing_number_path, "");
  _netconf_agent->changeData(subscriber_incoming_number_path, "");
  _netconf_agent->changeData(subscriber_status_path, mapStatusToString(Status::Idle));
  return true;
}

std::string MobileClient::getName() {
  return _name;
}

bool MobileClient::handleModuleChange(const std::string& xpath, const std::string& value) {
  if (xpath == getPath(kOutgoingNumberPath, _number) && !value.empty()) {
    std::cout << "Calling '" << value << "'...\n";
    return true;
  }
  if (xpath == getPath(kIncomingNumberPath, _number) && !value.empty()) {
    std::cout << "\nIncoming call from '" << value << "'.\nPossible actions: answer, reject\n\n";
    return true;
  }
  if (xpath == getPath(kStatusPath, _number) && value == mapStatusToString(Status::Active)) {
    std::cout << "\nCall is in progress...\nPossible actions: callEnd\n\n";
    return true;
  }
  if (xpath == getPath(kStatusPath, _number) && value == mapStatusToString(Status::Idle)) {
    std::cout << "\nCall ended.\n";
    return true;
  }
}

bool MobileClient::registerSubscriber(const std::string& number) {
  if (_is_registered) {
    std::cout << "You are already registered.\n";
    return false;
  }
  if (_name.empty()) {
    std::cout << "Set your name before registering.\n";
    return false;
  }
  std::string subscriber_status_path = getPath(kStatusPath, number);
  std::string tmp;
  if (_netconf_agent->fetchData(subscriber_status_path, tmp)) {
    std::cout << "Subscriber with number '" << number << "' is already registered. Choose another number.\n";
    return false;
  }
  std::string subscriber_path = getPath(kSubscriberPath, number);
  std::string subscriber_name_path = getPath(kUserNamePath, number);
  _netconf_agent->changeData(subscriber_path, number);
  _is_registered = true;
  _number = number;
  _netconf_agent->fetchData(subscriber_status_path, _status);
  _netconf_agent->subscribeForModelChanges(subscriber_path, *this);
  _netconf_agent->registerOperData(subscriber_name_path, *this);
  std::cout << "Subscriber with number '" << number << "' has been successfully registered!\n";
  return true;
}

bool MobileClient::reject() {
  if (!_is_registered) {
    std::cout << "You are not registered. Register before doing any other action.\n";
    return false;
  }
  std::string subscriber_status_path = getPath(kStatusPath, _number);
  std::string subscriber_status;
  _netconf_agent->fetchData(subscriber_status_path, subscriber_status);
  if (subscriber_status == mapStatusToString(Status::Idle) || subscriber_status == mapStatusToString(Status::Active)) {
    std::cout << "No incoming call to reject.\n";
    return false;
  }
  std::string subscriber_outgoing_number_path = getPath(kOutgoingNumberPath, _number);
  std::string subscriber_incoming_number_path = getPath(kIncomingNumberPath, _number);
  std::string subscriber_outgoing_number;
  std::string subscriber_incoming_number;
  _netconf_agent->fetchData(subscriber_outgoing_number_path, subscriber_outgoing_number);
  _netconf_agent->fetchData(subscriber_incoming_number_path, subscriber_incoming_number);
  if (!subscriber_outgoing_number.empty()) {
    std::string callee_incoming_number_path = getPath(kIncomingNumberPath, subscriber_outgoing_number);
    std::string callee_status_path = getPath(kStatusPath, subscriber_outgoing_number);
    _netconf_agent->changeData(callee_incoming_number_path, "");
    _netconf_agent->changeData(callee_status_path, mapStatusToString(Status::Idle));
  }
  if (!subscriber_incoming_number.empty()) {
    std::string caller_outgoing_number_path = getPath(kOutgoingNumberPath, subscriber_incoming_number);
    std::string caller_status_path = getPath(kStatusPath, subscriber_incoming_number);
    _netconf_agent->changeData(caller_outgoing_number_path, "");
    _netconf_agent->changeData(caller_status_path, mapStatusToString(Status::Idle));
  }
  _netconf_agent->changeData(subscriber_outgoing_number_path, "");
  _netconf_agent->changeData(subscriber_incoming_number_path, "");
  _netconf_agent->changeData(subscriber_status_path, mapStatusToString(Status::Idle));
  return true;
}

bool MobileClient::setName(const std::string& name) {
  _name = name;
  std::cout << "Name '" << _name << "' has been successfully set!\n";
  return true;
}

bool MobileClient::unregister() {
  if (!_is_registered) {
    std::cout << "You are not registered. Register before doing any other action.\n";
    return false;
  }
  std::string subscriber_status_path = getPath(kStatusPath, _number);
  std::string subscriber_status;
  _netconf_agent->fetchData(subscriber_status_path, subscriber_status);
  if (subscriber_status == mapStatusToString(Status::Busy) || subscriber_status == mapStatusToString(Status::Active)) {
    std::cout << "End call before unregistering.\n";
    return false;
  }
  std::string subscriber_path = getPath(kSubscriberPath, _number);
  _netconf_agent->deleteData(subscriber_path);
  _is_registered = false;
  _status = "";
  std::cout << "Subscriber with number '" << _number << "' has been successfully unregistered!\n";
  _number = "";
  return true;
}

std::string MobileClient::getPath(const std::string& xpath, const std::string& number) {
  std::string generated_path = xpath;
  auto found = generated_path.find("key");
  generated_path.replace(found, 3, number);
  return generated_path;
}

std::string MobileClient::mapStatusToString(Status status) {
  if (status == Status::Idle) return "idle";
  if (status == Status::Busy) return "busy";
  if (status == Status::Active) return "active";
}

Status MobileClient::mapStringToStatus(const std::string& status) {
  if (status == "idle") return Status::Idle;
  if (status == "busy") return Status::Busy;
  if (status == "active") return Status::Active;
}

}  // namespace MobileClient
