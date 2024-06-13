//
// Created by hy on 2024/6/12.
//

#ifndef DAHUA_SDK_ULTIMATE_GRPCSERVER_H
#define DAHUA_SDK_ULTIMATE_GRPCSERVER_H

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "dahua_sdk_ultimate.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerAsyncWriter;
using grpc::ServerAsyncReaderWriter;

using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using dahua_sdk_ultimate::DahuaManageService;
using dahua_sdk_ultimate::HelloReply;
using dahua_sdk_ultimate::HelloRequest;

/**********************************************
 C++异步服务端实例，详细注释版#
 gRPC使用C++实现异步服务端的基本逻辑：
  构建数据结构来存储需要处理的请求及其上下文信息，此处使用HandlerContext，相当于对到达的请求的封装
  首先注册各个接口的HandlerContext，放入完成队列CompletionQueue中，当请求到达时，根据类型封装进对应的HandlerContext，
  由于是异步客户端，需要保证后面到达的请求也有HandlerContext用，所以用一个就要再创建一个空的放回去
  运行完的接口，其HandlerContext需要销毁
  以下代码的关键为run()方法中的逻辑以及HandlerContext的设置，每一步都有注释，可以详细观看
 **********************************************/
#include "Thread/ThreadPool.h"

class ServerImpl final {
public:
    //构造时传入IP:Port即可
    ServerImpl();

    ~ServerImpl();

    void Run(const std::string &ip, uint16_t port);

private:
    // 包含为请求提供服务所需的状态和逻辑的类
    class CallData {
    public:
        // 以“服务”实例为例（在本例中表示异步 server） 和用于异步通信的完成队列“CQ” 使用 gRPC 运行时
        CallData(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq);

        virtual void Proceed() = 0;

    protected:
        // 与异步服务器的 gRPC 运行时进行通信的方式.
        DahuaManageService::AsyncService *service_;
        // 用于异步服务器通知的生产者-使用者队列.
        ServerCompletionQueue *cq_;
        // rpc 的上下文，允许调整它的各个方面，例如使用压缩、身份验证，以及将元数据发送回客户
        ServerContext ctx_;

        // 让我们实现一个具有以下状态的微型状态机
        enum CallStatus {
            CREATE, PROCESS, FINISH
        };
        CallStatus status_;  // The current serving state.
    };

    // 一元消息
    class SetAlarmRule : public CallData {
    public:
        SetAlarmRule(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq);

        void Proceed() override;

    private:
        void ImgUpload_proceed();

    private:
        // 我们从客户那里得到什么
        HelloRequest request_;
        // 我们发回给客户的内容
        HelloReply reply_;
        // 返回客户的方法
        ServerAsyncResponseWriter<HelloReply> responder_;
    };

    // 双向流
    class UploadAlarmInfo : public CallData {
    public:
        UploadAlarmInfo(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq);

        void Proceed() override {
            resImgFetched_proceed();
        }

    private:
        void resImgFetched_proceed();
    private:
        HelloRequest request_;
        HelloReply reply_;
        ServerAsyncReaderWriter<HelloReply, HelloRequest> stream_;
    };

    // 服务端流
    class NotifyMessage : public CallData {
    public:
        NotifyMessage(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq)
                : CallData(service, cq), writer_(&ctx_) {
            Proceed();
        }

        void Proceed();
    private:
        void descFetched_proceed();
    private:
        HelloRequest request_;
        // 我们发回给客户的内容
        HelloReply reply_;
        // 返回客户的方法
        ServerAsyncWriter<HelloReply> writer_;
    };

    // 如果需要，可以在多个线程中运行
    void HandleRpcs();

private:
    // 当前服务器的地址
    std::string server_address_;
    // 当前服务器的完成队列
    std::unique_ptr<ServerCompletionQueue> cq_;
    // 当前服务器的异步服务
    DahuaManageService::AsyncService service_;
    // 服务器实例
    std::unique_ptr<Server> server_;
};




#endif //DAHUA_SDK_ULTIMATE_GRPCSERVER_H
