#include "stdafx.h"
#include "GpuMemory.h"

///////////////////////////////////////////////////////////////////////////////////////////
// CPdhGPUMemory 实现
///////////////////////////////////////////////////////////////////////////////////////////
CPdhGPUMemory::CPdhGPUMemory(bool dedicated)
    : CPdhQuery(dedicated ? _T("\\GPU Adapter Memory(*)\\Dedicated Usage") : _T("\\GPU Adapter Memory(*)\\Shared Usage"))
{
}

CPdhGPUMemory::~CPdhGPUMemory()
{
}

bool CPdhGPUMemory::GetGpuMemory(long long& memory_used)
{
    if (isInitialized)
    {
        std::vector<CounterValueItem> valueItems;
        if (QueryValues(valueItems))
        {
            if (!valueItems.empty())
            {
                //计数器的每个实例的名称形如“luid_0x00000000_0x0000E4C6_phys_0”，
                //同一个显卡可能存在多个内存段（phys_N），因此按显卡（luid）分组累加
                std::map<std::wstring, double> gpu_memory_map;
                for (const auto& item : valueItems)
                {
                    std::wstring item_name = item.name;
                    size_t index = item.name.rfind(L"_phys");
                    if (index != std::wstring::npos)
                        item_name = item.name.substr(0, index);
                    gpu_memory_map[item_name] += item.value;
                }
                //存在多个显卡时取使用量最大的显卡（同显卡利用率的处理）
                double max_value = 0;
                for (const auto& item : gpu_memory_map)
                {
                    if (item.second > max_value)
                        max_value = item.second;
                }

                memory_used = static_cast<long long>(max_value);
                return true;
            }
        }
    }

    return false;
}
