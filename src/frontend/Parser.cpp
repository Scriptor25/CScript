#include "Parser.h"

//#define DEBUG_OUTPUT

AST::Program Parser::ProduceAST(const std::string source, bool debug)
{
	m_Tokens = Lexer::Tokenize(source);

	if (debug) for (Lexer::Token token : m_Tokens) std::cout << token.AsJson() << std::endl;

	AST::Program program;

	while (NotEOF())
	{
		program.Body().push_back(ParseStmt());
	}

	return program;
}

AST::Stmt Parser::ParseStmt()
{
	switch (At().type)
	{
	case Lexer::TOKEN_LET:
	case Lexer::TOKEN_CONST:
		return ParseVarDeclaration();
	case Lexer::TOKEN_FN:
		return ParseFnDeclaration();
	case Lexer::TOKEN_WHILE:
		return ParseWhileStmt();
	case Lexer::TOKEN_IF:
		return ParseIfStmt();
	case Lexer::TOKEN_ENV:
		return ParseEnvDeclaration();
	case Lexer::TOKEN_USING:
		return ParseUsingStmt();
	default:
		return ParseExpr();
	}
}

AST::Stmt Parser::ParseVarDeclaration()
{
	const bool isConstant = Eat().type == Lexer::TOKEN_CONST;
	const std::string identifier = Expect(Lexer::TOKEN_IDENTIFIER, "Expected identifier name following let | const keywords").value;

	if (At().type == Lexer::TOKEN_SEMICOLON)
	{
		Eat();
		if (isConstant)
		{
			throw ParserException{ "Must assign value to constant expression: No value provided" };
		}

		return AST::VarDeclaration(isConstant, identifier);
	}

	Expect(Lexer::TOKEN_EQUALS, "Expected equals token following identifier in var declaration");
	const AST::VarDeclaration declaration(isConstant, identifier, ParseExpr());

	Expect(Lexer::TOKEN_SEMICOLON, "Variable declaration statement must end with semicolon");
	return declaration;
}

AST::Stmt Parser::ParseFnDeclaration()
{
	Eat();

	const std::string name = Expect(Lexer::TOKEN_IDENTIFIER, "Expected function name following fn keyword").value;

	std::vector<std::string> params;

	const std::vector<AST::Expr> args = ParseArgs();
	for (AST::Expr arg : args)
	{
		if (arg.Kind() != AST::NODE_IDENTIFIER)
		{
			throw ParserException{ "Inside function declaration expected parameters to be of type string:\n" + FormatJson(arg.AsJson(), true) };
		}

		params.push_back(AST::Identifier(arg).Symbol());
	}

	Expect(Lexer::TOKEN_OPEN_BRACE, "Expected function body following declaration");

	std::vector<AST::Stmt> body;
	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		body.push_back(ParseStmt());
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Closing brace expected inside function declaration");

	return AST::FunctionDeclaration(params, name, body);
}

AST::Stmt Parser::ParseEnvDeclaration()
{
	Eat();

	const std::string name = Expect(Lexer::TOKEN_IDENTIFIER, "Expected environment name following env keyword").value;

	Expect(Lexer::TOKEN_OPEN_BRACE, "Expected environment body following declaration");

	std::vector<AST::Stmt> body;
	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		body.push_back(ParseStmt());
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Closing brace expected inside environment declaration");

	return AST::EnvDeclaration(name, body);
}

AST::Stmt Parser::ParseWhileStmt()
{
	Eat();
	const AST::Expr condition = ParsePrimaryExpr();

	Expect(Lexer::TOKEN_OPEN_BRACE, "Open brace expected inside while statement");

	std::vector<AST::Stmt> body;
	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		body.push_back(ParseStmt());
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Closing brace expected inside while statement");

	return AST::WhileStmt(condition, body);
}

AST::Stmt Parser::ParseIfStmt()
{
	Eat();
	const AST::Expr condition = ParsePrimaryExpr();

	Expect(Lexer::TOKEN_OPEN_BRACE, "Open brace expected inside if statement");

	std::vector<AST::Stmt> body;
	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		body.push_back(ParseStmt());
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Closing brace expected inside if statement");

	if (At().type == Lexer::TOKEN_ELSE)
	{
		return AST::IfStmt(condition, body, ParseElseStmt());
	}

	return AST::IfStmt(condition, body);
}

