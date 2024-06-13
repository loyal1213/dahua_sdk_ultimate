//
// Created by hy on 2024/6/13.
//

#ifndef DAHUA_SDK_ULTIMATE_RULE_CONFIG_H
#define DAHUA_SDK_ULTIMATE_RULE_CONFIG_H


class rule_config {
public:
    rule_config();
    ~rule_config();
    bool  SetAlarmRule();
    void* GetAlarmRule();
};


#endif //DAHUA_SDK_ULTIMATE_RULE_CONFIG_H
