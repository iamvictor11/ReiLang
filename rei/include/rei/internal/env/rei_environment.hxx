#pragma once
#include <string>
#include <cstdint>
#include <type_traits>
#include <unordered_map>

namespace rei
{
using id_t = uint32_t;
// enum 

class Environment final
{
private:
    id_t symbol_count_ = 0;
    std::unordered_map<std::string, id_t> symbol_name_to_id_;
    std::unordered_map<id_t, std::string> symbol_id_to_name_;
public:
    Environment() = default;
    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;
    Environment(Environment&& other) noexcept :
        symbol_count_(other.symbol_count_),
        symbol_name_to_id_(std::move(other.symbol_name_to_id_)),
        symbol_id_to_name_(std::move(other.symbol_id_to_name_)) {}
    Environment& operator=(Environment&& other) noexcept
    {
        destructor_();
        symbol_count_       = other.symbol_count_;
        symbol_name_to_id_  = std::move(other.symbol_name_to_id_);
        symbol_id_to_name_  = std::move(other.symbol_id_to_name_);
        return *this;
    }
    ~Environment() { destructor_(); }
private:
    void destructor_() {};
public:
    auto typeId(const std::string& name) const -> id_t { return symbol_name_to_id_.at(name); }
public:
    // struct ConstantRegistrar final
    // {
    // private:
    //     Environment& env_;
    //     std::string name_;
    // public:
    //     ConstantRegistrar(Environment& env, const std::string& name) : env_(env), name_(name) {}
    // public:
    //     auto type(id_t id) -> ConstantRegistrar&;
    //     auto value(void* v) -> ConstantRegistrar&;
    // public:
    //     void submit();
    // };
    // struct VariableRegistrar final
    // {
    // private:
    //     Environment& env_;
    //     std::string name_;
    // public:
    //     VariableRegistrar(Environment& env, const std::string& name) : env_(env), name_(name) {}
    // public:
    //     auto type(id_t id) -> VariableRegistrar&;
    //     auto value(void* v) -> VariableRegistrar&;
    // public:
    //     void submit();
    // };
    struct EnumerationRegistrar final
    {
    private:
        Environment& env_;
        std::string name_;
    private:
        uint64_t count_ = 0;
    public:
        EnumerationRegistrar(Environment& env, const std::string& name) : env_(env), name_(name) {}
    public:
        auto integer(id_t id) -> EnumerationRegistrar&;
        auto item(const std::string& name) -> EnumerationRegistrar& { return item_(name, count_); }
        template<typename T>
        auto item(const std::string& name, T value) -> EnumerationRegistrar&
        { static_assert(std::is_integral_v<T>, "enumeration must is integral type"); return item_(name, static_cast<uint64_t>(value)); }
        auto alias(const std::string& name) -> EnumerationRegistrar&;
    public:
        auto submit() -> id_t;
    private:
        auto item_(const std::string& name, uint64_t value) -> EnumerationRegistrar&;
    };
    struct StructureRegistrar final
    {
    private:
        Environment& env_;
        std::string name_;
    public:
        StructureRegistrar(Environment& env, const std::string& name) : env_(env), name_(name) {}
    public:
        auto align(int size) -> StructureRegistrar&;
        auto field(id_t id, const std::string& name, bool ptr = false) -> StructureRegistrar&;
        auto alias(const std::string& name) -> StructureRegistrar&;
    public:
        auto submit() -> id_t;
    };
    struct FunctionRegistrar final
    {
    private:
        Environment& env_;
        std::string name_;
    public:
        FunctionRegistrar(Environment& env, const std::string& name) : env_(env), name_(name) {}
    public:
        auto arg(id_t id, const std::string& name, bool ptr);
        auto ret(id_t id);
    public:
        auto submit() -> id_t;
    };
public:
    // auto constant(const std::string& name) -> ConstantRegistrar { return {*this, name}; }
    // auto variable(const std::string& name) -> VariableRegistrar { return {*this, name}; }
    auto enumeration(const std::string& name) -> EnumerationRegistrar { return {*this, name}; }
    auto structure(const std::string& name) -> StructureRegistrar { return {*this, name}; }
    auto function(const std::string& name) -> FunctionRegistrar { return {*this, name}; }
private:
    // void primitive_(const std::string& name, )
    // {
    // }
};
}
