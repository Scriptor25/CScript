#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <Windows.h>
#include <CkJsonObject.h>

static const std::string ReadFile(const char* path)
{
	std::string out;

	std::string line;
	std::ifstream file(path);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			// std::cout << line << std::endl;
			out.append(line + '\n');
		}
		file.close();
	}
	else
	{
		std::cerr << "Unable to open file '" << path << "'" << std::endl;
	}

	return out;
}

static std::string FormatJson(std::string str, bool compact = false, bool crlf = true)
{
	CkJsonObject json;
	bool success = json.Load(str.c_str());
	if (!success)
		return json.lastErrorText();
	else
	{
		json.put_EmitCompact(compact);
		json.put_EmitCrLf(crlf);

		return json.emit();
	}
}

static int StringToInt(const std::string& str, double& p_value, std::size_t* pos = 0, int base = 10) {
	// wrapping std::stoi because it may throw an exception

	try {
		p_value = std::stoi(str, pos, base);
		return 0;
	}

	catch (const std::invalid_argument& ia) {
		std::cerr << "Invalid argument: " << ia.what() << std::endl;
		return -1;
	}

	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << std::endl;
		return -2;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Undefined error: " << e.what() << std::endl;
		return -3;
	}
}

struct SuperObject {};

template<class T>
struct BaseObject : SuperObject
{
	T _value;
	BaseObject(T value) : _value(value) {}
};

class ValueObject
{
private:
	std::unordered_map<std::string, std::shared_ptr<SuperObject>> m_ObjectMap;

public:
	template<typename T>
	void SetValue(std::string name, T value)
	{
		m_ObjectMap[name] = std::make_shared<BaseObject<T>>(value);
	}

	template <typename T>
	T GetValue(const std::string& name)
	{
		std::shared_ptr<BaseObject<T>> obj = (std::static_pointer_cast<BaseObject<T>>)(m_ObjectMap[name]);
		return obj->_value;
	}

	template <typename T>
	T GetValue(const std::string& name, T defaultValue)
	{
		std::shared_ptr<BaseObject<T>> obj = (std::static_pointer_cast<BaseObject<T>>)(m_ObjectMap[name]);
		if (!obj) return defaultValue;
		return obj->_value;
	}
};