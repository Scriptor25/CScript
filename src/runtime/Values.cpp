#include "Values.h"

std::string RuntimeVal::AsJson()
{
	switch (Type())
	{
	case VALUE_BOOL: return BoolVal(*this).AsJson();
	case VALUE_NULL: return NullVal().AsJson();
	case VALUE_NUMBER: return NumberVal(*this).AsJson();
	case VALUE_STRING: return StringVal(*this).AsJson();
	case VALUE_OBJECT: return ObjectVal(*this).AsJson();
	case VALUE_ARRAY: return ArrayVal(*this).AsJson();
	case VALUE_NATIVE_ENV: return NativeEnvVal(*this).AsJson();
	case VALUE_ENVIRONMENT: return EnvironmentVal(*this).AsJson();
	case VALUE_NATIVE_FN: return NativeFnVal(*this).AsJson();
	case VALUE_FUNCTION: return FunctionVal(*this).AsJson();
	default: return "{ \"type\": \"" + ValueTypeToString(Type()) + "\" }";
	}
}

std::string NullVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"value\": \"null\" }";
}

std::string BoolVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"value\": \"" + (Value() ? "true" : "false") + "\" }";
}

std::string NumberVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"value\": \"" + std::to_string(Value()) + "\" }";
}

std::string StringVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"value\": \"" + Value() + "\" }";
}

std::string ObjectVal::AsJson()
{
	std::string properties;

	for (auto property : Properties())
	{
		properties += "{ \"key\": \"" + property.first + "\", \"value\": " + property.second.AsJson() + " }, ";
	}

	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"properties\": [ " + properties + "] }";
}

std::string ArrayVal::AsJson()
{
	std::string values;

	for (RuntimeVal value : Values())
	{
		values += value.AsJson() + ", ";
	}

	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"values\": [ " + values + "], \"size\": \"" + std::to_string(Length()) + "\" }";
}

std::string NativeFnVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"call\": \"<function>\" }";
}

std::string FunctionVal::AsJson()
{
	std::string parameters;

	for (std::string param : Parameters())
	{
		parameters += "\"" + param + "\", ";
	}

	std::string body;

	for (AST::Stmt stmt : Body())
	{
		body += stmt.AsJson() + ", ";
	}

	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"parameters\": [ " + parameters + "], \"declarationEnv\": " + DeclarationEnv()->AsJson() + ", \"body\": [ " + body + "] }";
}

std::string NativeEnvVal::AsJson()
{
	std::string members;

	for (auto member : Members())
	{
		members += "{ \"key\": \"" + member.first + "\", \"value\": " + member.second.AsJson() + " }, ";
	}

	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"body\": [ " + members + "] }";
}

std::string EnvironmentVal::AsJson()
{
	return "{ \"type\": \"" + ValueTypeToString(Type()) + "\", \"scope\": " + Scope()->AsJson() + " }";
}