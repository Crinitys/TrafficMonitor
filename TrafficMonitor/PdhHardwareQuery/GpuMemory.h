#pragma once
#include "PdhQuery.h"

// 显存使用量监控类（专用显存/共享显存）
class CPdhGPUMemory : public CPdhQuery
{
public:
    // dedicated 为true时监控专用显存，为false时监控共享显存
    CPdhGPUMemory(bool dedicated);
    ~CPdhGPUMemory();

    // 获取显存使用量（单位为字节）
    // 返回值: true=成功, false=失败
    bool GetGpuMemory(/*out*/ long long& memory_used);
};
