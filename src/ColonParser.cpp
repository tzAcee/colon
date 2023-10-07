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
	ColonParserCompliance compliance(function_unparam_node);

	compliance.AddChild(parse_def());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorParenthesesOpen);
	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorParenthesesClose);
	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorCurlyBracesOpen);

	compliance.AddChild(parse_expression());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorCurlyBracesClose);

	return compliance.CompletedNode();
}

auto ColonParser::parse_def() -> ASTNode
{
	ASTNode def_node("def");
	ColonParserCompliance compliance(def_node);

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorColon);

	compliance.AddChild(parse_word());

	return compliance.CompletedNode();
}

auto ColonParser::parse_expression() -> ASTNode
{
	//ParseFallThrough::parse_with_fall_through([this]() { parse_digit(); });
	ASTNode def_node("expression");
	ColonParserCompliance compliance(def_node);

	{
		// Parse assignment | assignment expression
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto assign_node = parse_assignment();
				compliance.AddChild(std::move(assign_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();

			m_TokenIterator.Remember();
			result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
				{
					auto expression_node = parse_expression();
					compliance.AddChild(std::move(expression_node));
				});
			if (result == ParseFallThrough::ParseState::SUCCESS)
				m_TokenIterator.Forget();
			else
				m_TokenIterator.Reset();

			return compliance.CompletedNode();
		}
		m_TokenIterator.Reset();
	}

	// Parse functionCall | functionCall expression
	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto fc_call_node = parse_functionCall();
				compliance.AddChild(std::move(fc_call_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();

			m_TokenIterator.Remember();
			result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
				{
					auto expression_node = parse_expression();
					compliance.AddChild(std::move(expression_node));
				});
			if (result == ParseFallThrough::ParseState::SUCCESS)
				m_TokenIterator.Forget();
			else
				m_TokenIterator.Reset();

			return compliance.CompletedNode();
		}
		m_TokenIterator.Reset();
	}

	throw std::runtime_error("Could not parse expression.");
}

auto ColonParser::parse_functionCall() -> ASTNode
{
	ASTNode function_call_node("function call");
	ColonParserCompliance compliance(function_call_node);

	compliance.AddChild(parse_word());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorParenthesesOpen);

	m_TokenIterator.Remember();
	auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
		{
			auto expression_node = parse_variableList();
			compliance.AddChild(std::move(expression_node));
		});
	if (result == ParseFallThrough::ParseState::SUCCESS)
		m_TokenIterator.Forget();
	else
		m_TokenIterator.Reset();

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorParenthesesClose);

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorSemicolon);

	return compliance.CompletedNode();
}

auto ColonParser::parse_variableList() -> ASTNode
{
	ASTNode variable_list_node("variable list");
	ColonParserCompliance compliance(variable_list_node);
	compliance.AddChild(parse_word());

	// TODO check comma

	return compliance.CompletedNode();
}

auto ColonParser::parse_assignment() -> ASTNode
{
	ASTNode assign_node("assignment");
	ColonParserCompliance compliance(assign_node);

	compliance.AddChild(parse_def());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorAssign);

	compliance.AddChild(parse_variableValue());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorSemicolon);

	return compliance.CompletedNode();
}

auto ColonParser::parse_variableValue() -> ASTNode
{
	ASTNode assign_node("variable value");
	ColonParserCompliance compliance(assign_node);

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto expression_node = parse_string();
				compliance.AddChild(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
			return compliance.CompletedNode();
		}
		else
			m_TokenIterator.Reset();
	}

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto expression_node = parse_number();
				compliance.AddChild(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
			return compliance.CompletedNode();
		}
		else
			m_TokenIterator.Reset();
	}

	throw std::runtime_error("could not parse variable value");
}

auto ColonParser::parse_string() -> ASTNode
{
	ASTNode string_node("string");
	ColonParserCompliance compliance(string_node);
	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorQuote);

	compliance.AddChild(parse_word());

	compliance.AddToNode(m_TokenIterator, PreDefToken::OperatorQuote);

	return compliance.CompletedNode();
}

auto ColonParser::parse_letter() -> ASTNode
{	ASTNode letter_node("letter");

	ColonParserCompliance compliance(letter_node);
	compliance.AddToNode(m_TokenIterator, PreDefToken::Letter);

	return compliance.CompletedNode();

}

auto ColonParser::parse_word() -> ASTNode
{
	ASTNode word_node("word");
	ColonParserCompliance compliance(word_node);
	compliance.AddChild(parse_letter());

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto expression_node = parse_word();
				compliance.AddChild(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
		}
		else
			m_TokenIterator.Reset();
	}

	return compliance.CompletedNode();
}

auto ColonParser::parse_number() -> ASTNode
{
	ASTNode number_node("number");
	ColonParserCompliance compliance(number_node);
	compliance.AddChild(parse_digit());

	{
		m_TokenIterator.Remember();
		auto result = ParseFallThrough::parse_with_fall_through([this, &compliance]()
			{
				auto expression_node = parse_number();
				compliance.AddChild(std::move(expression_node));
			});
		if (result == ParseFallThrough::ParseState::SUCCESS)
		{
			m_TokenIterator.Forget();
		}
		else
			m_TokenIterator.Reset();
	}

	return compliance.CompletedNode();
}

auto ColonParser::parse_digit() -> ASTNode
{
	ASTNode digit_node("digit");
	ColonParserCompliance compliance(digit_node);

	compliance.AddToNode(m_TokenIterator, PreDefToken::Number);

	return compliance.CompletedNode();
}
