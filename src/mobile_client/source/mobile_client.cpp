#include "mobile_client.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "netconf_agent.hpp"
#include "path_constants.hpp"

namespace MobileClient {

MobileClient::MobileClient(const std::string& module_name)
    : _netconf_agent(std::make_unique<NetConfAgent>(module_name)),
      _is_registered(false) {
}

void MobileClient::handleModuleChange(const std::string& xpath, const std::string& value) {
  std::cout << "xpath: " << xpath << '\n';
  std::cout << "value: " << value << '\n';
}

bool MobileClient::registerSubscriber(const std::string& number) {
  std::string subscriber_path = getPath(kSubscriberPath, number);
  std::string tmp;
  if (_netconf_agent->fetchData(subscriber_path, tmp)) return false;
  _netconf_agent->changeData(subscriber_path, number);
  _is_registered = true;
  _number = number;
  _netconf_agent->subscribeForModelChanges(subscriber_path, *this);
  std::cout << "Subscriber with number '" << number << "' has been successfully registered!\n";
  return true;
}

std::string MobileClient::getPath(const std::string& xpath, const std::string& number) {
  std::string generated_path = xpath;
  auto found = generated_path.find("key");
  while (found != std::string::npos) {
    generated_path.replace(found, 3, number);
  }
  return generated_path;
}

}  // namespace MobileClient
