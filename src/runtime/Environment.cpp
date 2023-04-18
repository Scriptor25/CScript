#include "Environment.h"

Environment* Environment::Resolve(std::string varname)
{
	if (m_Variables.count(varname))
		return this;

	if (m_Parent)
		return m_Parent->Resolve(varname);

	throw EnvironmentException{ "Cannot resolve variable '" + varname + "' as it does not exist" };
}

RuntimeVal Environment::DeclareVar(std::string varname, RuntimeVal value, bool constant)
{
	if (m_Variables.count(varname))
	{
		throw EnvironmentException{ "Cannot declare variable '" + varname + "' as it already is defined" };

	}

	m_Variables[varname] = value;
	if (constant)
		m_Constants.insert(varname);

	return value;
}

RuntimeVal Environment::AssignVar(std::string varname, RuntimeVal value)
{
	Environment* env = Resolve(varname);

	if (env->m_Constants.count(varname))
	{
		throw EnvironmentException{ "Cannot reassign to variable '" + varname + "' as it was declared as constant" };
	}

	env->m_Variables[varname] = value;
	return value;
}

RuntimeVal Environment::LookupVar(std::string varname)
{
	Environment* env = Resolve(varname);
	return env->m_Variables[varname];
}

Environment* Environment::CopyEnv(Environment* env)
{
	return env;
}

std::string Environment::AsJson()
{
	std::string variables;

	for (auto var : m_Variables)
	{
		variables += "{ \"key\": \"" + var.first + "\", \"value\": \"[object]\" }, ";
	}

	std::string constants;

	for (std::string con : m_Constants)
	{
		constants += "\"" + con + "\", ";
	}

	return "{ \"parent\": " + (m_Parent ? m_Parent->AsJson() : "\"undefined\"") + ", \"global\": \"" + (m_Global ? "true" : "false") + "\", \"variables\": [ " + variables + "], \"constants\": [ " + constants + "] }";
}

void Environment::SetupScope(Environment& env)
{
	env.DeclareVar("true", BoolVal(true), true);
	env.DeclareVar("false", BoolVal(false), true);
	env.DeclareVar("null", NullVal(), true);

	struct NativePrintFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			for (RuntimeVal val : args)
				std::cout << ValueAsString(val) << std::endl;

			return NullVal();
		}
	};

	struct NativeTimeFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			return NumberVal(CurrentTime());
		}
	};

	struct NativeRandFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			return NumberVal(Random(NumberVal(args[0]).Value(), NumberVal(args[1]).Value()));
		}
	};

	struct NativeRandIntFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			return NumberVal(std::round(Random(NumberVal(args[0]).Value(), NumberVal(args[1]).Value())));
		}
	};

	struct NativeInputFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			std::cout << ValueAsString(args[0]) << " ";

			std::string val;
			getline(std::cin, val);

			return StringVal(val);
		}
	};

	struct NativeIntFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			NumberVal val(0);
			StringToInt(ValueAsString(args[0]), val.Value());
			return val;
		}
	};

	struct NativeClearFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			ClearTerminal();

			return NullVal();
		}
	};

	struct NativeClearPrintFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			ClearTerminal();

			for (RuntimeVal val : args)
				std::cout << ValueAsString(val) << std::endl;

			return NullVal();
		}
	};

	struct NativeExitFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			throw ExitException{ EXIT_SUCCESS };
		}
	};

	struct NativeKeyFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			std::string key = StringVal(args[0]).Value();

			KeyState keyState = GetKey(key);

			std::map<std::string, RuntimeVal> properties;
			properties["down"] = BoolVal(keyState.down);
			properties["toggle"] = BoolVal(keyState.toggle);

			return ObjectVal(properties);
		}
	};

	struct NativeSleepFn : INativeFnCall
	{
		RuntimeVal Call(std::vector<RuntimeVal> args, Environment* env)
		{
			long x = long(NumberVal(args[0]).Value());

			std::this_thread::sleep_for(std::chrono::milliseconds(x));

			return NullVal();
		}
	};

	env.DeclareVar("sys", NativeEnvVal(
		{
			{ "print", NativeFnVal(new NativePrintFn()) },
			{ "cprint", NativeFnVal(new NativeClearPrintFn()) },
			{ "time", NativeFnVal(new NativeTimeFn()) },
			{ "rand", NativeFnVal(new NativeRandFn()) },
			{ "randint", NativeFnVal(new NativeRandIntFn()) },
			{ "input", NativeFnVal(new NativeInputFn()) },
			{ "key", NativeFnVal(new NativeKeyFn()) },
			{ "int", NativeFnVal(new NativeIntFn()) },
			{ "clear", NativeFnVal(new NativeClearFn()) },
			{ "sleep", NativeFnVal(new NativeSleepFn()) },
			{ "exit", NativeFnVal(new NativeExitFn()) },
		}
	), true);
}