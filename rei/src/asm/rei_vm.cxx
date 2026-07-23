#include "rei/internal/asm/rei_vm.hxx"

namespace rei
{
    auto VirtualMachine::compile(const std::string& source) -> Result<Image>
    {
        std::vector<Token> tokens = lexer_.compile(source);
        if (!lexer_.error().empty()) return {lexer_.error()};
        printf("[REI::Lexer]\n");
        for (auto& token : tokens)
            printf("%s\n", string_ReiTokenKind(token.kind));
        return {lexer_.error()};
    }
}
