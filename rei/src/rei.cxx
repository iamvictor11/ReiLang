#include "rei/rei.hxx"

namespace rei
{
    auto InitializeCallchain::allocationCallbacks(const ReiAllocationCallbacks& callback) -> InitializeCallchain&
    {
        reiAllocationCallbacks_g = callback;
    }
}
