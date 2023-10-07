#pragma once

class ASTNode
{
public:
	ASTNode(std::string const identifier) : m_Identifier(identifier)
	{

	}

	auto Children() -> std::vector<ASTNode>&
	{
		return m_Children;
	}

	auto Identifier() -> std::string const&
	{
		return m_Identifier;
	}
private:
	std::string const m_Identifier;
	std::vector<ASTNode> m_Children;
};

class ColonAST
{
public:
	ColonAST(ASTNode head) : m_Head(std::move(head))
	{

	}


	auto Head() -> ASTNode&
	{
		return m_Head;
	}

private:
	using ASTHead = ASTNode;
	ASTHead m_Head;
};
