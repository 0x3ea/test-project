#include "NetConfAgentMock.hpp"

#include <gtest/gtest.h>
#include <string>

#include "mobile_client.hpp"

using testing::_;
using testing::AtLeast;
using testing::DoAll;
using testing::Return;
using testing::SetArgReferee;
using testing::StrictMock;

namespace {

const std::string kModuleName = "mobile_network";

const std::string kCallerNumber = "910";
const std::string kCallerPath = "/" + kModuleName + ":subscribers/subscriber[number='" + kCalleeNumber + "']";
const std::string kCallerUserNamePath = kCallerPath + "/userName";
const std::string kCallerNumberPath = kCallerPath + "/number";
const std::string kCallerOutgoingNumberPath = kCallerPath + "/outgoingNumber";
const std::string kCallerIncomingNumberPath = kCallerPath + "/incomingNumber";
const std::string kCallerStatusPath = kCallerPath + "/status";

const std::string kCalleeNumber = "911";
const std::string kCalleePath = "/" + kModuleName + ":subscribers/subscriber[number='" + kCalleeNumber + "']";
const std::string kCalleeUserNamePath = kCalleePath + "/userName";
const std::string kCalleeNumberPath = kCalleePath + "/number";
const std::string kCalleeOutgoingNumberPath = kCalleePath + "/outgoingNumber";
const std::string kCalleeIncomingNumberPath = kCalleePath + "/incomingNumber";
const std::string kCalleeStatusPath = kCalleePath + "/status";

const std::string kStatusIdle = "idle";
const std::string kStatusBusy = "busy";
const std::string kStatusActive = "active";

}

namespace Test {

class MobileClientTest : public testing::Test {
 protected:
  void SetUp() override {
    auto tempMock = std::make_unique<StrictMock<NetConfAgentMock>>();
    _mock = tempMock.get();
    _client = std::make_unique<MobileClient::MobileClient>(std::move(tempMock));
  }

  void registerSubscriber() {
    EXPECT_CALL(*_mock, fetchData(kCallerNumberPath, _)).WillOnce(Return(false));
    EXPECT_CALL(*_mock, changeData(kCallerNumberPath, kCallerNumber)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges).WillOnce(Return(true));
    EXPECT_CALL(*_mock, registerOperData).WillOnce(Return(true));
    EXPECT_TRUE(_client->registerSubscriber(kCallerNumber));
  }

  void call() {
    EXPECT_CALL(*_mock, fetchData(kCalleeNumber, _)).WillOnce(DoAll(SetArgReferee<1>(kCalleeNumber), Return(true)));
    EXPECT_CALL(*_mock, fetchData(kCallerStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(true)));
    EXPECT_CALL(*_mock, fetchData(kCalleeStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(true)));
    EXPECT_CALL(*_mock, changeData(kCallerOutgoingNumberPath, kCalleeNumber));
    EXPECT_CALL(*_mock, changeData(kCalleeIncomingNumberPath, kCallerNumber));
    EXPECT_CALL(*_mock, changeData(kCalleeStatusPath, kStatusBusy));
    EXPECT_CALL(*_mock, changeData(kCallerStatusPath, kStatusBusy));
    EXPECT_TRUE(_client->call(kCalleeNumber));
  }

  StrictMock<NetConfAgentMock> *_mock;
  std::unique_ptr<MobileClient::MobileClient> _client;
};

TEST_F(MobileClientTest, TestRegisterSUbscriberReturnTrue) {
  registerSubscriber();
}

TEST_F(MobileClientTest, TestRegisterSubscriberReturnFalse) {
  EXPECT_CALL(*_mock, fetchData(kCallerStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kCallerNumber), Return(true)));
  EXPECT_FALSE(_client->registerSubscriber(kCallerNumber));
}

TEST_F(MobileClientTest, TestCallTrue) {
  registerSubcriber();
  EXPECT_CALL(*_mock, fetchData(kCalleeNumber, _)).WillOnce(DoAll(SetArgReferee<1>(kCalleeNumber), Return(true)));
  EXPECT_CALL(*_mock, fetchData(kCallerStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(true)));
  EXPECT_CALL(*_mock, fetchData(kCalleeStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(true)));
  EXPECT_CALL(*_mock, changeData(kCallerOutgoingNumberPath, kCalleeNumber));
  EXPECT_CALL(*_mock, changeData(kCalleeIncomingNumberPath, kCallerNumber));
  EXPECT_CALL(*_mock, changeData(kCalleeStatusPath, kStatusBusy));
  EXPECT_CALL(*_mock, changeData(kCallerStatusPath, kStatusBusy));
  EXPECT_TRUE(_client->call(kCalleeNumber));
}

TEST_F(MobileClientTest, TestCallFalse) {
  registerSubscriber();
  EXPECT_CALL(*_mock, fetchData(kCalleeNumber, _)).WillOnce(DoAll(SetArgReferee<1>(kCalleeNumber), Return(false)));
  EXPECT_FALSE(_client->call(KCalleeNumber));
}

TEST_F(MobileClientTest, TestAnswerTrue) {
  registerSubscriber();
  _client->handleModuleChange(kCallerStatusPath, kStatusActive);
  _client->handleModuleChange(kCallerOutgoingNumber, kCalleeNumber);
  EXPECT_CALL(*_mock, changeData(kCallerStatusPath, kStatusActive));
  EXPECT_CALL(*_mock, changeData(kCalleeStatusPath, kStatusActive));
  EXPECT_TRUE(_client->answer());
  _client->handleModuleChange(kCallerStatus, kStatusIdle);
} 

TEST_F(MobileClientTest, TestAnswerFalse) {
  registerSubscriber();
  EXPECT_CALL(*_mock, fetchData(kCalleeIncomingNumberPath, _)).WillOnce(DoAll(SetArgReferee<1>(kCallerNumber), Return(false)));
  EXPECT_FALSE(_client->answer());
}

TEST_F(MobileClientTest, TestRejectTrue) {
  registerSubscriber();
  _client->handleModuleChange(kCallerStatusPath, kStatusActive);
  _client->handleModuleChange(kCallerOutgoingNumberPath, kCalleeNumber);
  EXPECT_CALL(*_mock, changeData(kCallerStatusPath, kStatusIdle));
  EXPECT_CALL(*_mock, changeData(kCalleeStatusPath, kStatusIdle));
  EXPECT_TRUE(_client->reject());
  _client->handleModuleChange(kCallerPath, kStateIdle);
} 

TEST_F(MobileClientTest, TestRejectFalse) {
  registerSubscriber();
  EXPECT_CALL(*_mock, fetchData(kCallerStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(false)));
  EXPECT_FALSE(_client->reject());
}
 
TEST_F(MobileClientTest, TestCallEndFalse) {
  registerSubscriber();
  call();
  EXPECT_CALL(*_mock, fetchData(kCallerStatusPath, _)).WillOnce(DoAll(SetArgReferee<1>(kStatusIdle), Return(false)));
  EXPECT_FALSE(_client->callEnd());
}

}  // namespace Test
