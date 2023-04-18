#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <thread>

#include "../builtin/Exceptions.h"
#include "../builtin/Random.h"
#include "../builtin/Terminal.h"
#include "../builtin/Time.h"

#include "Values.h"

struct RuntimeVal;

class Environment
{
public:
	Environment(Environment* parent = nullptr)
		: m_Parent(parent), m_Global(!parent)
	{
		if (m_Global) SetupScope(*this);
	}

	Environment* Resolve(std::string varname);
	RuntimeVal DeclareVar(std::string varname, RuntimeVal value, bool constant);
	RuntimeVal AssignVar(std::string varname, RuntimeVal value);
	RuntimeVal LookupVar(std::string varname);
	Environment* CopyEnv(Environment* env);

	std::string AsJson();

	static void SetupScope(Environment& env);

private:
	Environment* m_Parent;
	bool m_Global;
	std::map<std::string, RuntimeVal> m_Variables;
	std::set<std::string> m_Constants;
};