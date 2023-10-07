#pragma once
#include <vector>
#include <TokenIterator.h>
#include <ColonAST.h>
#include <ColonParserCompliance.h>
#include <functional>

class ParseFallThrough
{
public:
	enum class ParseState
	{
		SUCCESS,
		FAIL
	};

	template <typename ParseFunction>
	static auto parse_with_fall_through(ParseFunction parse_function) -> ParseState
	{
		try {
			parse_function();
			return ParseState::SUCCESS;
		}
		catch (const std::runtime_error& )
		{
			return ParseState::FAIL;
		}
	}
};
class ColonParser
{
public:
	ColonParser(std::vector<TokenMeta> const& tokens) : m_TokenIterator{ TokenIterator{tokens} }
	{
	}

	auto parse() ->std::optional<ColonAST const>;

private: 
	auto parse_entry() -> ASTNode;
	auto parse_functionUnparameterised()->ASTNode;
	auto parse_def()->ASTNode;
	auto parse_expression()->ASTNode;
	auto parse_functionCall()->ASTNode;
	auto parse_variableList()->ASTNode;
	auto parse_assignment()->ASTNode;
	auto parse_variableValue()->ASTNode;
	auto parse_string()->ASTNode;
	auto parse_letter()->ASTNode;
	auto parse_word()->ASTNode;
	auto parse_number()->ASTNode;
	auto parse_digit()->ASTNode;

private:
	TokenIterator m_TokenIterator;
};

