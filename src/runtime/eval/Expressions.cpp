#include "../Interpreter.h"

NumberVal Interpreter::EvaluateNumericBinaryExpr(NumberVal lhs, NumberVal rhs, std::string op)
{
	const double v0 = lhs.Value();
	const double v1 = rhs.Value();
	double result = 0.0;

	if (op == "+") result = v0 + v1;
	else if (op == "-") result = v0 - v1;
	else if (op == "*") result = v0 * v1;
	else if (op == "/") result = v0 / v1;
	else if (op == "%") result = std::fmod(v0, v1);
	else
	{
		throw InterpreterException{ "Operator '" + op + "' is undefined for binary expression of type number" };
	}

	return NumberVal(result);
}

StringVal Interpreter::EvaluateStringBinaryExpr(RuntimeVal lhs, RuntimeVal rhs, std::string op)
{
	const std::string v0 = ValueAsString(lhs);
	const std::string v1 = ValueAsString(rhs);

	if (op == "+") return StringVal(v0 + v1);
	else
	{
		throw InterpreterException{ "Operator '" + op + "' is undefined for binary expression of type string" };
	}
}

BoolVal Interpreter::EvaluateComparisonBinaryExpr(RuntimeVal lhs, RuntimeVal rhs, std::string op)
{
	if (op == "==")
	{
		return ValueAsString(lhs) == ValueAsString(rhs);
	}
	else if (op == "!=")
	{
		return ValueAsString(lhs) != ValueAsString(rhs);
	}

	if (lhs.Type() == VALUE_NUMBER && rhs.Type() == VALUE_NUMBER)
	{
		if (op == "<")
		{
			return NumberVal(lhs).Value() < NumberVal(rhs).Value();
		}
		else if (op == ">")
		{
			return NumberVal(lhs).Value() > NumberVal(rhs).Value();
		}
		else if (op == "<=")
		{
			return NumberVal(lhs).Value() <= NumberVal(rhs).Value();
		}
		else if (op == ">=")
		{
			return NumberVal(lhs).Value() >= NumberVal(rhs).Value();
		}
		else
		{
			throw InterpreterException{ "Operator '" + op + "' is undefined for type number" };
		}
	}

	if (lhs.Type() == VALUE_BOOL && rhs.Type() == VALUE_BOOL)
	{
		if (op == "|")
		{
			return BoolVal(lhs).Value() || BoolVal(rhs).Value();
		}
		else if (op == "&")
		{
			return BoolVal(lhs).Value() && BoolVal(rhs).Value();
		}
		else
		{
			throw InterpreterException{ "Operator '" + op + "' is undefined for type boolean" };
		}
	}

	throw InterpreterException{ "Operator '" + op + "' is undefined for comparison of given types: \n" + FormatJson(lhs.AsJson()) + "\nAnd\n" + FormatJson(rhs.AsJson()) };
}

RuntimeVal Interpreter::EvaluateBinaryExpr(AST::BinaryExpr binop, Environment* env)
{
	RuntimeVal lhs = Evaluate(binop.Left(), env);
	RuntimeVal rhs = Evaluate(binop.Right(), env);

	std::set<std::string> ops = { "<", ">", "<=", ">=", "==", "!=", "&", "|" };

	if (ops.count(binop.Operator()))
	{
		return EvaluateComparisonBinaryExpr(lhs, rhs, binop.Operator());
	}

	if (lhs.Type() == VALUE_NUMBER && rhs.Type() == VALUE_NUMBER)
	{
		return EvaluateNumericBinaryExpr(lhs, rhs, binop.Operator());
	}

	if (lhs.Type() == VALUE_STRING || rhs.Type() == VALUE_STRING)
	{
		return EvaluateStringBinaryExpr(lhs, rhs, binop.Operator());
	}

	return NullVal();
}

RuntimeVal Interpreter::EvaluateIdentifier(AST::Identifier ident, Environment* env)
{
	return env->LookupVar(ident.Symbol());
}

