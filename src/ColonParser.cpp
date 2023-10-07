#include <ColonParser.h>
#include <iostream>

auto ColonParser::parse() -> ColonAST const
{
	try {
		ColonAST return_ast{ parse_entry() };
		return return_ast;
	}
	catch (std::exception const& e)
	{
		std::cerr << "parse error:\n";
		std::cerr << e.what();
		throw e;
	}
}

auto ColonParser::parse_entry() -> ASTNode
{
	ASTNode entry_node("entry");
	entry_node.Children().emplace_back(parse_functionUnparameterised());
	return entry_node;
}

auto ColonParser::parse_functionUnparameterised() -> ASTNode
{
	ASTNode function_unparam_node("function unparameterised");

	function_unparam_node.Children().emplace_back(parse_def());

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorParenthesesOpen)
	{
		throw std::runtime_error("Expected '(' in function unparameterised");
	}
	function_unparam_node.Children().emplace_back(ASTNode("("));

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorParenthesesClose)
	{
		throw std::runtime_error("Expected ')' in function unparameterised");
	}
	function_unparam_node.Children().emplace_back(ASTNode(")"));

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorCurlyBracesOpen)
	{
		throw std::runtime_error("Expected '{' in function unparameterised");
	}
	function_unparam_node.Children().emplace_back(ASTNode("{"));

	function_unparam_node.Children().emplace_back(parse_expression());

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorCurlyBracesClose)
	{
		throw std::runtime_error("Expected '}' in function unparameterised");
	}
	function_unparam_node.Children().emplace_back(ASTNode("}"));

	return function_unparam_node;
}

auto ColonParser::parse_def() -> ASTNode
{
	ASTNode def_node("def");

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorColon)
	{
		throw std::runtime_error("Expected ':' in def");
	}
	def_node.Children().emplace_back(ASTNode(":"));

	def_node.Children().emplace_back(parse_word());

	return def_node;
}

auto ColonParser::parse_expression() -> ASTNode
{
	//ParseFallThrough::parse_with_fall_through([this]() { parse_digit(); });
	ASTNode def_node("expression");

	{
		// Parse assignment | assignment expression
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &def_node]()
			{
				auto assign_node = parse_assignment();
				def_node.Children().emplace_back(std::move(assign_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();

			m_TokenIterator.Remember();
			result = ParseFallThrough::parse_with_fall_through([this, &def_node]()
				{
					auto expression_node = parse_expression();
					def_node.Children().emplace_back(std::move(expression_node));
				});
			if (result == ParseFallThrough::ParseState::SUCCESS)
				m_TokenIterator.Forget();
			else
				m_TokenIterator.Reset();

			return def_node;
		}
		m_TokenIterator.Reset();
	}

	// Parse functionCall | functionCall expression
	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &def_node]()
			{
				auto fc_call_node = parse_functionCall();
				def_node.Children().emplace_back(std::move(fc_call_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();

			m_TokenIterator.Remember();
			result = ParseFallThrough::parse_with_fall_through([this, &def_node]()
				{
					auto expression_node = parse_expression();
					def_node.Children().emplace_back(std::move(expression_node));
				});
			if (result == ParseFallThrough::ParseState::SUCCESS)
				m_TokenIterator.Forget();
			else
				m_TokenIterator.Reset();

			return def_node;
		}
		m_TokenIterator.Reset();
	}

	throw std::runtime_error("Could not parse expression.");
}

auto ColonParser::parse_functionCall() -> ASTNode
{
	ASTNode function_call_node("function call");
	function_call_node.Children().emplace_back(parse_word());
	if (m_TokenIterator.Next().value() != PreDefToken::OperatorParenthesesOpen)
	{
		throw std::runtime_error("Expected '(' in function call");
	}
	function_call_node.Children().emplace_back(ASTNode("("));

	m_TokenIterator.Remember();
	auto result = ParseFallThrough::parse_with_fall_through([this, &function_call_node]()
		{
			auto expression_node = parse_variableList();
			function_call_node.Children().emplace_back(std::move(expression_node));
		});
	if (result == ParseFallThrough::ParseState::SUCCESS)
		m_TokenIterator.Forget();
	else
		m_TokenIterator.Reset();

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorParenthesesOpen)
	{
		throw std::runtime_error("Expected ')' in function call");
	}
	function_call_node.Children().emplace_back(ASTNode(")"));

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorSemicolon)
	{
		throw std::runtime_error("Expected ';' in function call");
	}
	function_call_node.Children().emplace_back(ASTNode(";"));

	return function_call_node;
}

