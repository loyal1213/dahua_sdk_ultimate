#include "grpc_server.h"
#include <utility>
#include "Util/logger.h"
#include "Thread/ThreadPool.h"
#include "tools/ini_file.h"
#include "tools/singleton.h"

ServerImpl::ServerImpl() {

}

// ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");
ServerImpl::~ServerImpl() {
    server_->Shutdown();
    // 始终在服务器之后关闭完成队列。
    cq_->Shutdown();
}

// 此代码中没有关机处理
void ServerImpl::Run(const std::string &ip, uint16_t port) {
    server_address_ = absl::StrFormat("%s:%d", ip.c_str(), port);
    ServerBuilder builder;
    // 在没有任何身份验证机制的情况下侦听给定的地址
    builder.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
    // 注册“service_”作为我们与客户通信的实例。在本例中，它对应于*异步*服务
    builder.RegisterService(&service_);
    // 获取用于与 gRPC 运行时进行异步通信的完成队列
    cq_ = builder.AddCompletionQueue();
    // 最后组装服务器
    server_ = builder.BuildAndStart();
    InfoL << "AysncTestServer_New is listening on " << server_address_ << std::endl;



/*********
    // 为各个接口创建请求上下文，然后注册请求到服务端
    HandlerHttpContext *http_context = new HandlerHttpContext;
    http_context->type_ = 1;
    http_context->status_ = 1;
    HandlerDownloadContext *download_context = new HandlerDownloadContext;
    download_context->type_ = 2;
    download_context->status_ = 1;
    HandlerUploadContext *upload_context = new HandlerUploadContext;
    upload_context->type_ = 3;
    upload_context->status_ = 1;

    // 注册服务,参数从前到后分别是：rpc服务上下文，rpc请求对象，异步响应器，新的rpc请求使用的完成队列，通知完成使用的完成队列，唯一标识tag标识当前这次请求的上下文
    service_.Requesthttp(&http_context->ctx_, &http_context->req_, &http_context->responder_, cq_.get(), cq_.get(), http_context);
    service_.Requestdownload(&download_context->ctx_, &download_context->req_, &download_context->responder_, cq_.get(), cq_.get(), download_context);
    service_.Requestupload(&upload_context->ctx_, &upload_context->req_, &upload_context->responder_, cq_.get(), cq_.get(), upload_context);
********/


    // IniFile *ini = Singleton<IniFile>::instance();
    // int thread_num = (*ini)["server"]["threads"];
    // toolkit::ThreadPool pool(thread_num, toolkit::ThreadPool::PRIORITY_HIGHEST, true);
    // pool.async([&]() {
    // toolkit::setThreadName("thread_pool_grpc");
    // 进入服务器的主循环
    HandleRpcs();
    // });

}

// 如果需要，可以在多个线程中运行
void ServerImpl::HandleRpcs() {
    // 生成一个新的 CallData 实例来为新客户端提供服务
    // new CallData(&service_, cq_.get());
    new SetAlarmRule(&service_, cq_.get());
    new UploadAlarmInfo(&service_, cq_.get());
    new NotifyMessage(&service_, cq_.get());


    void *tag;  // 唯一标识一个请求
    bool ok;
    while (true) {
        // 阻止等待从完成队列中读取下一个事件,事件由其标记唯一标识，
        // 在本例中为CallData 实例的内存地址,应始终检查 Next 的返回值,这个返回值告诉我们是否有任何类型的事件或 cq_ 正在关闭。
        CHECK(cq_->Next(&tag, &ok));
        CHECK(ok);
        static_cast<CallData *>(tag)->Proceed();
    }
}

ServerImpl::CallData::CallData(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq)
        : service_(service), cq_(cq), status_(CREATE) {
    // 立即调用投放逻辑
    // Proceed();
}


ServerImpl::SetAlarmRule::SetAlarmRule(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq)
        : CallData(service, cq), responder_(&ctx_) {
    Proceed();
}

void ServerImpl::SetAlarmRule::Proceed() {
    ImgUpload_proceed();
}


void ServerImpl::SetAlarmRule::ImgUpload_proceed() {
    if (status_ == CREATE) {
        // 使此实例进入 PROCESS 状态
        status_ = PROCESS;

        // 作为初始 CREATE 状态的一部分，我们*请求*系统开始处理 SayHello 请求。
        // 在此请求中，“this” 行为是唯一标识请求的标记（以便不同的 CallData实例可以同时处理不同的请求），
        // 在这种情况下此 CallData 实例的内存地址
        service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_, this);

    } else if (status_ == PROCESS) {
        // 生成一个新的 CallData 实例，以便在我们处理时为新客户提供服务
        // 用于此 CallData 的那个实例将自身解除分配为其FINISH状态的一部分。
        new SetAlarmRule(service_, cq_);

        // 实际处理
        std::string prefix("Hello ");
        reply_.set_message(prefix + request_.name());

        // 大功告成！让 gRPC 运行时知道我们已经完成了，
        // 使用此实例的内存地址作为事件的唯一标识标签。
        status_ = FINISH;

        responder_.Finish(reply_, Status::OK, this);
    } else {
        CHECK_EQ(status_, FINISH);
        // 进入 FINISH 状态后，解除分配我们自己 （CallData）。
        delete this;
    }
}

ServerImpl::UploadAlarmInfo::UploadAlarmInfo(DahuaManageService::AsyncService *service, ServerCompletionQueue *cq)
        : CallData(service, cq), stream_(&ctx_) {
    Proceed();
}

void ServerImpl::UploadAlarmInfo::resImgFetched_proceed() {
    if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestSayHelloBidiStream(&ctx_, &stream_, cq_, cq_, this);
    } else if (status_ == PROCESS) {
        new UploadAlarmInfo(service_, cq_);

        stream_.Read(&request_, this);

        std::string prefix("double stream, ");
        reply_.set_message(prefix + request_.name());
        stream_.Write(reply_, this);

        status_ = FINISH;
        stream_.Finish(Status::OK, this); // 终止写入流(表示客户端本次请求的资源已经全部发出)
    } else {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
}


void ServerImpl::NotifyMessage::Proceed() {
    descFetched_proceed();
}

void ServerImpl::NotifyMessage::descFetched_proceed() {
    if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestSayHelloStreamReply(&ctx_, &request_, &writer_, cq_, cq_, this);
    } else if (status_ == PROCESS) {
        // 创建一个新的对象，用以处理下一个客户端(连接)的调用事件
        new NotifyMessage(service_, cq_);
        status_ = FINISH;

        std::string prefix("Hello ");
        reply_.set_message(prefix + request_.name());
        writer_.Write(reply_, this);


        writer_.Finish(Status::OK, this);
    } else {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
}
