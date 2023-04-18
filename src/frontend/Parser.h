#pragma once

#include <string>
#include <vector>

#include "Ast.h"
#include "Lexer.h"

class Parser
{
public:
	AST::Program ProduceAST(const std::string sourceCode, bool debug = false);

private:
	bool NotEOF() const
	{
		return At().type != Lexer::TOKEN_EOF;
	};

	Lexer::Token At() const
	{
		return m_Tokens[0];
	}

	Lexer::Token Next() const
	{
		if (NotEOF()) return m_Tokens[1];
		else
		{
			throw ParserException{ "Parser Error: There cannot be a next token because end of file has been reached" };
		}
	}

	Lexer::Token Eat()
	{
		Lexer::Token token = m_Tokens[0];
		m_Tokens.erase(m_Tokens.begin());
		return token;
	}

	Lexer::Token Expect(Lexer::TokenType type, std::string err)
	{
		Lexer::Token token = Eat();
		if (token.type != type)
		{
			throw ParserException{ "Parser Error:\n" + err + "\nGot\n" + FormatJson(token.AsJson()) + "Expected Type '" + Lexer::TokenTypeToString(type) + "'" };
		}
		return token;
	}

	AST::Stmt ParseStmt();
	AST::Stmt ParseVarDeclaration();
	AST::Stmt ParseFnDeclaration();
	AST::Stmt ParseEnvDeclaration();
	AST::Stmt ParseWhileStmt();
	AST::Stmt ParseIfStmt();
	AST::Stmt ParseElseStmt();
	AST::Stmt ParseUsingStmt();
	AST::Expr ParseExpr();
	AST::Expr ParseAssignmentExpr();
	AST::Expr ParseObjectExpr();
	AST::Expr ParseArrayExpr();
	AST::Expr ParseBoolExpr();
	AST::Expr ParseComparisonExpr();
	AST::Expr ParseAdditiveExpr();
	AST::Expr ParseMultiplicativeExpr();
	AST::Expr ParseCallMemberExpr();
	AST::Expr ParseCallExpr(AST::Expr caller);
	std::vector<AST::Expr> ParseArgs();
	std::vector<AST::Expr> ParseArgumentsList();
	AST::Expr ParseMemberExpr();
	AST::Expr ParsePrimaryExpr();
	double ParseFloat(std::string value);

private:
	std::vector<Lexer::Token> m_Tokens;
};