auto ColonParser::parse_variableList() -> ASTNode
{
	ASTNode variable_list_node("variable list");

	variable_list_node.Children().emplace_back(parse_word());

	// TODO check comma

	return variable_list_node;
}

auto ColonParser::parse_assignment() -> ASTNode
{
	ASTNode assign_node("assignment");

	assign_node.Children().emplace_back(parse_def());

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorAssign)
	{
		throw std::runtime_error("Expected '=' in assignment");
	}
	assign_node.Children().emplace_back(ASTNode("="));

	assign_node.Children().emplace_back(parse_variableValue());

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorSemicolon)
	{
		throw std::runtime_error("Expected ';' in assignment");
	}
	assign_node.Children().emplace_back(ASTNode(";"));

	return assign_node;
}

auto ColonParser::parse_variableValue() -> ASTNode
{
	ASTNode assign_node("variable value");
	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &assign_node]()
			{
				auto expression_node = parse_string();
				assign_node.Children().emplace_back(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
			return assign_node;
		}
		else
			m_TokenIterator.Reset();
	}

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &assign_node]()
			{
				auto expression_node = parse_number();
				assign_node.Children().emplace_back(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
			return assign_node;
		}
		else
			m_TokenIterator.Reset();
	}

	throw std::runtime_error("could not parse variable value");
}

auto ColonParser::parse_string() -> ASTNode
{
	ASTNode string_node("string");

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorQuote)
	{
		throw std::runtime_error("Expected '\"' in string");
	}
	string_node.Children().emplace_back(ASTNode("\""));

	string_node.Children().emplace_back(parse_word());

	if (m_TokenIterator.Next().value() != PreDefToken::OperatorQuote)
	{
		throw std::runtime_error("Expected '\"' in string");
	}
	string_node.Children().emplace_back(ASTNode("\""));

	return string_node;
}

auto ColonParser::parse_letter() -> ASTNode
{
	ASTNode letter_node("letter");

	if (m_TokenIterator.Next().value() != PreDefToken::Letter)
	{
		throw std::runtime_error("Expected a 'letter' in letter");
	}
	letter_node.Children().emplace_back(ASTNode("a letter"));

	return letter_node;
}

auto ColonParser::parse_word() -> ASTNode
{
	ASTNode word_node("word");

	word_node.Children().emplace_back(parse_letter());

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &word_node]()
			{
				auto expression_node = parse_word();
				word_node.Children().emplace_back(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
		}
		else
			m_TokenIterator.Reset();
	}

	return word_node;
}

auto ColonParser::parse_number() -> ASTNode
{
	ASTNode number_node("number");

	number_node.Children().emplace_back(parse_digit());

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &number_node]()
			{
				auto expression_node = parse_number();
				number_node.Children().emplace_back(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
		}
		else
			m_TokenIterator.Reset();
	}

	return number_node;
}

auto ColonParser::parse_digit() -> ASTNode
{
	ASTNode digit_node("digit");

	if (m_TokenIterator.Next().value() != PreDefToken::Number)
	{
		throw std::runtime_error("Expected a 'digit' in digit");
	}
	digit_node.Children().emplace_back(ASTNode("digit"));

	return digit_node;
}
