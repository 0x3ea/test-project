#ifndef MOBILE_NETWORK_NETCONF_AGENT_MOCK_INCLUDE_HPP_
#define MOBILE_NETWORK_NETCONF_AGENT_MOCK_INCLUDE_HPP_

#include "INetConfAgent.hpp"

#include <gmock/gmock.h>

namespace Mock {

class NetConfAgentMock : public MobileClient::INetConfAgent {
 public:
  MOCK_METHOD2(changeData, bool(const std::string& xpath, const std::string& value));
  MOCK_METHOD1(deleteData, bool(const std::string& xpath));
  MOCK_METHOD2(fetchData, bool(const std::string& xpath, std::string& value));
  MOCK_METHOD2(registerOperData, bool(const std::string& xpath, MobileClient& mobile_client));
  MOCK_METHOD2(subscribeForModelChanges, bool(const std::string& xpath, MobileClient& mobile_client));
};

}  // namespace Mock

#endif  // MOBILE_NETWORK_NETCONF_AGENT_MOCK_INCLUDE_HPP_
