#pragma once
#include <string>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace rei
{
using id_t = uint32_t;

class Environment final
{
private:
    id_t symbol_count_ = 0;
    std::unordered_map<std::string, id_t> symbol_name_to_id_ {};
    std::vector<std::string> symbol_id_to_name_ {};
public:
    Environment() = default;
    Environment(uint32_t space)
    {
        symbol_id_to_name_.reserve(space);
    }
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
    auto id(const std::string& name) const -> id_t { return symbol_name_to_id_.at(name); }
    auto name(id_t id) const -> const std::string& { return symbol_id_to_name_[id]; }
public:
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
        auto field(id_t id, const std::string& name, int indirection = 0) -> StructureRegistrar&;
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
        auto arg(id_t id, const std::string& name, int indirection = 0);
        auto ret(id_t id, int indirection = 0);
    public:
        auto submit() -> id_t;
    };
public:
    auto constant(id_t id, const std::string& name, void* value);
    auto variable(id_t id, const std::string& name, void* value);
    auto enumeration(const std::string& name) -> EnumerationRegistrar { return {*this, name}; }
    auto structure(const std::string& name) -> StructureRegistrar { return {*this, name}; }
    auto function(const std::string& name) -> FunctionRegistrar { return {*this, name}; }
    void alias(id_t id, const std::string& name) { symbol_name_to_id_[name] = id; }
private:
    // void primitive_(const std::string& name, )
    // {
    // }
};
}
