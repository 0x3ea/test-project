#include "netconf_agent.hpp"

#include <sysrepo-cpp/Session.hpp>

#include <iostream>
#include <string>

NetConfAgent::NetConfAgent(const std::string& module_name)
    : _conn(),
      _sess(_conn.sessionStart()) {
  // _sess.copyConfig(sysrepo::Datastore::Startup, module_name.c_str());
}

bool NetConfAgent::subscribeForModelChanges(const std::string& xpath) {
  sysrepo::ModuleChangeCb module_change_cb = [] (sysrepo::Session session, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
    for (const auto& change : session.getChanges()) {
      std::cout << "\nChanges: " << std::string{change.node.path()} << '\n';
    }
    return sysrepo::ErrorCode::Ok;
  };
  _sub = _sess.onModuleChange(xpath.c_str(), module_change_cb, nullptr, 0, sysrepo::SubscribeOptions::DoneOnly);
  return true;
}

bool NetConfAgent::fetchData(const std::string& xpath, std::string& value) {
  auto term = _sess.getData(xpath.c_str())->findPath(xpath.c_str())->asTerm();
  value = term.valueStr();
  return true;
}
