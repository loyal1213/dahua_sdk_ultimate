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
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "dahua_sdk_ultimate.grpc.pb.h"

ABSL_FLAG(std::string, target, "0.0.0.0:50051", "Server address");

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

  // 组装客户端的有效负载并将其发送到服务器
  void SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // 调用对象来存储 rpc 数据
    AsyncClientCall* call = new AsyncClientCall;

    // stub_->PrepareAsyncSayHello（）
    // 创建一个 RPC 对象，返回存储在“call”中的实例，但实际上不启动 RPC
    // 因为我们使用的是异步 API，所以我们需要坚持“调用”实例，以获取正在进行的 RPC 的更新。
    call->response_reader =
        stub_->PrepareAsyncSayHello(&call->context, request, &cq_);

    // StartCall 启动 RPC 调用
    call->response_reader->StartCall();

    // 请求在完成 RPC 后，将“回复”更新为服务器的响应;“status”表示操作是否
    // 成功了。使用调用的内存地址标记请求对象。
    call->response_reader->Finish(&call->reply, &call->status, (void*)call);
  }

  // 在侦听已完成的响应时循环。打印出来自服务器的响应。
  void AsyncCompleteRpc() {
    void* got_tag;
    bool ok = false;

    // 阻塞，直到下一个结果在完成队列“cq”中可用
    while (cq_.Next(&got_tag, &ok)) {
      // 此示例中的标记是调用对象的内存位置
      AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);

      // 验证请求是否已成功完成。请注意，“确定” 仅对应于 Finish（） 引入的更新请求。
      CHECK(ok);

      if (call->status.ok())
        std::cout << "Greeter received: " << call->reply.message() << std::endl;
      else
        std::cout << "RPC failed" << std::endl;

      // 完成后，解除分配调用对象
      delete call;
    }
  }

 private:
  // 用于保存状态和数据信息的结构
  struct AsyncClientCall {
    // 我们期望从服务器获得的数据的容器
    HelloReply reply;

    // 客户端的上下文。它可以用来将额外的信息传达给服务器和/或调整某些 RPC 行为。
    ClientContext context;

    // RPC 完成时状态的存储
    Status status;

    std::unique_ptr<ClientAsyncResponseReader<HelloReply>> response_reader;
  };

  // 从传入的通道中传出存根，存储在这里，我们查看服务器公开的服务
  std::unique_ptr<DahuaManageService::Stub> stub_;

  // 我们用来与 gRPC 运行时
  CompletionQueue cq_;
};

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  // 实例化客户端。它需要一个通道，实际的 RPC 从中
  // 被创建。此通道对与
  // 参数 “--target=” 是唯一预期的参数。
  std::string target_str = absl::GetFlag(FLAGS_target);
  // 我们指示该频道未经过身份验证（使用 InsecureChannelCredentials（））
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  // 生成无限循环的读取器线程
  std::thread thread_ = std::thread(&GreeterClient::AsyncCompleteRpc, &greeter);

  for (int i = 0; i < 100; i++) {
    std::string user("world " + std::to_string(i));
    greeter.SayHello(user);  // The actual RPC call!
  }

  std::cout << "Press control-c to quit" << std::endl << std::endl;
  thread_.join();  // blocks forever

  return 0;
}