AST::Stmt Parser::ParseElseStmt()
{
	Eat();
	Expect(Lexer::TOKEN_OPEN_BRACE, "Open brace expected inside else statement");

	std::vector<AST::Stmt> body;
	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		body.push_back(ParseStmt());
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Closing brace expected inside else statement");

	return AST::ElseStmt(body);
}

AST::Stmt Parser::ParseUsingStmt()
{
	Eat();

	if (At().type != Lexer::TOKEN_IDENTIFIER)
	{
		if (At().type != Lexer::TOKEN_STRING)
		{
			throw ParserException{ "Expected token type identifier or string after using token:\n" + FormatJson(At().AsJson()) };
		}

		std::string path = Eat().value;

		Expect(Lexer::TOKEN_SEMICOLON, "Missing semicolon after using statement");

		return AST::UsingStmt(path, "path");
	}

	std::string envname = Eat().value;

	Expect(Lexer::TOKEN_SEMICOLON, "Missing semicolon after using statement");

	return AST::UsingStmt(envname, "env");
}

AST::Expr Parser::ParseExpr()
{
	return ParseAssignmentExpr();
}

AST::Expr Parser::ParseAssignmentExpr()
{
	const AST::Expr left = ParseArrayExpr();

	if (At().type == Lexer::TOKEN_EQUALS)
	{
		Eat();
		const AST::Expr value = ParseAssignmentExpr();
		return AST::AssignmentExpr(left, value);
	}

	return left;
}

AST::Expr Parser::ParseArrayExpr()
{
	if (At().type != Lexer::TOKEN_OPEN_BRACKET)
	{
		return ParseObjectExpr();
	}

	Eat();

	std::vector<AST::Expr> values;

	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACKET)
	{
		AST::Expr value = ParseExpr();

		values.push_back(value);
		if (At().type != Lexer::TOKEN_CLOSE_BRACKET)
		{
			Expect(Lexer::TOKEN_COMMA, "Expected comma or closing bracket following value");
		}
	}

	Expect(Lexer::TOKEN_CLOSE_BRACKET, "Array expression missing closing bracket");
	return AST::ArrayLiteral(values);
}

AST::Expr Parser::ParseObjectExpr()
{
	if (At().type != Lexer::TOKEN_OPEN_BRACE)
	{
		return ParseBoolExpr();
	}

	Eat();
	std::vector<AST::Property> properties;

	while (NotEOF() && At().type != Lexer::TOKEN_CLOSE_BRACE)
	{
		const std::string key = Expect(Lexer::TOKEN_IDENTIFIER, "Object literal key expected").value;

		if (At().type == Lexer::TOKEN_COMMA)
		{
			Eat();
			properties.push_back(AST::Property(key));
			continue;
		}
		else if (At().type == Lexer::TOKEN_CLOSE_BRACE)
		{
			properties.push_back(AST::Property(key));
			continue;
		}

		Expect(Lexer::TOKEN_COLON, "Missing colon identifier in object literal");
		const AST::Expr value = ParseExpr();

		properties.push_back(AST::Property(key, value));
		if (At().type != Lexer::TOKEN_CLOSE_BRACE)
		{
			Expect(Lexer::TOKEN_COMMA, "Expected comma or closing bracket following property");
		}
	}

	Expect(Lexer::TOKEN_CLOSE_BRACE, "Object literal missing closing brace");
	return AST::ObjectLiteral(properties);
}

AST::Expr Parser::ParseBoolExpr()
{
	AST::Expr left = ParseComparisonExpr();

	while (At().value == "&" || At().value == "|")
	{
		const std::string op = Eat().value;
		const AST::Expr right = ParseComparisonExpr();

		left = AST::BinaryExpr{
			left,
			right,
			op
		};
	}

	return left;
}

AST::Expr Parser::ParseComparisonExpr()
{
	AST::Expr left = ParseAdditiveExpr();

	while (At().value == "<" || At().value == ">" || (At().value == "=" && Next().value == "=") || (At().value == "!" && Next().value == "="))
	{
		Lexer::Token tk = Eat();
		std::string op = tk.value;

		Lexer::Token other = At();

		if (other.value == "=")
		{
			op += Eat().value;
		}
		else if (op == "=" || op == "!")
		{
			throw ParserException{ "Missing '=' after '=' or '!' operator for binary comparison:\n" + FormatJson(tk.AsJson()) + "\nAnd\n" + FormatJson(other.AsJson()) };

		}

		const AST::Expr right = ParseAdditiveExpr();

		left = AST::BinaryExpr{
			left,
			right,
			op
		};
	}

	return left;
}

