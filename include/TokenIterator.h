#pragma once
#include <vector>
#include "token-def.h"
#include <optional>
#include <cstdint>
#include <stdexcept>
#include <stack>

class TokenIterator
{
public:
	TokenIterator(std::vector<TokenMeta> const& tokens) : m_Tokens{ tokens }
	{
	}

	auto Next() -> std::optional<TokenMeta const>
	{
		try
		{
			return m_Tokens.at(m_Index++);
		}
		catch (std::out_of_range const&)
		{
			return std::nullopt;
		}
	}

	auto Consume() -> void
	{
		m_Index++;
	}

	auto Get() -> std::optional<TokenMeta const>
	{
		return LookAhead(0);
	}

	auto LookAhead(uint8_t const n) -> std::optional<TokenMeta const>
	{
		try
		{
			return m_Tokens.at(m_Index+n);
		}
		catch (std::out_of_range const&)
		{
			return std::nullopt;
		}
	}

	auto LookBehind(uint8_t const n) -> std::optional<TokenMeta const>
	{
		try
		{
			return m_Tokens.at(m_Index - n);
		}
		catch (std::out_of_range const&)
		{
			return std::nullopt;
		}
	}

	auto Remember()
	{
		m_RememberedIndices.push(m_Index);
	}

	auto Forget()
	{
		m_RememberedIndices.pop();
	}

	auto Reset()
	{
		m_Index = m_RememberedIndices.top();
		m_RememberedIndices.pop();
	}
private:
	std::vector<TokenMeta> const m_Tokens;
	size_t m_Index = 0;

	std::stack<size_t> m_RememberedIndices;
};