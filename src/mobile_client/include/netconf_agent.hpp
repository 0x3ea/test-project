#ifndef MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
#define MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_

#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Subscription.hpp>

#include <optional>
#include <string>

#include "mobile_client.hpp"

namespace MobileClient {

class MobileClient;

class NetConfAgent {
 public:
  NetConfAgent(const std::string& module_name);

  bool changeData(const std::string& xpath, const std::string& value);
  bool deleteData(const std::string& xpath);
  bool fetchData(const std::string& xpath, std::string& value);
  bool registerOperData(const std::string& xpath, MobileClient& mobile_client);
  bool subscribeForModelChanges(const std::string& xpath, MobileClient& mobile_client);

 private:
  sysrepo::Connection _conn;
  sysrepo::Session _sess;
  std::optional<sysrepo::Subscription> _sub;
  std::optional<sysrepo::Subscription> _oper_sub;
  std::string _module_name;
};

}  // namespace MobileClient

#endif  // MOBILE_NETWORK_NETCONF_AGENT_INCLUDE_HPP_
