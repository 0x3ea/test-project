#include "mobile_client.hpp"

#include <memory>

#include "netconf_agent.hpp"

MobileClient::MobileClient()
    : _netconf_agent(std::make_unique<NetConfAgent>()) {
}