AST::Expr Parser::ParseAdditiveExpr()
{
	AST::Expr left = ParseMultiplicativeExpr();

	while (At().value == "+" || At().value == "-")
	{
		const std::string op = Eat().value;
		const AST::Expr right = ParseMultiplicativeExpr();

		left = AST::BinaryExpr{
			left,
			right,
			op
		};
	}

	return left;
}

AST::Expr Parser::ParseMultiplicativeExpr()
{
	AST::Expr left = ParseCallMemberExpr();

	while (At().value == "*" || At().value == "/" || At().value == "%")
	{
		const std::string op = Eat().value;
		const AST::Expr right = ParseCallMemberExpr();

		left = AST::BinaryExpr{
			left,
			right,
			op
		};
	}

	return left;
}

AST::Expr Parser::ParseCallMemberExpr()
{
	const AST::Expr member = ParseMemberExpr();

	if (At().type == Lexer::TOKEN_OPEN_PAREN)
	{
		return ParseCallExpr(member);
	}

	return member;
}

AST::Expr Parser::ParseCallExpr(AST::Expr caller)
{
	AST::CallExpr call_expr(ParseArgs(), caller);

	if (At().type == Lexer::TOKEN_OPEN_PAREN)
	{
		call_expr = ParseCallExpr(call_expr);
	}

	return call_expr;
}

std::vector<AST::Expr> Parser::ParseArgs()
{
	Expect(Lexer::TOKEN_OPEN_PAREN, "Expected open paranthesis");

	std::vector<AST::Expr> args = At().type == Lexer::TOKEN_CLOSE_PAREN ? std::vector<AST::Expr>() : ParseArgumentsList();

	Expect(Lexer::TOKEN_CLOSE_PAREN, "Missing closing paranthesis after args list");
	return args;
}

std::vector<AST::Expr> Parser::ParseArgumentsList()
{
	std::vector<AST::Expr> args;
	args.push_back(ParseAssignmentExpr());

	while (NotEOF() && At().type == Lexer::TOKEN_COMMA)
	{
		Eat();
		args.push_back(ParseAssignmentExpr());
	}

	return args;
}

AST::Expr Parser::ParseMemberExpr()
{
	AST::Expr object = ParsePrimaryExpr();

	while (At().type == Lexer::TOKEN_DOT || At().type == Lexer::TOKEN_OPEN_BRACKET)
	{
		const Lexer::Token op = Eat();

		AST::Expr property;
		bool computed;

		if (op.type == Lexer::TOKEN_DOT)
		{
			computed = false;
			property = ParsePrimaryExpr();

			if (property.Kind() != AST::NODE_IDENTIFIER)
			{
				throw ParserException{ "Cannot use dot operator without right hand side being an identifier" };
			}
		}
		else
		{
			computed = true;
			property = ParseExpr();
			Expect(Lexer::TOKEN_CLOSE_BRACKET, "Missing closing bracket in computed value");
		}

		object = AST::MemberExpr(object, property, computed);
	}

	return object;
}

AST::Expr Parser::ParsePrimaryExpr()
{
	const Lexer::Token token = Eat();

	switch (token.type)
	{
	case Lexer::TOKEN_IDENTIFIER:
		return AST::Identifier(token.value);

	case Lexer::TOKEN_NUMBER:
	{
		std::string val = token.value;
		if (At().type == Lexer::TOKEN_DOT)
		{
			Eat();
			val += ".";
			if (At().type == Lexer::TOKEN_NUMBER)
			{
				val += Eat().value;
			}
		}

		return AST::NumericLiteral(ParseFloat(val));
	}

	case Lexer::TOKEN_STRING:
		return AST::StringLiteral(token.value);

	case Lexer::TOKEN_OPEN_PAREN:
	{
		const AST::Expr value = ParseExpr();
		Expect(Lexer::TOKEN_CLOSE_PAREN, "Unexpected token found inside paranthesized expression");
		return value;
	}

	default:
		throw ParserException{ "Unexpected token found during parsing: \n" + FormatJson(token.AsJson()) };
	}
}

double Parser::ParseFloat(std::string value)
{
	return std::stod(value);
}
