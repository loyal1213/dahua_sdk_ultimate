/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "dahua_sdk_ultimate.grpc.pb.h"

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using dahua_sdk_ultimate::DahuaManageService;
using dahua_sdk_ultimate::HelloReply;
using dahua_sdk_ultimate::HelloRequest;

class GreeterClient {
 public:
  explicit GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(DahuaManageService::NewStub(channel)) {}

  // 组装客户端的有效负载，发送它并返回响应从服务器。
  std::string SayHello(const std::string& user) {
    // 我们发送到服务器的数据
    HelloRequest request;
    request.set_name(user);

    // 我们期望从服务器获得的数据的容器
    HelloReply reply;

    // 客户端的上下文。它可以用来将额外的信息传达给服务器和/或调整某些 RPC 行为
    ClientContext context;

    // 我们用来与 gRPC 运行时异步通信的生产者-使用者队列
    CompletionQueue cq;

    // RPC 完成时状态的存储
    Status status;

    std::unique_ptr<ClientAsyncResponseReader<HelloReply> > rpc(
        stub_->AsyncSayHello(&context, request, &cq));

    // 请求在完成 RPC 后，将“回复”更新为服务器的响应; “status”表示操作是否成功了。使用整数 1 标记请求。
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    // 阻塞，直到下一个结果在完成队列“cq”中可用。应始终检查 Next 的返回值,
    // 此返回值告诉我们是否有任何类型的事件或cq_正在关闭
    CHECK(cq.Next(&got_tag, &ok));

    // 验证来自“cq”的结果是否通过其标签与我们之前的请求相对应
    CHECK_EQ(got_tag, (void*)1);
    // ...并且请求已成功完成。请注意，“确定” 仅对应于 Finish（） 引入的更新请求
    CHECK(ok);

    // 根据实际 RPC 的状态执行操作
    if (status.ok()) {
      return reply.message();
    } else {
      return "RPC failed, err: " + std::string (status.error_message());
    }
  }

 private:
  // 从传入的通道中传出存根，存储在这里，我们对服务器公开服务的视图。
  std::unique_ptr<DahuaManageService::Stub> stub_;
};

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  // 实例化客户端。它需要一个通道，实际的 RPC 从中被创建。 此通道对与参数 “--target=” 是唯一预期的参数。
  std::string target_str = absl::GetFlag(FLAGS_target);
  // 我们指示通道未经过身份验证（使用 InsecureChannelCredentials（））。
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);  // The actual RPC call!
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
