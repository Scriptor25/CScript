#pragma once

#include <map>

#include "../frontend/Ast.h"
#include "Environment.h"

class Environment;

enum ValueType
{
	VALUE_UNDEFINED,

	VALUE_NULL,
	VALUE_BOOL,
	VALUE_NUMBER,
	VALUE_STRING,
	VALUE_OBJECT,
	VALUE_ARRAY,
	VALUE_NATIVE_FN,
	VALUE_FUNCTION,
	VALUE_NATIVE_ENV,
	VALUE_ENVIRONMENT,
};

struct RuntimeVal : ValueObject
{
	RuntimeVal()
	{
		SetValue<ValueType>("type", VALUE_UNDEFINED);
	}

	ValueType Type()
	{
		return GetValue<ValueType>("type");
	}

	std::string AsJson();
};

struct NullVal : RuntimeVal
{
	NullVal()
	{
		SetValue<ValueType>("type", VALUE_NULL);
	}

	std::string AsJson();
};

struct BoolVal : RuntimeVal
{
	BoolVal(bool value)
	{
		SetValue<ValueType>("type", VALUE_BOOL);
		SetValue<bool&>("value", *new bool(value));
	}

	BoolVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_BOOL);
		SetValue<bool&>("value", val.GetValue<bool&>("value", *new bool(false)));
	}

	bool& Value()
	{
		return GetValue<bool&>("value");
	}

	std::string AsJson();
};

struct NumberVal : RuntimeVal
{
	NumberVal(double value)
	{
		SetValue<ValueType>("type", VALUE_NUMBER);
		SetValue<double&>("value", *new double(value));
	}

	NumberVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_NUMBER);
		SetValue<double&>("value", val.GetValue<double&>("value", *new double(0.0)));
	}

	double& Value()
	{
		return GetValue<double&>("value");
	}

	std::string AsJson();
};

struct StringVal : RuntimeVal
{
	StringVal(std::string value)
	{
		SetValue<ValueType>("type", VALUE_STRING);
		SetValue<std::string&>("value", *new std::string(value));
	}

	StringVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_STRING);
		SetValue<std::string&>("value", val.GetValue<std::string&>("value"));
	}

	std::string& Value()
	{
		return GetValue<std::string&>("value");
	}

	std::string AsJson();
};

struct ObjectVal : RuntimeVal
{
	ObjectVal()
	{
		SetValue<ValueType>("type", VALUE_OBJECT);
		SetValue<std::map<std::string, RuntimeVal>&>("properties", *new std::map<std::string, RuntimeVal>());
	}

	ObjectVal(std::map<std::string, RuntimeVal> properties)
	{
		SetValue<ValueType>("type", VALUE_OBJECT);
		SetValue<std::map<std::string, RuntimeVal>&>("properties", *new std::map<std::string, RuntimeVal>(properties));
	}

	ObjectVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_OBJECT);
		SetValue<std::map<std::string, RuntimeVal>&>("properties", val.GetValue<std::map<std::string, RuntimeVal>&>("properties"));
	}

	std::map<std::string, RuntimeVal>& Properties()
	{
		return GetValue<std::map<std::string, RuntimeVal>&>("properties");
	}

	std::string AsJson();
};

struct ArrayVal : RuntimeVal
{
	ArrayVal(std::vector<RuntimeVal> values)
	{
		SetValue<ValueType>("type", VALUE_ARRAY);
		SetValue<std::vector<RuntimeVal>&>("values", *new std::vector<RuntimeVal>(values));
	}

	ArrayVal(size_t size)
	{
		SetValue<ValueType>("type", VALUE_ARRAY);
		SetValue<std::vector<RuntimeVal>&>("values", *new std::vector<RuntimeVal>(size));
	}

	ArrayVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_ARRAY);
		SetValue<std::vector<RuntimeVal>&>("values", val.GetValue<std::vector<RuntimeVal>&>("values"));
	}

	std::vector<RuntimeVal>& Values()
	{
		return GetValue<std::vector<RuntimeVal>&>("values");
	}

	size_t Length()
	{
		return Values().size();
	}

	std::string AsJson();
};

__interface INativeFnCall
{
	RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env);
};

struct NativeFnVal : RuntimeVal
{
	NativeFnVal(INativeFnCall* call)
	{
		SetValue<ValueType>("type", VALUE_NATIVE_FN);
		SetValue<INativeFnCall&>("call", *call);
	}

	NativeFnVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_NATIVE_FN);
		SetValue<INativeFnCall&>("call", val.GetValue<INativeFnCall&>("call"));
	}

	INativeFnCall& Call()
	{
		return GetValue<INativeFnCall&>("call");
	}

	std::string AsJson();
};

struct FunctionVal : RuntimeVal
{
	FunctionVal(std::vector<std::string> parameters, std::vector<AST::Stmt> body, Environment* declarationEnv)
	{
		SetValue<ValueType>("type", VALUE_FUNCTION);
		SetValue<std::vector<std::string>&>("parameters", *new std::vector<std::string>(parameters));
		SetValue<std::vector<AST::Stmt>&>("body", *new std::vector<AST::Stmt>(body));
		SetValue<Environment*>("declarationEnv", declarationEnv);
	}

	FunctionVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_FUNCTION);
		SetValue<std::vector<std::string>&>("parameters", val.GetValue<std::vector<std::string>&>("parameters"));
		SetValue<std::vector<AST::Stmt>&>("body", val.GetValue<std::vector<AST::Stmt>&>("body"));
		SetValue<Environment*>("declarationEnv", val.GetValue<Environment*>("declarationEnv"));
	}

	std::vector<std::string>& Parameters()
	{
		return GetValue<std::vector<std::string>&>("parameters");
	}

	Environment* DeclarationEnv()
	{
		return GetValue<Environment*>("declarationEnv");
	}

	std::vector<AST::Stmt>& Body()
	{
		return GetValue<std::vector<AST::Stmt>&>("body");
	}

	std::string AsJson();
};

struct NativeEnvVal : RuntimeVal
{
	NativeEnvVal(std::map<std::string, RuntimeVal> members)
	{
		SetValue<ValueType>("type", VALUE_NATIVE_ENV);
		SetValue<std::map<std::string, RuntimeVal>&>("members", *new std::map<std::string, RuntimeVal>(members));
	}

	NativeEnvVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_NATIVE_ENV);
		SetValue<std::map<std::string, RuntimeVal>&>("members", val.GetValue<std::map<std::string, RuntimeVal>&>("members"));
	}

	std::map<std::string, RuntimeVal>& Members()
	{
		return GetValue<std::map<std::string, RuntimeVal>&>("members");
	}

	std::string AsJson();
};

struct EnvironmentVal : RuntimeVal
{
	EnvironmentVal(Environment* scope)
	{
		SetValue<ValueType>("type", VALUE_ENVIRONMENT);
		SetValue<Environment*>("scope", scope);
	}

	EnvironmentVal(RuntimeVal val)
	{
		SetValue<ValueType>("type", VALUE_ENVIRONMENT);
		SetValue<Environment*>("scope", val.GetValue<Environment*>("scope"));
	}

	Environment* Scope()
	{
		return GetValue<Environment*>("scope");
	}

	std::string AsJson();
};

static std::string ValueAsString(RuntimeVal val)
{
	switch (val.Type())
	{
	case VALUE_BOOL:
		return BoolVal(val).Value() ? "true" : "false";

	case VALUE_NULL:
		return "null";

	case VALUE_NUMBER:
		return std::to_string(NumberVal(val).Value());

	case VALUE_STRING:
		return StringVal(val).Value();

	case VALUE_ARRAY:
	{
		std::string array;

		for (RuntimeVal runtimeVal : ArrayVal(val).Values())
		{
			array += ValueAsString(runtimeVal) + "\n";
		}

		return array;
	}

	default:
		return FormatJson(val.AsJson());
	}
}

static std::string ValueTypeToString(ValueType type)
{
	switch (type)
	{
	case VALUE_NULL: return "Null";
	case VALUE_NUMBER: return "Number";
	case VALUE_STRING: return "String";
	case VALUE_BOOL: return "Bool";
	case VALUE_OBJECT: return "Object";
	case VALUE_ARRAY: return "Array";
	case VALUE_NATIVE_ENV: return "NativeEnv";
	case VALUE_ENVIRONMENT: return "Environment";
	case VALUE_NATIVE_FN: return "NativeFn";
	case VALUE_FUNCTION: return "Function";
	default: return "Undefined";
	}
}
