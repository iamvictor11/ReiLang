#pragma once
#include "rei/internal/rei_result.hxx"
#include "rei/internal/rei_allocation_callbacks.hxx"

namespace rei
{
    struct InitializeCallchain final
    {
    private:
        Result<void> result_ {};
    public:
        InitializeCallchain() = default;
    public:
        auto allocationCallbacks(const ReiAllocationCallbacks& callback) -> InitializeCallchain&;
    public:
        auto call() -> Result<void> { return {}; }
    };
    inline auto initialize() -> InitializeCallchain { return {}; }
}
