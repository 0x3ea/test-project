#include "netconf_agent.hpp"

#include <sysrepo-cpp/Session.hpp>

#include <string>

#include "mobile_client.hpp"

NetConfAgent::NetConfAgent(const std::string& module_name)
    : _conn(),
      _sess(_conn.sessionStart()),
      _module_name(module_name) {
}

bool NetConfAgent::changeData(const std::string& xpath, const std::string& value) {
  _sess.setItem(xpath.c_str(), value.c_str());
  return true;
}

bool NetConfAgent::fetchData(const std::string& xpath, std::string& value) {
  auto data = _sess.getData(xpath.c_str());
  if (!data.has_value()) return false;
  value = data->findPath(xpath.c_str())->asTerm().valueStr();
  return true;
}

bool NetConfAgent::subscribeForModelChanges(const std::string& xpath, MobileClient& mobile_client) {
  sysrepo::ModuleChangeCb module_change_cb = [&mobile_client] (sysrepo::Session session, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
    for (const auto& change : session.getChanges()) {
      mobile_client.handleModuleChange(std::string{change.node.path()}, "value");
    }
    return sysrepo::ErrorCode::Ok;
  };
  _sub = _sess.onModuleChange(_module_name.c_str(), module_change_cb, xpath.c_str(), 0, sysrepo::SubscribeOptions::DoneOnly);
  return true;
}
