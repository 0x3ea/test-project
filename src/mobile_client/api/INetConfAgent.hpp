#ifndef MOBILE_NETWORK_INETCONF_AGENT_INCLUDE_HPP_
#define MOBILE_NETWORK_INETCONF_AGENT_INCLUDE_HPP_

#include <optional>
#include <string>

#include "mobile_client.hpp"

namespace MobileClient {

class MobileClient;

class INetConfAgent {
 public:
  virtual bool changeData(const std::string& xpath, const std::string& value) = 0;
  virtual bool deleteData(const std::string& xpath) = 0;
  virtual bool fetchData(const std::string& xpath, std::string& value) = 0;
  virtual bool registerOperData(const std::string& xpath, MobileClient& mobile_client) = 0;
  virtual bool subscribeForModelChanges(const std::string& xpath, MobileClient& mobile_client) = 0;
};

}  // namespace MobileClient

#endif  // MOBILE_NETWORK_INETCONF_AGENT_INCLUDE_HPP_
