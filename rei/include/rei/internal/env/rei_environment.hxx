#pragma once

namespace rei
{
class Environment final
{
public:
    template<typename StructureType>
    struct StructureRegistrar final
    {
    private:
        VirtualMachine& vm_;
        std::string name_;
    public:
        StructureRegistrar(VirtualMachine& vm, const std::string& name) : vm_(vm), name_(name) {}
    public:
        template<typename FieldType>
        auto field(const std::string& name, FieldType StructureType::* field_ptr) -> StructureRegistrar&;
    public:
        void submit();
    };
public:
    template<typename T>
    auto structure(const std::string& name) -> StructureRegistrar<T> { return {*this, name}; }
};
}
