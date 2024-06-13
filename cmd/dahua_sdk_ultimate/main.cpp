#include <iostream>
#include <csignal>
#include <chrono>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>

#include "Network/Socket.h"
#include "tools/singleton.h"
#include "tools/system.h"
#include "tools/ini_file.h"


#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Util/TimeTicker.h"
#include "Thread/ThreadPool.h"
#include "server/grpc_server.h"

int main(int argc, char **argv) {
    //初始化日志系统
    Logger::Instance().add(std::make_shared<ConsoleChannel>());
    Logger::Instance().add(std::make_shared<FileChannel>());
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

    System *sys = Singleton<System>::instance();
    sys->init(argc, argv);

    IniFile *ini = Singleton<IniFile>::instance();
    const string &ip = (*ini)["server"]["ip"];
    int port = (*ini)["server"]["port"];
    int thread_num = (*ini)["server"]["threads"];


    ThreadPool pool(thread_num, ThreadPool::PRIORITY_HIGHEST, true);
    ServerImpl *server = Singleton<ServerImpl>::instance();
    static semaphore sem;   // 设置退出信号处理函数
    pool.async([&]() {
        setThreadName("grpc thread");
        server->Run(ip, port);
    });


    signal(SIGINT, [](int) {
        sem.post();
    });

    signal(SIGINT, [](int) {
        InfoL << "SIGINT:exit";
        signal(SIGINT, SIG_IGN); // 设置退出信号
        sem.post();
    }); // 设置退出信号

    signal(SIGTERM, [](int) {
        WarnL << "SIGTERM:exit";
        signal(SIGTERM, SIG_IGN);
        sem.post();
    }); // signal 11:SIGSEGV 非法访问内存
    TraceL << "signal set done ..." << std::endl;

    sem.wait();

    // 休眠 100ms 再退出，防止资源释放顺序错误
    InfoL << "程序退出中,请等待...";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    InfoL << "程序退出完毕!";
    exit(0);
    return 0;
}
