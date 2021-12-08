#include "mobile_client.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "netconf_agent.hpp"

MobileClient::MobileClient(const std::string& module_name)
    : _netconf_agent(std::make_unique<NetConfAgent>(module_name)),
      _is_registered(false) {
}

void MobileClient::handleModuleChange(const std::string& xpath, const std::string& value) {
  std::cout << "xpath: " << xpath << '\n';
  std::cout << "value: " << value << '\n';
}

bool MobileClient::registerSubscriber(const std::string& number) {
  std::string xpath = "/mobile_network:subscribers/subscriber[number='" + number + "']";
  std::string tmp;
  if (_netconf_agent->fetchData(xpath, tmp)) return false;
  _netconf_agent->changeData(xpath, number);
  _is_registered = true;
  _number = number;
  _netconf_agent->subscribeForModelChanges(xpath, *this);
  std::cout << "Subscriber with number '" << number << "' has been successfully registered!\n";
  return true;
}
