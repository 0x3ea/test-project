#ifndef MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
#define MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_

#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Subscription.hpp>

#include <optional>

class NetConfAgent {
 public:
  NetConfAgent(const char* module_name);

  bool SubscribeForModelChanges(const char* module_name);

 private:
  sysrepo::Connection conn;
  sysrepo::Session sess;
  std::optional<sysrepo::Subscription> sub;
};

#endif  // MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
