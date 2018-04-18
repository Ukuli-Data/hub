#ifndef __HUB_COMMANDS_TESTS_RUNNER_H_
#define __HUB_COMMANDS_TESTS_RUNNER_H_

#include <gtest/gtest.h>

#include "hub/commands/create_user.h"
#include "hub/db/db.h"
#include "hub/stats/session.h"
#include "proto/hub.pb.h"

namespace iota {
class CommandTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    auto db = iota::db::DBManager::get();
    db.resetConnection();
    db.loadSchema(true);

    _session = std::make_shared<ClientSession>();
  }

  virtual void TearDown() { _session = nullptr; }

  std::shared_ptr<ClientSession> session() { return _session; }

  grpc::Status createUser(std::shared_ptr<ClientSession> session,
                          std::string username) {
    rpc::CreateUserRequest req;
    rpc::CreateUserReply res;

    req.set_userid(std::move(username));
    cmd::CreateUser command(std::move(session));
    return command.doProcess(&req, &res);
  }

  rpc::Error errorFromStatus(grpc::Status& status) {
    rpc::Error err;
    err.ParseFromString(status.error_details());

    return err;
  }

 private:
  std::shared_ptr<ClientSession> _session;
};

}  // namespace iota
#endif /* __HUB_COMMANDS_TESTS_RUNNER_H_ */