RuntimeVal Interpreter::EvaluateAssignment(AST::AssignmentExpr node, Environment* env)
{
	if (node.Assigne().Kind() != AST::NODE_IDENTIFIER)
	{
		if (node.Assigne().Kind() == AST::NODE_MEMBER_EXPR)
		{
			AST::MemberExpr assigne = node.Assigne();

			RuntimeVal runtimeVal = Evaluate(assigne.Object(), env);

			if (runtimeVal.Type() == VALUE_OBJECT)
			{
				return ObjectVal(runtimeVal).Properties()[assigne.Computed() ? AST::StringLiteral(assigne.Property()).Value() : AST::Identifier(assigne.Property()).Symbol()] = Evaluate(node.Value(), env);
			}

			if (runtimeVal.Type() == VALUE_ARRAY)
			{
				if (assigne.Computed())
				{
					std::vector<RuntimeVal>& values = ArrayVal(runtimeVal).Values();

					size_t position = size_t(NumberVal(Evaluate(assigne.Property(), env)).Value());

					size_t p = values.size();

					while (p <= position)
					{
						values.push_back(NullVal());
						p++;
					}

					return values[position] = Evaluate(node.Value(), env);
				}
			}
		}

		throw InterpreterException{ "Invalid LHS inside assignment expression: \n" + FormatJson(node.Assigne().AsJson()) };
	}

	const std::string varname = AST::Identifier(node.Assigne()).Symbol();
	return env->AssignVar(varname, Evaluate(node.Value(), env));
}

RuntimeVal Interpreter::EvaluateObjectExpr(AST::ObjectLiteral obj, Environment* env)
{
	std::map<std::string, RuntimeVal> properties;

	for (AST::Property property : obj.Properties())
	{
		std::string key = property.Key();
		AST::Expr value = property.Value();

		RuntimeVal runtimeVal = (AST::IsUndefined(value) ? env->LookupVar(key) : Evaluate(value, env));

		properties[key] = runtimeVal;
	}

	ObjectVal object(properties);

	return object;
}

RuntimeVal Interpreter::EvaluateArrayExpr(AST::ArrayLiteral arr, Environment* env)
{
	std::vector<RuntimeVal> values;

	for (AST::Expr value : arr.Values())
	{
		RuntimeVal runtimeVal = Evaluate(value, env);
		values.push_back(runtimeVal);
	}

	ArrayVal array(values);

	return array;
}

RuntimeVal Interpreter::EvaluateCallExpr(AST::CallExpr expr, Environment* env)
{
	std::vector<RuntimeVal> args;
	for (AST::Expr arg : expr.Args())
	{
		args.push_back(Evaluate(arg, env));
	}

	RuntimeVal fn = Evaluate(expr.Caller(), env);
	RuntimeVal result;

	if (fn.Type() == VALUE_NATIVE_FN)
	{
		return NativeFnVal(fn).Call().Call(args, env);
	}

	if (fn.Type() == VALUE_FUNCTION)
	{
		FunctionVal func = fn;
		Environment* scope = new Environment(func.DeclarationEnv());

		for (int i = 0; i < func.Parameters().size(); i++)
		{
			const std::string varname = func.Parameters()[i];
			scope->DeclareVar(varname, args[i], false);
		}

		RuntimeVal result = NullVal();
		for (AST::Stmt stmt : func.Body())
		{
			result = Evaluate(stmt, scope);
		}

		return result;
	}

	throw InterpreterException{ "Cannot call value that is not a function: " + FormatJson(fn.AsJson()) };
}

RuntimeVal Interpreter::EvaluateMemberExpr(AST::MemberExpr expr, Environment* env)
{
	RuntimeVal runtimeVal = Evaluate(expr.Object(), env);

	if (runtimeVal.Type() == VALUE_OBJECT)
	{
		return ObjectVal(runtimeVal).Properties()[expr.Computed() ? AST::StringLiteral(expr.Property()).Value() : AST::Identifier(expr.Property()).Symbol()];
	}

	if (runtimeVal.Type() == VALUE_ARRAY)
	{
		if (expr.Computed())
		{
			std::vector<RuntimeVal>& values = ArrayVal(runtimeVal).Values();

			size_t position = size_t(NumberVal(Evaluate(expr.Property(), env)).Value());

			if (position < values.size())
			{
				return values[position];
			}

			return NullVal();
		}
		else
		{
			std::string ident = AST::Identifier(expr.Property()).Symbol();
			if (ident == "length")
			{
				return NumberVal(double(ArrayVal(runtimeVal).Length()));
			}
		}
	}

	if (runtimeVal.Type() == VALUE_ENVIRONMENT)
	{
		return EnvironmentVal(runtimeVal).Scope()->LookupVar(expr.Computed() ? AST::StringLiteral(expr.Property()).Value() : AST::Identifier(expr.Property()).Symbol());
	}

	if (runtimeVal.Type() == VALUE_NATIVE_ENV)
	{
		return NativeEnvVal(runtimeVal).Members()[expr.Computed() ? AST::StringLiteral(expr.Property()).Value() : AST::Identifier(expr.Property()).Symbol()];
	}

	throw InterpreterException{ "Invalid member expression: \n" + FormatJson(expr.AsJson()) };
}