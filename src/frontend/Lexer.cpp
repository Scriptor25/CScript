#include "Lexer.h"

static std::map<std::string, Lexer::TokenType> KEYWORDS =
{
	{ "let", Lexer::TOKEN_LET },
	{ "const", Lexer::TOKEN_CONST },
	{ "fn", Lexer::TOKEN_FN },
	{ "while", Lexer::TOKEN_WHILE },
	{ "if", Lexer::TOKEN_IF },
	{ "else", Lexer::TOKEN_ELSE },
	{ "env", Lexer::TOKEN_ENV },
	{ "using", Lexer::TOKEN_USING },
};

std::string Lexer::TokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TOKEN_NUMBER: return "Number";
	case TOKEN_IDENTIFIER: return "Identifier";
	case TOKEN_STRING: return "String";

	case TOKEN_LET: return "Let";
	case TOKEN_CONST: return "Const";
	case TOKEN_FN: return "Function";
	case TOKEN_WHILE: return "While";
	case TOKEN_IF: return "If";
	case TOKEN_ELSE: return "Else";
	case TOKEN_ENV: return "Environment";
	case TOKEN_USING: return "Using";

	case TOKEN_BINARY_OPERATOR: return "Binary Operator";
	case TOKEN_EQUALS: return "Equals";
	case TOKEN_COMMA: return "Comma";
	case TOKEN_DOT: return "Dot";
	case TOKEN_COLON: return "Colon";
	case TOKEN_SEMICOLON: return "Semicolon";
	case TOKEN_OPEN_PAREN: return "Open Paranthesis";
	case TOKEN_CLOSE_PAREN: return "Close Paranthesis";
	case TOKEN_OPEN_BRACE: return "Open Brace";
	case TOKEN_CLOSE_BRACE: return "Close Brace";
	case TOKEN_OPEN_BRACKET: return "Open Bracket";
	case TOKEN_CLOSE_BRACKET: return "Close Bracket";
	case TOKEN_EXCLAMATION: return "Exclamation Mark";

	case TOKEN_EOF: return "End Of File";

	default: return "Undefined";
	}
}

bool Lexer::IsAlpha(const char c)
{
	return std::toupper(c) != std::tolower(c) || c == '_';
}

bool Lexer::IsSkippable(const char c)
{
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool Lexer::IsNumber(const char c)
{
	return c >= '0' && c <= '9';
}

void Lexer::Push(bool isNum, std::string& num, bool isIdent, std::string& ident, std::vector<Token>& tokens, int line)
{
	if (!isNum && num != "")
	{
		tokens.push_back(Token{ num, TOKEN_NUMBER, line });
		num = "";
	}
	if (!isIdent && ident != "")
	{
		if (KEYWORDS.count(ident))
		{
			TokenType reserved = KEYWORDS[ident];
			tokens.push_back(Token{ ident, reserved, line });
		}
		else
		{
			tokens.push_back(Token{ ident, TOKEN_IDENTIFIER, line });
		}

		ident = "";
	}
}

std::vector<Lexer::Token> Lexer::Tokenize(const std::string source)
{

	std::vector<Token> tokens;

	std::string num = "";
	std::string ident = "";

	bool isString = false;
	std::string str = "";

	int line = 1;
	bool isComment = false;

	for (const char c : source)
	{
		bool isNum = false;
		bool isIdent = false;

		if (c == '\n')
		{
			line++;
		}

		if (c == '#')
		{
			if (!isString)
			{
				isComment = !isComment;
				continue;
			}
		}

		if (isComment)
		{
			continue;
		}

		if (c == '"')
		{
			if (isString)
			{
				tokens.push_back(Token{ str, TOKEN_STRING, line });
				str = "";
			}

			isString = !isString;
			continue;
		}

		if (isString)
		{
			str += c;
			continue;
		}

		if (IsNumber(c))
		{
			isNum = true;
			num += c;
			continue;
		}

		if (IsAlpha(c))
		{
			isIdent = true;
			ident += c;
			continue;
		}

		Push(isNum, num, isIdent, ident, tokens, line);

		if (IsSkippable(c))
		{
			continue;
		}

		TokenType type = TOKEN_EOF;
		switch (c)
		{
		case '(':
			type = TOKEN_OPEN_PAREN;
			break;

		case ')':
			type = TOKEN_CLOSE_PAREN;
			break;

		case '{':
			type = TOKEN_OPEN_BRACE;
			break;

		case '}':
			type = TOKEN_CLOSE_BRACE;
			break;

		case '[':
			type = TOKEN_OPEN_BRACKET;
			break;

		case ']':
			type = TOKEN_CLOSE_BRACKET;
			break;

		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '<':
		case '>':
		case '|':
		case '&':
			type = TOKEN_BINARY_OPERATOR;
			break;

		case '=':
			type = TOKEN_EQUALS;
			break;

		case ';':
			type = TOKEN_SEMICOLON;
			break;

		case ':':
			type = TOKEN_COLON;
			break;

		case ',':
			type = TOKEN_COMMA;
			break;

		case '.':
			type = TOKEN_DOT;
			break;

		case '!':
			type = TOKEN_EXCLAMATION;
			break;

		default:
			throw LexerException{ "Unreconized character found in source: '" + std::string(1, c) + "' on line " + std::to_string(line) };
		}

		tokens.push_back(Token{ std::string(1, c), type, line });
	}

	Push(false, num, false, ident, tokens, line);

	tokens.push_back(Token{ "EndOfFile", TOKEN_EOF });

	return tokens;
}