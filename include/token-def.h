#pragma once
#include <cstdint>

enum class PreDefToken : uint8_t
{
	// Brackets:
	OperatorSemicolon,
	OperatorParenthesesOpen,
	OperatorParenthesesClose,
	OperatorCurlyBracesOpen,
	OperatorCurlyBracesClose,
	OperatorSquareBracketsOpen,
	OperatorSquareBracketsClose,

	// Arithmetical:
	OperatorAdd,
	OperatorSub,
	OperatorMult,
	OperatorDiv,

	// Language Specific:
	OperatorArrow,
	OperatorColon,
	OperatorQuote,

	// Logical:
	OperatorAssign,
	OperatorLogicalAnd,
	OperatorLogicalNot,
	OperatorLogicalOr,
	OperatorGreater,
	OperatorLess,

	// Literal:
	Number,
	Letter,

	EndOfFile
};

struct SourceInformation
{
	char CurrentElem;
	uint16_t Line = 0;
	uint16_t Column = 0;
};

struct TokenMeta 
{
	PreDefToken Token;
	SourceInformation SourceInfo;
};