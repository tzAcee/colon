#pragma once
#include <string_view>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include <cctype>

#include "token-def.h"

template<typename T>
class ColonLexer
{
public:
    ColonLexer(auto const& source) : m_Source{ std::move(source) }
    { }

    auto lex() const -> std::vector<PreDefToken> const
    {
        std::vector<PreDefToken> return_token_list;

        std::for_each(m_Source.begin(), m_Source.end(), [this, &return_token_list](auto current_char)
            {
                auto token = get_pre_def_token(current_char);
                if (token.has_value()) {
                    return_token_list.emplace_back(token.value());
                }
            });

        if (return_token_list.size() != m_Source.size())
        {
            throw std::exception{ "Invalid lexing, source length doesn't match tokenized values length." };
        }

        return return_token_list;
    }

private:
    auto get_pre_def_token(T const& current_elem) const -> std::optional<PreDefToken> const
    {
        auto lexem_variant = is_fixed_lexeme(current_elem);
        if (lexem_variant.has_value()) {
            return lexem_variant.value();
        }

        lexem_variant = is_number(current_elem);
        if (lexem_variant.has_value()) {
            return lexem_variant.value();
        }

        lexem_variant = is_letter(current_elem);
        if (lexem_variant.has_value()) {
            return lexem_variant.value();
        }

        return std::nullopt;
    }

    auto is_fixed_lexeme(T const& current_elem) const -> std::optional<PreDefToken> const
    {
        try
        {
            return m_FixedLexemes.at(current_elem);
        }
        catch (std::out_of_range const& )
        {
            return std::nullopt;
        }
    }

    auto is_number(T const& current_elem) const -> std::optional<PreDefToken> const
    {
        if (std::isdigit(current_elem))
        {
            return PreDefToken::Number;
        }
        else
        {
            return std::nullopt;
        }
    }

    auto is_letter(T const& current_elem) const -> std::optional<PreDefToken> const
    {
        if (std::isalpha(current_elem))
        {
            return PreDefToken::Letter;
        }
        else
        {
            return std::nullopt; // Indicate that it's not a letter.
        }
    }

private:
    std::string_view const m_Source;

    std::unordered_map<T, PreDefToken> const m_FixedLexemes = {
        { ';', PreDefToken::OperatorSemicolon },
        { '(', PreDefToken::OperatorParenthesesOpen },
        { ')', PreDefToken::OperatorParenthesesClose },
        { '{', PreDefToken::OperatorCurlyBracesOpen },
        { '}', PreDefToken::OperatorCurlyBracesClose },
        { '[', PreDefToken::OperatorSquareBracketsOpen },
        { ']', PreDefToken::OperatorSquareBracketsClose },
        { '+', PreDefToken::OperatorAdd },
        { '-', PreDefToken::OperatorSub },
        { '*', PreDefToken::OperatorMult },
        { '/', PreDefToken::OperatorDiv },
        { '>', PreDefToken::OperatorGreater },
        { '<', PreDefToken::OperatorLess },
        { '=', PreDefToken::OperatorAssign },
        { '&', PreDefToken::OperatorLogicalAnd },
        { '!', PreDefToken::OperatorLogicalNot },
        { '|', PreDefToken::OperatorLogicalOr },
        { '-', PreDefToken::OperatorArrow },
        { ':', PreDefToken::OperatorColon },
        { '"', PreDefToken::OperatorQuote }
    };
};
