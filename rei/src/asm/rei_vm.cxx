#include "rei/internal/asm/rei_vm.hxx"

namespace rei
{
    auto VirtualMachine::compile(const std::string& source) -> Result<Image>
    {
        std::vector<Token> tokens = lexer_.compile(source);
        if (!lexer_.error().empty()) return {lexer_.error()};
        for (auto& token : tokens)
            printf(string_ReiTokenKind(token.kind));
    }
}
