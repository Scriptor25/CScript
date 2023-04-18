#include "Ast.h"

std::string AST::Stmt::AsJson()
{
	switch (Kind())
	{
	case NODE_ARRAY_LITERAL: return ArrayLiteral(*this).AsJson();
	case NODE_ASSIGNMENT_EXPR: return AssignmentExpr(*this).AsJson();
	case NODE_BINARY_EXPR: return BinaryExpr(*this).AsJson();
	case NODE_CALL_EXPR: return CallExpr(*this).AsJson();
	case NODE_ELSE: return ElseStmt(*this).AsJson();
	case NODE_FUNCTION_DECLARATION: return FunctionDeclaration(*this).AsJson();
	case NODE_IDENTIFIER: return Identifier(*this).AsJson();
	case NODE_IF: return IfStmt(*this).AsJson();
	case NODE_MEMBER_EXPR: return MemberExpr(*this).AsJson();
	case NODE_ENV_DECLARATION: return EnvDeclaration(*this).AsJson();
	case NODE_NUMERIC_LITERAL: return NumericLiteral(*this).AsJson();
	case NODE_OBJECT_LITERAL: return ObjectLiteral(*this).AsJson();
	case NODE_PROPERTY: return Property(*this).AsJson();
	case NODE_PROGRAM: return Program(*this).AsJson();
	case NODE_STRING_LITERAL: return StringLiteral(*this).AsJson();
	case NODE_USING: return UsingStmt(*this).AsJson();
	case NODE_VAR_DECLARATION: return VarDeclaration(*this).AsJson();
	case NODE_WHILE: return WhileStmt(*this).AsJson();
	default: return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\" }";
	}
}

std::string AST::Expr::AsJson()
{
	switch (Kind())
	{
	case NODE_ARRAY_LITERAL: return ArrayLiteral(*this).AsJson();
	case NODE_ASSIGNMENT_EXPR: return AssignmentExpr(*this).AsJson();
	case NODE_BINARY_EXPR: return BinaryExpr(*this).AsJson();
	case NODE_CALL_EXPR: return CallExpr(*this).AsJson();
	case NODE_IDENTIFIER: return Identifier(*this).AsJson();
	case NODE_MEMBER_EXPR: return MemberExpr(*this).AsJson();
	case NODE_NUMERIC_LITERAL: return NumericLiteral(*this).AsJson();
	case NODE_OBJECT_LITERAL: return ObjectLiteral(*this).AsJson();
	case NODE_PROPERTY: return Property(*this).AsJson();
	case NODE_STRING_LITERAL: return StringLiteral(*this).AsJson();
	default: return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\" }";
	}
}

std::string AST::AssignmentExpr::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"assigne\": " + Assigne().AsJson() + ", \"value\": " + Value().AsJson() + " }";
}

std::string AST::CallExpr::AsJson()
{
	std::string args;

	for (Expr expr : Args())
	{
		args += expr.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"args\": [ " + args + "], \"caller\": " + Caller().AsJson() + " }";
}

std::string AST::MemberExpr::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"object\": " + Object().AsJson() + ", \"property\": " + Property().AsJson() + ", \"computed\": \"" + (Computed() ? "true" : "false") + "\" }";
}

std::string AST::BinaryExpr::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"left\": " + Left().AsJson() + ", \"right\": " + Right().AsJson() + ", \"operator\": \"" + Operator() + "\" }";
}

std::string AST::Identifier::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"symbol\": \"" + Symbol() + "\" }";
}

std::string AST::NumericLiteral::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"value\": \"" + std::to_string(Value()) + "\" }";
}

std::string AST::Property::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"key\": \"" + Key() + "\", \"value\": " + Value().AsJson() + " }";
}

std::string AST::ObjectLiteral::AsJson()
{
	std::string properties;

	for (Property property : Properties())
	{
		properties += property.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"properties\": [ \n" + properties + "] }";
}

std::string AST::ArrayLiteral::AsJson()
{
	std::string values;

	for (Expr value : Values())
	{
		values += value.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"values\": [ " + values + "], \"size\": \"" + std::to_string(Values().size()) + "\" }";
}

std::string AST::StringLiteral::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"value\": \"" + Value() + "\" }";
}

std::string AST::Program::AsJson()
{
	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"body\": [ \n" + body + "] }";
}

std::string AST::VarDeclaration::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"constant\": \"" + (Constant() ? "true" : "false") + "\", \"identifier\": \"" + Identifier() + "\", \"value\": " + Value().AsJson() + " }";
}

std::string AST::FunctionDeclaration::AsJson()
{
	std::string parameters;

	for (std::string param : Parameters())
	{
		parameters += param + ", ";
	}

	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"parameters\": [ " + parameters + "], \"name\": \"" + Name() + "\", \"body\": [ " + body + "] }";
}

std::string AST::EnvDeclaration::AsJson()
{
	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"name\": \"" + Name() + "\", \"body\": [ " + body + "] }";
}

std::string AST::UsingStmt::AsJson()
{
	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"name\": \"" + Name() + "\", \"type\": \"" + Type() + "\" }";
}

std::string AST::WhileStmt::AsJson()
{
	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"condition\": " + Condition().AsJson() + ", \"body\": [ " + body + "] }";
}

std::string AST::IfStmt::AsJson()
{
	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"condition\": " + Condition().AsJson() + ", \"body\": [ " + body + "], \"else\": " + Else().AsJson() + " }";
}

std::string AST::ElseStmt::AsJson()
{
	std::string body;

	for (Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"kind\": \"" + NodeTypeToString(Kind()) + "\", \"body\": [ " + body + "] }";
}

std::string AST::NodeTypeToString(NodeType type)
{
	switch (type)
	{
	case NODE_ARRAY_LITERAL: return "ArrayLiteral";
	case NODE_ASSIGNMENT_EXPR: return "AssignmentExpr";
	case NODE_BINARY_EXPR: return "BinaryExpr";
	case NODE_CALL_EXPR: return "CallExpr";
	case NODE_ELSE: return "Else";
	case NODE_FUNCTION_DECLARATION: return "FunctionDeclaration";
	case NODE_IDENTIFIER: return "Identifier";
	case NODE_IF: return "If";
	case NODE_MEMBER_EXPR: return "MemberExpr";
	case NODE_ENV_DECLARATION: return "EnvironmentDeclaration";
	case NODE_NUMERIC_LITERAL: return "NumericLiteral";
	case NODE_OBJECT_LITERAL: return "ObjectLiteral";
	case NODE_PROGRAM: return "Program";
	case NODE_PROPERTY: return "Property";
	case NODE_STRING_LITERAL: return "StringLiteral";
	case NODE_USING: return "Using";
	case NODE_VAR_DECLARATION: return "VarDeclaration";
	case NODE_WHILE: return "While";
	default: return "Undefined";
	}
}

bool AST::IsUndefined(Stmt stmt)
{
	return stmt.Kind() == NODE_UNDEFINED;
}