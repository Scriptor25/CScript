#include <signal.h>

#include "frontend/Parser.h"
#include "runtime/Environment.h"
#include "runtime/Interpreter.h"

bool debug_output = false;

void signal_callback_handler(int signum)
{
	if (debug_output)
		std::cout << "Caught signal " << signum << std::endl;

	exit(signum);
}

static void FromFile(const char* path, Parser parser, Environment* env)
{
	try {
		AST::Program program = parser.ProduceAST(ReadFile(path), debug_output);

		if (debug_output)
			std::cout << FormatJson(program.AsJson()) << std::endl;

		RuntimeVal result = Interpreter::Evaluate(program, env);

		if (debug_output)
			std::cout << FormatJson(result.AsJson()) << std::endl;
	}
	catch (std::string e) {
		std::cerr << e << std::endl;
	}
	catch (ExitException e)
	{
		std::cout << "Press Enter To Exit";
		std::string input;
		std::getline(std::cin, input);

		exit(e.code);
	}
	catch (LexerException e)
	{
		std::cerr << "LexerException: \n" << e.message << std::endl;
	}
	catch (ParserException e)
	{
		std::cerr << "ParserException: \n" << e.message << std::endl;
	}
	catch (EnvironmentException e)
	{
		std::cerr << "EnvironmentException: \n" << e.message << std::endl;
	}
	catch (InterpreterException e)
	{
		std::cerr << "InterpreterException: \n" << e.message << std::endl;
	}
	catch (RuntimeException e)
	{
		std::cerr << "RuntimeException: \n" << e.message << std::endl;
	}
}

static void FromInput(Parser parser, Environment* env)
{
	std::cout << "CScript v1.0" << std::endl;
	while (true)
	{
		std::string input;

		std::cout << "> ";
		std::getline(std::cin, input);

		try {
			AST::Program program = parser.ProduceAST(input, debug_output);

			if (debug_output)
				std::cout << FormatJson(program.AsJson()) << std::endl;

			RuntimeVal result = Interpreter::Evaluate(program, env);

			if (debug_output)
				std::cout << FormatJson(result.AsJson()) << std::endl;
		}
		catch (std::string e) {
			std::cerr << e << std::endl;
		}
		catch (ExitException e)
		{
			std::cout << "Press Enter To Exit";
			std::string input;
			std::getline(std::cin, input);

			exit(e.code);
		}
		catch (LexerException e)
		{
			std::cerr << "LexerException: \n" << e.message << std::endl;
		}
		catch (ParserException e)
		{
			std::cerr << "ParserException: \n" << e.message << std::endl;
		}
		catch (EnvironmentException e)
		{
			std::cerr << "EnvironmentException: \n" << e.message << std::endl;
		}
		catch (InterpreterException e)
		{
			std::cerr << "InterpreterException: \n" << e.message << std::endl;
		}
		catch (RuntimeException e)
		{
			std::cerr << "RuntimeException: \n" << e.message << std::endl;
		}
	}
}

int main(int argc, char** argv)
{
	signal(SIGINT, signal_callback_handler);

	std::string debug = "";
	std::string path = "";

	if (argc > 1) path = argv[1];
	if (argc > 2) debug = argv[2];
	if (argc > 3)
	{
		std::cerr << "args are: <path: path to file> <debug: true|false>" << std::endl;
		return EXIT_FAILURE;
	}

	Parser parser;
	Environment env;

	debug_output = debug == "true";

	if (path != "")
		FromFile(path.c_str(), parser, &env);
	else
		FromInput(parser, &env);

	std::string input;
	std::cout << "Press Enter To Exit";
	std::getline(std::cin, input);

	return EXIT_SUCCESS;
}