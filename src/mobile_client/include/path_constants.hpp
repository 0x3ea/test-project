#ifndef MOBILE_NETWORK_PATH_CONSTANTS_INCLUDE_HPP_
#define MOBILE_NETWORK_PATH_CONSTANTS_INCLUDE_HPP_

#include <string>

namespace {

const std::string kModuleName = "mobile_network";
const std::string kSubscriberPath = "/" + kModuleName + ":subscribers/subscriber[number='key']";
const std::string kUserNamePath = kSubscriberPath + "/userName";
const std::string kNumberPath = kSubscriberPath + "/number";
const std::string kOutgoingNumberPath = kSubscriberPath + "/outgoingNumber";
const std::string kIncomingNumberPath = kSubscriberPath + "/incomingNumber";
const std::string kStatusPath = kSubscriberPath + "/status";

}  // namespace

#endif  // MOBILE_NETWORK_PATH_CONSTANTS_INCLUDE_HPP_
