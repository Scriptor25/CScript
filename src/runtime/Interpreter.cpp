#include "Interpreter.h"

RuntimeVal Interpreter::Evaluate(AST::Stmt astNode, Environment* env)
{
	switch (astNode.Kind())
	{
	case AST::NODE_NUMERIC_LITERAL:
		return NumberVal(AST::NumericLiteral(astNode).Value());

	case AST::NODE_IDENTIFIER:
		return EvaluateIdentifier(astNode, env);

	case AST::NODE_STRING_LITERAL:
		return StringVal(AST::StringLiteral(astNode).Value());

	case AST::NODE_OBJECT_LITERAL:
		return EvaluateObjectExpr(astNode, env);

	case AST::NODE_ARRAY_LITERAL:
		return EvaluateArrayExpr(astNode, env);

	case AST::NODE_CALL_EXPR:
		return EvaluateCallExpr(astNode, env);

	case AST::NODE_MEMBER_EXPR:
		return EvaluateMemberExpr(astNode, env);

	case AST::NODE_ASSIGNMENT_EXPR:
		return EvaluateAssignment(astNode, env);

	case AST::NODE_BINARY_EXPR:
		return EvaluateBinaryExpr(astNode, env);

	case AST::NODE_PROGRAM:
		return EvaluateProgram(astNode, env);

	case AST::NODE_VAR_DECLARATION:
		return EvaluateVarDeclaration(astNode, env);

	case AST::NODE_FUNCTION_DECLARATION:
		return EvaluateFunctionDeclaration(astNode, env);

	case AST::NODE_ENV_DECLARATION:
		return EvaluateEnvDeclaration(astNode, env);

	case AST::NODE_WHILE:
		return EvaluateWhileStmt(astNode, env);

	case AST::NODE_IF:
		return EvaluateIfStmt(astNode, env);

		/*case AST::NODE_USING:
			return EvaluateUsingStmt(astNode, env);*/

	default:
		throw InterpreterException{ "This AST Node has not yet been setup for interpretation: \n" + FormatJson(astNode.AsJson()) };

	}
}