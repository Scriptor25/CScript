#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "../builtin/Exceptions.h"

class Lexer
{
public:
	enum TokenType
	{
		TOKEN_NUMBER,
		TOKEN_IDENTIFIER,
		TOKEN_STRING,

		TOKEN_LET,
		TOKEN_CONST,
		TOKEN_FN,
		TOKEN_WHILE,
		TOKEN_IF,
		TOKEN_ELSE,
		TOKEN_ENV,
		TOKEN_USING,

		TOKEN_BINARY_OPERATOR,	// + - * / % < > & |
		TOKEN_EQUALS,			// =
		TOKEN_COMMA,			// ,
		TOKEN_DOT,				// .
		TOKEN_COLON,			// :
		TOKEN_SEMICOLON,		// ;
		TOKEN_OPEN_PAREN,		// (
		TOKEN_CLOSE_PAREN,		// )
		TOKEN_OPEN_BRACE,		// {
		TOKEN_CLOSE_BRACE,		// }
		TOKEN_OPEN_BRACKET,		// [
		TOKEN_CLOSE_BRACKET,	// ]
		TOKEN_EXCLAMATION,		// !

		TOKEN_EOF,
	};

	struct Token
	{
		std::string value;
		TokenType type;
		int line;

		std::string AsJson() const
		{
			return "{ \"type\": \"" + TokenTypeToString(type) + "\", \"value\": \"" + value + "\", \"line\": \"" + std::to_string(line) + "\" }";
		}
	};

	static std::string TokenTypeToString(TokenType type);

	static bool IsAlpha(const char c);

	static bool IsSkippable(const char c);

	static bool IsNumber(const char c);

	static void Push(bool isNum, std::string& num, bool isIdent, std::string& ident, std::vector<Token>& tokens, int line);

	static std::vector<Token> Tokenize(const std::string source);
};
