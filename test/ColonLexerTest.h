#pragma once

#include "doctest.h"

#include <ColonLexer.h>

#include <cstdint>

TEST_CASE("ColonLexer char8_t ':ba2'-test")
{
	auto const test_source = ":ba2";

	std::vector const expected_tokens = {
		PreDefToken::OperatorColon,
		PreDefToken::Letter,
		PreDefToken::Letter,
		PreDefToken::Number
	};

	ColonLexer<char8_t> const lexer{ test_source };
	auto res = lexer.lex();

	CHECK(res == expected_tokens);
}

TEST_CASE("ColonLexer char8_t 'Ya[{}]()'-test")
{
	auto const test_source = "Ya[{}]()";

	std::vector const expected_tokens = {
		PreDefToken::Letter,
		PreDefToken::Letter,
		PreDefToken::OperatorSquareBracketsOpen,
		PreDefToken::OperatorCurlyBracesOpen,
		PreDefToken::OperatorCurlyBracesClose,
		PreDefToken::OperatorSquareBracketsClose,
		PreDefToken::OperatorParenthesesOpen,
		PreDefToken::OperatorParenthesesClose,
	};

	ColonLexer<char8_t> const lexer{ test_source };
	auto res = lexer.lex();

	CHECK(res == expected_tokens);
}

TEST_CASE("ColonLexer char8_t '*+-/'-test")
{
	auto const test_source = "*+-/";

	std::vector const expected_tokens = {
		PreDefToken::OperatorMult,
		PreDefToken::OperatorAdd,
		PreDefToken::OperatorSub,
		PreDefToken::OperatorDiv,
	};

	ColonLexer<char8_t> const lexer{ test_source };
	auto res = lexer.lex();

	CHECK(res == expected_tokens);
}

TEST_CASE("ColonLexer char8_t '=&!|><\"'-test")
{
	auto const test_source = "=&!|><\"";

	std::vector const expected_tokens = {
		PreDefToken::OperatorAssign,
		PreDefToken::OperatorLogicalAnd,
		PreDefToken::OperatorLogicalNot,
		PreDefToken::OperatorLogicalOr,
		PreDefToken::OperatorGreater,
		PreDefToken::OperatorLess,
		PreDefToken::OperatorQuote
	};

	ColonLexer<char8_t> const lexer{ test_source };
	auto res = lexer.lex();

	CHECK(res == expected_tokens);
}

TEST_CASE("ColonLexer char8_t ':a(){:u=1963;}'-test")
{
	auto const test_source = ":a(){:u=1963;}";

	std::vector const expected_tokens = {
		PreDefToken::OperatorColon,
		PreDefToken::Letter,
		PreDefToken::OperatorParenthesesOpen,
		PreDefToken::OperatorParenthesesClose,
		PreDefToken::OperatorCurlyBracesOpen,
		PreDefToken::OperatorColon,
		PreDefToken::Letter,
		PreDefToken::OperatorAssign,
		PreDefToken::Number,
		PreDefToken::Number,
		PreDefToken::Number,
		PreDefToken::Number,
		PreDefToken::OperatorSemicolon,
		PreDefToken::OperatorCurlyBracesClose
	};

	ColonLexer<char8_t> const lexer{ test_source };
	auto res = lexer.lex();

	CHECK(res == expected_tokens);
}