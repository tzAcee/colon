#pragma once
#include <ColonAST.h>
#include <token-def.h>
#include <TokenIterator.h>
#include <string>

class ColonParserCompliance
{
public:
	ColonParserCompliance(ASTNode& current_node) : m_CurrentNode{ current_node } // make a Copy!
	{

	}

	auto AddToNode(TokenIterator& token_iter, PreDefToken const expected_token)
	{
		auto token_iter_current = token_iter.Get().value();
		if (token_iter_current.Token != expected_token)
		{
			throw std::runtime_error("Expected a Token with ID '"+ std::to_string(static_cast<uint8_t>(expected_token)) +"', but got '" + std::string(1, token_iter_current.SourceInfo.CurrentElem) + "' ["+std::to_string(static_cast<uint8_t>(token_iter_current.Token)) +"].");
		}
		m_CurrentNode.Children().emplace_back(ASTNode(std::string(1, token_iter_current.SourceInfo.CurrentElem)));
		token_iter.Consume();
	}

	auto AddChild(ASTNode node)
	{
		m_CurrentNode.Children().emplace_back(node);
	}

	auto CompletedNode() -> ASTNode&
	{
		return m_CurrentNode;
	}

private:
	ASTNode m_CurrentNode;
};