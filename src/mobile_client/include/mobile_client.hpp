#ifndef MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_
#define MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_

#include <memory>
#include <string>

#include "netconf_agent.hpp"

class MobileClient {
 public:
  MobileClient();

 private:
  std::unique_ptr<NetConfAgent> _netconf_agent;
  std::string _subscriber_name;
  std::string _subscriber_number;
  std::string _status;
};

#endif  // MOBILE_NETWORK_MOBILE_CLIENT_INCLUDE_HPP_
