#ifndef MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_
#define MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_

#include <memory>
#include <string>

#include "netconf_agent.hpp"

class NetConfAgent;

class MobileClient {
 public:
  MobileClient(const std::string& module_name);

  void handleModuleChange(const std::string& xpath, const std::string& value);
  bool registerSubscriber(const std::string& number);

 private:
  std::unique_ptr<NetConfAgent> _netconf_agent;
  bool _is_registered;
  std::string _number;
  std::string _status;
};

#endif  // MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_
