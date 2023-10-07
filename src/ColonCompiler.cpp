#include <ColonCompiler.h>

auto ColonCompiler::compile(const std::string& source) -> std::optional<CompileResult const>
{
    ColonLexer<char32_t> lexer{ source };
    auto const tokens = lexer.lex();

    ColonParser parser{ tokens };
    auto parse_result = parser.parse();
    if (parse_result.has_value())
    {
        return CompileResult{ tokens, parse_result.value() };
    }
    return std::nullopt;
}
