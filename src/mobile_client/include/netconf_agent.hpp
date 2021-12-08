#ifndef MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
#define MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_

#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Subscription.hpp>

#include <optional>
#include <string>

#include "mobile_client.hpp"

class MobileClient;

class NetConfAgent {
 public:
  NetConfAgent(const std::string& module_name);

  bool changeData(const std::string& xpath, const std::string& value);
  bool fetchData(const std::string& xpath, std::string& value);
  bool subscribeForModelChanges(const std::string& xpath, MobileClient& mobile_client);

 private:
  sysrepo::Connection _conn;
  sysrepo::Session _sess;
  std::optional<sysrepo::Subscription> _sub;
  std::string _module_name;
};

#endif  // MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
