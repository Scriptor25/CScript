#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../builtin/Util.h"

class AST
{
public:
	enum NodeType
	{
		NODE_UNDEFINED,

		/* Statements */
		NODE_PROGRAM,
		NODE_VAR_DECLARATION,
		NODE_FUNCTION_DECLARATION,
		NODE_ENV_DECLARATION,
		NODE_USING,
		NODE_WHILE,
		NODE_IF,
		NODE_ELSE,

		/* Expressions */
		NODE_ASSIGNMENT_EXPR,
		NODE_CALL_EXPR,
		NODE_MEMBER_EXPR,

		/* Literals */
		NODE_BINARY_EXPR,
		NODE_IDENTIFIER,
		NODE_NUMERIC_LITERAL,
		NODE_OBJECT_LITERAL,
		NODE_ARRAY_LITERAL,
		NODE_PROPERTY,
		NODE_STRING_LITERAL,
	};

	struct Stmt : ValueObject
	{
		Stmt()
		{
			SetValue<NodeType>("kind", NODE_UNDEFINED);
		}

		NodeType Kind()
		{
			return GetValue<NodeType>("kind");
		}

		std::string AsJson();
	};

	struct Expr : Stmt
	{
		Expr()
		{
			SetValue<NodeType>("kind", NODE_UNDEFINED);
		}

		std::string AsJson();
	};

	struct AssignmentExpr : Expr
	{
		AssignmentExpr()
		{
			SetValue<NodeType>("kind", NODE_ASSIGNMENT_EXPR);
			SetValue<Expr&>("assigne", *new Expr());
			SetValue<Expr&>("value", *new Expr());
		}

		AssignmentExpr(Expr assigne, Expr value)
		{
			SetValue<NodeType>("kind", NODE_ASSIGNMENT_EXPR);
			SetValue<Expr&>("assigne", *new Expr(assigne));
			SetValue<Expr&>("value", *new Expr(value));
		}

		AssignmentExpr(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_ASSIGNMENT_EXPR);
			SetValue<Expr&>("assigne", stmt.GetValue<Expr&>("assigne"));
			SetValue<Expr&>("value", stmt.GetValue<Expr&>("value"));
		}

		Expr& Assigne()
		{
			return GetValue<Expr&>("assigne");
		}

		Expr& Value()
		{
			return GetValue<Expr&>("value");
		}

