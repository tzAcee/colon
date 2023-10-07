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
