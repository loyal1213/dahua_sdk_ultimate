#include "system.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <cstring>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>
#include "singleton.h"
#include "ini_file.h"
#include "common/dhnetsdk.h"
#include "server/grpc_server.h"

#include "Util/util.h"
#include "Util/logger.h"
#include "Util/uv_errno.h"
#include "device/device_register.h"
System::System() {
}

System::~System() {
    //结束搜索
    // StopSearchDevice();

    CLIENT_Cleanup();
}

void System::init(int argc, char **argv) {
    core_dump();

    m_root_path = get_root_path();

    const string &logdir = m_root_path + "/../log";
    DIR *dp = opendir(logdir.c_str());
    if (dp == NULL) {
        mkdir(logdir.c_str(), 0755);
    } else {
        closedir(dp);
    }

    // init logger
    // Logger::instance()->open(m_root_path + "/log/main.log");

    // init inifile
    IniFile *ini = Singleton<IniFile>::instance();
    ini->load(get_root_path() + "/../configs/config.ini");
//
//    // init workflow
//    Workflow *workflow = Singleton<Workflow>::instance();
//    workflow->load(get_root_path() + "/config/workflow.xml");

    // absl::ParseCommandLine(argc, argv);
    // ServerImpl server;
    // server.Run(absl::GetFlag(FLAGS_port));

    device_register *device = Singleton<device_register>::instance();
    device->Init();
    device->Login();
}

void System::core_dump() {
    // core dump
    struct rlimit x;
    int ret = getrlimit(RLIMIT_CORE, &x);
    x.rlim_cur = x.rlim_max;
    ret = setrlimit(RLIMIT_CORE, &x);

    ret = getrlimit(RLIMIT_DATA, &x);
    x.rlim_cur = 768000000;
    ret = setrlimit(RLIMIT_DATA, &x);
}


string System::get_root_path() {
    if (m_root_path != "") {
        return m_root_path;
    }
    char path[1024]{};
    memset(path, 0, 1024);
    int cnt = readlink("/proc/self/exe", path, 1024);
    if (cnt < 0 || cnt >= 1024) {
        return "";
    }
    for (int i = cnt; i >= 0; --i) {
        if (path[i] == '/') {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}


void System::startDaemon(bool &kill_parent_if_failed) {
    kill_parent_if_failed = true;
#ifndef _WIN32
    static pid_t pid;
    do {
        pid = fork();
        if (pid == -1) {
            WarnL << "fork失败:" << get_uv_errmsg();
            //休眠1秒再试
            sleep(1);
            continue;
        }

        if (pid == 0) {
            //子进程
            return;
        }

        //父进程,监视子进程是否退出
        DebugL << "启动子进程:" << pid;
        signal(SIGINT, [](int) {
            WarnL << "收到主动退出信号,关闭父进程与子进程";
            kill(pid, SIGINT);
            exit(0);
        });

        signal(SIGTERM, [](int) {
            WarnL << "收到主动退出信号,关闭父进程与子进程";
            kill(pid, SIGINT);
            exit(0);
        });

        do {
            int status = 0;
            if (waitpid(pid, &status, 0) >= 0) {
                WarnL << "子进程退出";
                //休眠3秒再启动子进程
                sleep(3);
                //重启子进程，如果子进程重启失败，那么不应该杀掉守护进程，这样守护进程可以一直尝试重启子进程
                kill_parent_if_failed = false;
                break;
            }
            DebugL << "waitpid被中断:" << get_uv_errmsg();
        } while (true);
    } while (true);
#endif // _WIN32
}