		std::string AsJson();
	};

	struct CallExpr : Expr
	{
		CallExpr()
		{
			SetValue<NodeType>("kind", NODE_CALL_EXPR);
			SetValue<std::vector<Expr>&>("args", *new std::vector<Expr>());
			SetValue<Expr&>("caller", *new Expr());
		}

		CallExpr(std::vector<Expr> args, Expr caller)
		{
			SetValue<NodeType>("kind", NODE_CALL_EXPR);
			SetValue<std::vector<Expr>&>("args", *new std::vector<Expr>(args));
			SetValue<Expr&>("caller", *new Expr(caller));
		}

		CallExpr(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_CALL_EXPR);
			SetValue<std::vector<Expr>&>("args", stmt.GetValue<std::vector<Expr>&>("args"));
			SetValue<Expr&>("caller", stmt.GetValue<Expr&>("caller"));
		}

		std::vector<Expr>& Args()
		{
			return GetValue<std::vector<Expr>&>("args");
		}

		Expr& Caller()
		{
			return GetValue<Expr&>("caller");
		}

		std::string AsJson();
	};

	struct MemberExpr : Expr
	{
		MemberExpr()
		{
			SetValue<NodeType>("kind", NODE_MEMBER_EXPR);
			SetValue<Expr&>("object", *new Expr());
			SetValue<Expr&>("property", *new Expr());
			SetValue<bool&>("computed", *new bool());
		}

		MemberExpr(Expr object, Expr property, bool computed)
		{
			SetValue<NodeType>("kind", NODE_MEMBER_EXPR);
			SetValue<Expr&>("object", *new Expr(object));
			SetValue<Expr&>("property", *new Expr(property));
			SetValue<bool&>("computed", *new bool(computed));
		}

		MemberExpr(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_MEMBER_EXPR);
			SetValue<Expr&>("object", stmt.GetValue<Expr&>("object"));
			SetValue<Expr&>("property", stmt.GetValue<Expr&>("property"));
			SetValue<bool&>("computed", stmt.GetValue<bool&>("computed"));
		}

		Expr& Object()
		{
			return GetValue<Expr&>("object");
		}

		Expr& Property()
		{
			return GetValue<Expr&>("property");
		}

		bool& Computed()
		{
			return GetValue<bool&>("computed");
		}

		std::string AsJson();
	};

	struct BinaryExpr : Expr
	{
		BinaryExpr()
		{
			SetValue<NodeType>("kind", NODE_BINARY_EXPR);
			SetValue<Expr&>("left", *new Expr());
			SetValue<Expr&>("right", *new Expr());
			SetValue<std::string&>("operator", *new std::string());
		}

		BinaryExpr(Expr left, Expr right, std::string op)
		{
			SetValue<NodeType>("kind", NODE_BINARY_EXPR);
			SetValue<Expr&>("left", *new Expr(left));
			SetValue<Expr&>("right", *new Expr(right));
			SetValue<std::string&>("operator", *new std::string(op));
		}

		BinaryExpr(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_BINARY_EXPR);
			SetValue<Expr&>("left", stmt.GetValue<Expr&>("left"));
			SetValue<Expr&>("right", stmt.GetValue<Expr&>("right"));
			SetValue<std::string&>("operator", stmt.GetValue<std::string&>("operator"));
		}

		Expr& Left()
		{
			return GetValue<Expr&>("left");
		}

		Expr& Right()
		{
			return GetValue<Expr&>("right");
		}

		std::string& Operator()
		{
			return GetValue<std::string&>("operator");
		}

		std::string AsJson();
	};

	struct Identifier : Expr
	{
		Identifier(std::string symbol)
		{
			SetValue<NodeType>("kind", NODE_IDENTIFIER);
			SetValue<std::string&>("symbol", *new std::string(symbol));
		}

		Identifier(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_IDENTIFIER);
			SetValue<std::string&>("symbol", stmt.GetValue<std::string&>("symbol"));
		}

		std::string& Symbol()
		{
			return GetValue<std::string&>("symbol");
		}

		std::string AsJson();
	};

	struct NumericLiteral : Expr
	{
		NumericLiteral(double value)
		{
			SetValue<NodeType>("kind", NODE_NUMERIC_LITERAL);
			SetValue<double&>("value", *new double(value));
		}

		NumericLiteral(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_NUMERIC_LITERAL);
			SetValue<double&>("value", stmt.GetValue<double&>("value"));
		}

		double& Value()
		{
			return GetValue<double&>("value");
		}

		std::string AsJson();
	};

	struct Property : Expr
	{
		Property(std::string key, Expr value)
		{
			SetValue<NodeType>("kind", NODE_PROPERTY);
			SetValue<std::string&>("key", *new std::string(key));
			SetValue<Expr&>("value", *new Expr(value));
		}

		Property(std::string key)
		{
			SetValue<NodeType>("kind", NODE_PROPERTY);
			SetValue<std::string&>("key", *new std::string(key));
			SetValue<Expr&>("value", *new Identifier("null"));
		}

		Property(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_PROPERTY);
			SetValue<std::string&>("key", stmt.GetValue<std::string&>("key"));
			SetValue<Expr&>("value", stmt.GetValue<Expr&>("value"));
		}

		std::string& Key()
		{
			return GetValue<std::string&>("key");
		}

		Expr& Value()
		{
			return GetValue<Expr&>("value");
		}

		std::string AsJson();
	};

	struct ObjectLiteral : Expr
	{
		ObjectLiteral()
		{
			SetValue<NodeType>("kind", NODE_OBJECT_LITERAL);
			SetValue<std::vector<Property>&>("properties", *new std::vector<Property>());
		}

		ObjectLiteral(std::vector<Property> properties)
		{
			SetValue<NodeType>("kind", NODE_OBJECT_LITERAL);
			SetValue<std::vector<Property>&>("properties", *new std::vector<Property>(properties));
		}

		ObjectLiteral(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_OBJECT_LITERAL);
			SetValue<std::vector<Property>&>("properties", stmt.GetValue<std::vector<Property>&>("properties"));
		}

		std::vector<Property>& Properties()
		{
			return GetValue<std::vector<Property>&>("properties");
		}

		std::string AsJson();
	};

	struct ArrayLiteral : Expr
	{
		ArrayLiteral()
		{
			SetValue<NodeType>("kind", NODE_ARRAY_LITERAL);
			SetValue<std::vector<Expr>&>("values", *new std::vector<Expr>());
		}

		ArrayLiteral(std::vector<Expr> values)
		{
			SetValue<NodeType>("kind", NODE_ARRAY_LITERAL);
			SetValue<std::vector<Expr>&>("values", *new std::vector<Expr>(values));
		}

		ArrayLiteral(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_ARRAY_LITERAL);
			SetValue<std::vector<Expr>&>("values", stmt.GetValue<std::vector<Expr>&>("values"));
		}

		std::vector<Expr>& Values()
		{
			return GetValue<std::vector<Expr>&>("values");
		}

		std::string AsJson();
	};

	struct StringLiteral : Expr
	{
		StringLiteral(std::string value)
		{
			SetValue<NodeType>("kind", NODE_STRING_LITERAL);
			SetValue<std::string&>("value", *new std::string(value));
		}

		StringLiteral(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_STRING_LITERAL);
			SetValue<std::string&>("value", stmt.GetValue<std::string&>("value"));
		}

		std::string& Value()
		{
			return GetValue<std::string&>("value");
		}

		std::string AsJson();
	};

	struct Program : Stmt
	{
		Program()
		{
			SetValue<NodeType>("kind", NODE_PROGRAM);
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>());
		}

		Program(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_PROGRAM);
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		std::string AsJson();
	};

	struct VarDeclaration : Stmt
	{
		VarDeclaration(bool constant, std::string identifier, Expr value)
		{
			SetValue<NodeType>("kind", NODE_VAR_DECLARATION);
			SetValue<bool&>("constant", *new bool(constant));
			SetValue<std::string&>("identifier", *new std::string(identifier));
			SetValue<Expr&>("value", *new Expr(value));
		}

		VarDeclaration(bool constant, std::string identifier)
		{
			SetValue<NodeType>("kind", NODE_VAR_DECLARATION);
			SetValue<bool&>("constant", *new bool(constant));
			SetValue<std::string&>("identifier", *new std::string(identifier));
			SetValue<Expr&>("value", *new AST::Identifier("null"));
		}

		VarDeclaration(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_VAR_DECLARATION);
			SetValue<bool&>("constant", stmt.GetValue<bool&>("constant"));
			SetValue<std::string&>("identifier", stmt.GetValue<std::string&>("identifier"));
			SetValue<Expr&>("value", stmt.GetValue<Expr&>("value"));
		}

		bool& Constant()
		{
			return GetValue<bool&>("constant");
		}

		std::string& Identifier()
		{
			return GetValue<std::string&>("identifier");
		}

		Expr& Value()
		{
			return GetValue<Expr&>("value");
		}

		std::string AsJson();
	};

	struct FunctionDeclaration : Stmt
	{
		FunctionDeclaration(std::vector<std::string> parameters, std::string name, std::vector<Stmt> body)
		{
			SetValue<NodeType>("kind", NODE_FUNCTION_DECLARATION);
			SetValue<std::vector<std::string>&>("parameters", *new std::vector<std::string>(parameters));
			SetValue<std::string&>("name", *new std::string(name));
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
		}

		FunctionDeclaration(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_FUNCTION_DECLARATION);
			SetValue<std::vector<std::string>&>("parameters", stmt.GetValue<std::vector<std::string>&>("parameters"));
			SetValue<std::string&>("name", stmt.GetValue<std::string&>("name"));
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
		}

		std::vector<std::string>& Parameters()
		{
			return GetValue<std::vector<std::string>&>("parameters");
		}

		std::string& Name()
		{
			return GetValue<std::string&>("name");
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		std::string AsJson();
	};

	struct EnvDeclaration : Stmt
	{
		EnvDeclaration(std::string name, std::vector<Stmt> body)
		{
			SetValue<NodeType>("kind", NODE_ENV_DECLARATION);
			SetValue<std::string&>("name", *new std::string(name));
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
		}

		EnvDeclaration(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_ENV_DECLARATION);
			SetValue<std::string&>("name", stmt.GetValue<std::string&>("name"));
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
		}

		std::string& Name()
		{
			return GetValue<std::string&>("name");
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		std::string AsJson();
	};

	struct UsingStmt : Stmt
	{
		UsingStmt(std::string name, std::string type)
		{
			SetValue<NodeType>("kind", NODE_USING);
			SetValue<std::string&>("name", *new std::string(name));
			SetValue<std::string&>("type", *new std::string(type));
		}

		UsingStmt(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_USING);
			SetValue<std::string&>("name", stmt.GetValue<std::string&>("name"));
			SetValue<std::string&>("type", stmt.GetValue<std::string&>("type"));
		}

		std::string& Name()
		{
			return GetValue<std::string&>("name");
		}

		std::string& Type()
		{
			return GetValue<std::string&>("type");
		}

		std::string AsJson();
	};

	struct WhileStmt : Stmt
	{
		WhileStmt(Expr condition, std::vector<Stmt> body)
		{
			SetValue<NodeType>("kind", NODE_WHILE);
			SetValue<Expr&>("condition", *new Expr(condition));
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
		}

		WhileStmt(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_WHILE);
			SetValue<Expr&>("condition", stmt.GetValue<Expr&>("condition"));
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
		}

		Expr& Condition()
		{
			return GetValue<Expr&>("condition");
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		std::string AsJson();
	};

	struct ElseStmt : Stmt
	{
		ElseStmt()
		{
			SetValue<NodeType>("kind", NODE_ELSE);
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>());
		}

		ElseStmt(std::vector<Stmt> body)
		{
			SetValue<NodeType>("kind", NODE_ELSE);
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
		}

		ElseStmt(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_ELSE);
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		std::string AsJson();
	};

	struct IfStmt : Stmt
	{
		IfStmt(Expr condition, std::vector<Stmt> body)
		{
			SetValue<NodeType>("kind", NODE_IF);
			SetValue<Expr&>("condition", *new Expr(condition));
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
			SetValue<ElseStmt&>("else", *new ElseStmt());
		}

		IfStmt(Expr condition, std::vector<Stmt> body, ElseStmt elseStmt)
		{
			SetValue<NodeType>("kind", NODE_IF);
			SetValue<Expr&>("condition", *new Expr(condition));
			SetValue<std::vector<Stmt>&>("body", *new std::vector<Stmt>(body));
			SetValue<ElseStmt&>("else", *new ElseStmt(elseStmt));
		}

		IfStmt(Stmt stmt)
		{
			SetValue<NodeType>("kind", NODE_IF);
			SetValue<Expr&>("condition", stmt.GetValue<Expr&>("condition"));
			SetValue<std::vector<Stmt>&>("body", stmt.GetValue<std::vector<Stmt>&>("body"));
			SetValue<ElseStmt&>("else", stmt.GetValue<ElseStmt&>("else"));
		}

		Expr& Condition()
		{
			return GetValue<Expr&>("condition");
		}

		std::vector<Stmt>& Body()
		{
			return GetValue<std::vector<Stmt>&>("body");
		}

		ElseStmt& Else()
		{
			return GetValue<ElseStmt&>("else");
		}

		std::string AsJson();
	};

	static std::string NodeTypeToString(NodeType type);
	static bool IsUndefined(Stmt stmt);
};
