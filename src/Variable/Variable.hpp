#ifndef __VARIABLE_HPP
#define __VARIABLE_HPP

#include <string>

class Variable
{
   public:
	enum class type
	{
		UNKNOWN = 0,
		U8 = 1,
		I8 = 2,
		U16 = 3,
		I16 = 4,
		U32 = 5,
		I32 = 6,
		F32 = 7

	};

	Variable(std::string name) : name(name)
	{
	}
	template <typename T>
	Variable(std::string name, type type_, T value_) : name(name), varType(type_)
	{
		setValue<T>(value_);
	}
	~Variable() = default;

	void setType(type varType_)
	{
		varType = varType_;
	}

	type getType()
	{
		return varType;
	}

	std::string getTypeStr()
	{
		return std::string(types[static_cast<uint8_t>(varType)]);
	}

	template <typename T>
	void setValue(T val)
	{
		value = *(uint32_t*)&val;
	}

	template <typename T>
	T getValue()
	{
		return *(T*)&value;
	}

	void setAddress(uint32_t addr)
	{
		address = addr;
	}
	uint32_t getAddress()
	{
		return address;
	}

	std::string& getName()
	{
		return name;
	}

	void setName(std::string name_)
	{
		name = name_;
	}

   private:
	std::string name;
	type varType;
	uint32_t value;
	uint32_t address;
	const char* types[8] = {"UNKNOWN",
							"U8",
							"I8",
							"U16",
							"I16",
							"U32",
							"I32",
							"F32"};
};

#endif