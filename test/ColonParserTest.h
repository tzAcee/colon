#pragma once

#include "doctest.h"

#include <ColonParser.h>

TEST_CASE("ColonParser test-success")
{
	std::vector const tokens = {
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

	ColonParser parser{ tokens };

	auto result = parser.parse();

	CHECK(result.Head().Identifier() == "enatry");
	CHECK(result.Head().Children().size() == 1);

	CHECK(result.Head().Children().at(0).Identifier() == "function unparameterised");
	CHECK(result.Head().Children().at(0).Children().size() == 6);

	auto unparam = result.Head().Children().at(0).Children();
	CHECK(unparam[0].Identifier() == "def");
	CHECK(unparam[0].Children().size() == 2);
	auto def = unparam[0].Children();
	CHECK(def[0].Identifier() == ":");
	CHECK(def[0].Children().size() == 0);
	CHECK(def[1].Identifier() == "word");
	CHECK(def[1].Children()[0].Identifier() == "letter");
	CHECK(def[1].Children()[0].Children()[0].Identifier() == "a letter");

	CHECK(unparam[1].Identifier() == "(");
	CHECK(unparam[1].Children().empty());
	CHECK(unparam[2].Identifier() == ")");
	CHECK(unparam[2].Children().empty());
	CHECK(unparam[3].Identifier() == "{");
	CHECK(unparam[3].Children().empty());

	auto expression = unparam[4].Children();
	CHECK(unparam[4].Identifier() == "expression");
	CHECK(expression.size() == 1);
	CHECK(expression[0].Identifier() == "assignment");
	CHECK(expression[0].Children().size() == 4);
	auto assign = expression[0].Children();
	CHECK(assign[0].Identifier() == "def");
	auto assign_def = assign[0].Children();
	CHECK(assign_def[0].Identifier() == ":");
	CHECK(assign_def[0].Children().size() == 0);
	CHECK(assign_def[1].Identifier() == "word");
	CHECK(assign_def[1].Children()[0].Identifier() == "letter");
	CHECK(assign_def[1].Children()[0].Children()[0].Identifier() == "a letter");
	CHECK(assign[1].Identifier() == "=");
	CHECK(assign[1].Children().empty());
	CHECK(assign[2].Identifier() == "variable value");
	auto var_val = assign[2].Children();
	CHECK(var_val.size() == 1);
	CHECK(var_val[0].Identifier() == "number");
	CHECK(var_val[0].Children()[0].Identifier() == "digit");
	CHECK(var_val[0].Children()[0].Children().size() == 1);
	CHECK(var_val[0].Children()[0].Children()[0].Identifier() == "digit");
	CHECK(assign[3].Identifier() == ";");
	CHECK(assign[3].Children().empty());

	CHECK(unparam[5].Identifier() == "}");
	CHECK(unparam[5].Children().empty());
}