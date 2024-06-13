#pragma once

#include <string>
#include "Util/logger.h"

using namespace toolkit;
using namespace std;

class System {
public:
    System();

    ~System();

    void init(int argc, char** argv);

    string get_root_path();
    static void startDaemon(bool &kill_parent_if_failed);
private:
    void core_dump();

private:
    string m_root_path;
};

