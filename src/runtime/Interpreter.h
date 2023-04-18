#pragma once

#include <string>
#include <vector>

#include "../builtin/Exceptions.h"

#include "../frontend/Ast.h"

#include "Environment.h"
#include "Values.h"

class Interpreter
{
public:
	static RuntimeVal Evaluate(AST::Stmt astNode, Environment* env);

private:
	static RuntimeVal EvaluateProgram(AST::Program program, Environment* env);
	static RuntimeVal EvaluateVarDeclaration(AST::VarDeclaration declaration, Environment* env);
	static RuntimeVal EvaluateFunctionDeclaration(AST::FunctionDeclaration declaration, Environment* env);
	static RuntimeVal EvaluateEnvDeclaration(AST::EnvDeclaration declaration, Environment* env);
	static RuntimeVal EvaluateWhileStmt(AST::WhileStmt loop, Environment* env);
	static RuntimeVal EvaluateIfStmt(AST::IfStmt loop, Environment* env);
	static RuntimeVal EvaluateElseStmt(AST::ElseStmt loop, Environment* env);
	static RuntimeVal EvaluateUsingStmt(AST::UsingStmt stmt, Environment* env);
	static NumberVal EvaluateNumericBinaryExpr(NumberVal lhs, NumberVal rhs, std::string op);
	static StringVal EvaluateStringBinaryExpr(RuntimeVal lhs, RuntimeVal rhs, std::string op);
	static BoolVal EvaluateComparisonBinaryExpr(RuntimeVal lhs, RuntimeVal rhs, std::string op);
	static RuntimeVal EvaluateBinaryExpr(AST::BinaryExpr binop, Environment* env);
	static RuntimeVal EvaluateIdentifier(AST::Identifier ident, Environment* env);
	static RuntimeVal EvaluateAssignment(AST::AssignmentExpr node, Environment* env);
	static RuntimeVal EvaluateObjectExpr(AST::ObjectLiteral obj, Environment* env);
	static RuntimeVal EvaluateArrayExpr(AST::ArrayLiteral arr, Environment* env);
	static RuntimeVal EvaluateCallExpr(AST::CallExpr expr, Environment* env);
	static RuntimeVal EvaluateMemberExpr(AST::MemberExpr expr, Environment* env);
};
