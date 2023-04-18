#include "../Interpreter.h"

RuntimeVal Interpreter::EvaluateProgram(AST::Program program, Environment* env)
{
	RuntimeVal lastEvaluated = NullVal();

	for (AST::Stmt stmt : program.Body())
	{
		lastEvaluated = Evaluate(stmt, env);
	}

	return lastEvaluated;
}

RuntimeVal Interpreter::EvaluateVarDeclaration(AST::VarDeclaration declaration, Environment* env)
{
	RuntimeVal value = Evaluate(declaration.Value(), env);
	return env->DeclareVar(declaration.Identifier(), value, declaration.Constant());
}

RuntimeVal Interpreter::EvaluateFunctionDeclaration(AST::FunctionDeclaration declaration, Environment* env)
{
	FunctionVal fn(declaration.Parameters(), declaration.Body(), env);
	return env->DeclareVar(declaration.Name(), fn, true);
}

RuntimeVal Interpreter::EvaluateEnvDeclaration(AST::EnvDeclaration declaration, Environment* env)
{
	Environment* scope = new Environment(env);

	for (AST::Stmt stmt : declaration.Body())
	{
		Evaluate(stmt, scope);
	}

	EnvironmentVal nameSpace(scope);
	return env->DeclareVar(declaration.Name(), nameSpace, true);
}

RuntimeVal Interpreter::EvaluateWhileStmt(AST::WhileStmt loop, Environment* env)
{
	RuntimeVal lastEvaluated = NullVal();

	while (BoolVal(Evaluate(loop.Condition(), env)).Value())
	{
		Environment* scope = new Environment(env);
		for (AST::Stmt stmt : loop.Body())
		{
			lastEvaluated = Evaluate(stmt, scope);
		}
	}

	return lastEvaluated;
}

RuntimeVal Interpreter::EvaluateIfStmt(AST::IfStmt loop, Environment* env)
{
	RuntimeVal lastEvaluated = NullVal();

	if (BoolVal(Evaluate(loop.Condition(), env)).Value())
	{
		Environment* scope = new Environment(env);
		for (AST::Stmt stmt : loop.Body())
		{
			lastEvaluated = Evaluate(stmt, scope);
		}
	}
	else
	{
		lastEvaluated = EvaluateElseStmt(loop.Else(), env);
	}

	return lastEvaluated;
}

RuntimeVal Interpreter::EvaluateElseStmt(AST::ElseStmt loop, Environment* env)
{
	RuntimeVal lastEvaluated = NullVal();

	Environment* scope = new Environment(env);
	for (AST::Stmt stmt : loop.Body())
	{
		lastEvaluated = Evaluate(stmt, scope);
	}

	return lastEvaluated;
}

RuntimeVal Interpreter::EvaluateUsingStmt(AST::UsingStmt stmt, Environment* env)
{
	if (stmt.Type() == "env")
	{
		RuntimeVal runtimeVal = env->LookupVar(stmt.Name());
		if (runtimeVal.Type() == VALUE_NATIVE_ENV)
		{
			for (auto member : NativeEnvVal(runtimeVal).Members())
			{
				AST::Stmt stmt;
				if (member.second.Type() == VALUE_ENVIRONMENT)
				{
					
				}
				else if (member.second.Type() == VALUE_NATIVE_ENV)
				{

				}

				Evaluate(stmt, env);
			}
		}
		else if (runtimeVal.Type() == VALUE_ENVIRONMENT)
		{
			env->CopyEnv(EnvironmentVal(runtimeVal).Scope());
		}
	}

	return NullVal();
}