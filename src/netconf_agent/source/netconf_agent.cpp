#include "netconf_agent.hpp"

#include <sysrepo-cpp/Session.hpp>

#include <iostream>
#include <string>

NetConfAgent::NetConfAgent(const char* module_name)
    : conn(),
      sess(conn.sessionStart()) {
  sess.copyConfig(sysrepo::Datastore::Startup, module_name);
}

bool NetConfAgent::SubscribeForModelChanges(const char* module_name) {
  sysrepo::ModuleChangeCb module_change_cb = [] (sysrepo::Session session, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
    for (const auto& change : session.getChanges()) {
      std::cout << "\nChanges: " << std::string{change.node.path()} << '\n';
    }
    return sysrepo::ErrorCode::Ok;
  };
  sub = sess.onModuleChange(module_name, module_change_cb, nullptr, 0, sysrepo::SubscribeOptions::DoneOnly);
  return true;
}
