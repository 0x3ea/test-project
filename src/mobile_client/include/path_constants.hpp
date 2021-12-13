#include <string>

namespace {

const std::string kModuleName = "mobile_network";
const std::string kSubscriberPath = "/" + kModuleName + ":subscribers/subscriber[number='key']";
const std::string kNumberPath = kSubscriberPath + "/number";
const std::string kStatePath = kSubscriberPath + "/state";
const std::string kIncomingNumberPath = kSubscriberPath + "/incomingNumber";

}  // namespace
