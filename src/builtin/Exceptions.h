#pragma once

#include <string>

struct ExitException
{
	int code;
};

struct LexerException
{
	std::string message;
};

struct ParserException
{
	std::string message;
};

struct InterpreterException
{
	std::string message;
};

struct EnvironmentException
{
	std::string message;
};

struct RuntimeException
{
	std::string message;
};