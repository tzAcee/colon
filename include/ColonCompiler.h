#pragma once
#include <string>
#include <optional>

#include "ColonLexer.h"
#include "ColonParser.h"

class ColonCompiler
{
public:
	ColonCompiler() = default;

	struct CompileResult
	{
		std::vector<TokenMeta> TokenMetas;
		ColonAST AST;
	};
	auto compile(const std::string& source)->std::optional<CompileResult const>;
};

