#ifndef MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
#define MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_

#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Subscription.hpp>

#include <optional>
#include <string>

class NetConfAgent {
 public:
  NetConfAgent(const std::string& module_name);

  bool subscribeForModelChanges(const std::string& xpath);
  bool fetchData(const std::string& xpath, std::string& value);

 private:
  sysrepo::Connection _conn;
  sysrepo::Session _sess;
  std::optional<sysrepo::Subscription> _sub;
};

#endif  // MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
