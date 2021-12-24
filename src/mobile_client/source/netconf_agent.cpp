#include "netconf_agent.hpp"

#include <libyang-cpp/DataNode.hpp>
#include <sysrepo-cpp/Session.hpp>

#include <optional>
#include <string>

#include "mobile_client.hpp"

namespace MobileClient {

NetConfAgent::NetConfAgent(const std::string& module_name)
    : _conn(),
      _sess(_conn.sessionStart()),
      _module_name(module_name) {
}

bool NetConfAgent::changeData(const std::string& xpath, const std::string& value) {
  _sess.setItem(xpath.c_str(), value.c_str());
  _sess.applyChanges();
  return true;
}

bool NetConfAgent::deleteData(const std::string& xpath) {
  _sess.deleteItem(xpath.c_str());
  _sess.applyChanges();
  return true;
}

bool NetConfAgent::fetchData(const std::string& xpath, std::string& value) {
  auto data = _sess.getData(xpath.c_str());
  if (!data.has_value()) return false;
  value = data->findPath(xpath.c_str())->asTerm().valueStr();
  return true;
}

bool NetConfAgent::registerOperData(const std::string& xpath, MobileClient& mobile_client) {
  sysrepo::OperGetItemsCb oper_get_items_cb = [&] (sysrepo::Session, auto, auto, auto, auto, auto, std::optional<libyang::DataNode>& parent) -> sysrepo::ErrorCode {
    parent = _sess.getContext().newPath(xpath.c_str(), mobile_client.getName().c_str());
    return sysrepo::ErrorCode::Ok;
  };
  _oper_sub = _sess.onOperGetItems(_module_name.c_str(), oper_get_items_cb, xpath.c_str());
  return true;
}

bool NetConfAgent::subscribeForModelChanges(const std::string& xpath, MobileClient& mobile_client) {
  sysrepo::ModuleChangeCb module_change_cb = [&mobile_client] (sysrepo::Session session, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
    for (const auto& change : session.getChanges()) {
      if (change.node.schema().nodeType() == libyang::NodeType::Leaf) {
        mobile_client.handleModuleChange(std::string{change.node.path()}, std::string{change.node.asTerm().valueStr()});
      }
    }
    return sysrepo::ErrorCode::Ok;
  };
  _sub = _sess.onModuleChange(_module_name.c_str(), module_change_cb, xpath.c_str(), 0, sysrepo::SubscribeOptions::DoneOnly);
  return true;
}

}  // namespace MobileClient
