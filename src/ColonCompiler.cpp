#include <ColonCompiler.h>

auto ColonCompiler::compile(const std::string& source) -> CompileResult const
{
    ColonLexer<char32_t> lexer{ source };
    auto const tokens = lexer.lex();

    ColonParser parser{ tokens };
    auto parse_result = parser.parse();

    return CompileResult{ tokens, parse_result };
}
