#pragma once

#include <functional>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iomanip> // for the string formating functions like setfill 
#include <cstring>
#include <cmath>
 
namespace Integrity {
	static constexpr const char* defaultExceptionMessage = "Integrity check failed";
	static constexpr const char* defaultNullPointerMessage = "Null pointer";
	static constexpr const char* defaultEmptyStringMessage = "Empty string";

	enum class DispType;
	struct TypeValue;
	static std::string makeString(const std::vector<TypeValue>& items);
	static std::string makeString(std::function<void(std::stringstream&)> messageFunc);
	static void throwWithMessage(const std::vector<Integrity::TypeValue>& items);
	template<typename T> static std::string getFloatAppropriateMessage(T value);
	template<typename T> TypeValue toTypeValue(T primitive);
	template<typename N> std::string throwInvalidFloat(N value);

	typedef std::stringstream& out;

	// *********
	// * check *
	// *********

	template <typename B> void check(B condition);

	template<> inline void check<bool>(bool condition) {
		if (!condition) {
			throw std::logic_error(defaultExceptionMessage);
		}
	}

	template<typename T1> inline void check(bool condition, T1 messageArg1) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1, typename T2> inline void check(bool condition, T1 messageArg1, T2 messageArg2) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename T1, typename T2, typename T3> inline void check(bool condition, T1 messageArg1, T2 messageArg2, T3 messageArg3) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2), toTypeValue(messageArg3) });
		}
	}

	inline void checkM(bool condition, std::function<void(std::stringstream&)> messageFunc) {
		if (!condition) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// ********
	// * fail *
	// ********

	inline void fail() {
		throw std::logic_error(defaultExceptionMessage);
	}

	inline void failM(std::function<void(std::stringstream&)> messageFunc) {
		throw std::logic_error(makeString(messageFunc));
	}
	template<typename T1> inline void fail(T1 messageArg1) {
		throwWithMessage({ toTypeValue(messageArg1) });
	}
	template<typename T1, typename T2> inline void fail(T1 messageArg1, T2 messageArg2) {
		throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
	}

	// **********************
	// * checkIsValidNumber *
	// **********************

	template<typename N> inline void checkIsValidNumber(const N value) {
		if (std::isnan(value) || std::isinf(value)) {
			throwInvalidFloat(value);
		}
	}
	template<typename N, typename T1> inline void checkIsValidNumber(const N value, T1 messageArg1) {
		if (std::isnan(value) || std::isinf(value)) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename N, typename T1, typename T2> inline void checkIsValidNumber(const N value, T1 messageArg1, T2 messageArg2) {
		if (std::isnan(value) || std::isinf(value)) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename N> inline void checkIsValidNumberM(const N value, std::function<void(std::stringstream&)> messageFunc) {
		if (std::isnan(value) || std::isinf(value)) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// ****************
	// * checkNotNull *
	// ****************

	inline void checkNotNull(const void* pointer) {
		if (pointer == 0) {
			throw std::logic_error("Null pointer");
		}
	}
	template<typename T1> inline void checkNotNull(const void* pointer, T1 messageArg1) {
		if (pointer == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1, typename T2> inline void checkNotNull(const void* pointer, T1 messageArg1, T2 messageArg2) {
		if (pointer == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}

	inline void checkNotNullM(const void* pointer, std::function<void(std::stringstream&)> messageFunc) {
		if (pointer == 0) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// *****************************
	// * checkStringNotNullOrEmpty *
	// *****************************

	inline void checkStringNotNullOrEmpty(const char* s) {
		if (s == 0) {
			throw std::logic_error(defaultNullPointerMessage);
		}
		if (std::strlen(s) == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}
	inline void checkStringNotNullOrEmpty(std::string& s) {
		if (s.empty() == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}
	inline void checkStringNotNullOrEmpty(std::wstring& s) {
		if (s.empty() == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}
	inline void checkStringNotNullOrEmpty(std::u16string& s) {
		if (s.empty() == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}
	inline void checkStringNotNullOrEmpty(std::u32string& s) {
		if (s.empty() == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}

	template<typename T1> inline void checkStringNotNullOrEmpty(const char* s, T1 messageArg1) {
		if (s == 0 || std::strlen(s) == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1> inline void checkStringNotNullOrEmpty(std::string& s, T1 messageArg1) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1> inline void checkStringNotNullOrEmpty(std::wstring& s, T1 messageArg1) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1> inline void checkStringNotNullOrEmpty(std::u16string& s, T1 messageArg1) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename T1> inline void checkStringNotNullOrEmpty(std::u32string& s, T1 messageArg1) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}

	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(const char* s, T1 messageArg1, T2 messageArg2) {
		if (s == 0 || std::strlen(s) == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(std::string& s, T1 messageArg1, T2 messageArg2) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(std::wstring& s, T1 messageArg1, T2 messageArg2) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(std::u16string& s, T1 messageArg1, T2 messageArg2) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(std::u32string& s, T1 messageArg1, T2 messageArg2) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	inline void checkStringNotNullOrEmptyM(const char* s, std::function<void(std::stringstream&)> messageFunc) {
		if (s == 0 || std::strlen(s) == 0) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// "private" functions... -----------------------------------------------------------------------------------------------

	enum class DispType {
		isBool,
		isString, // std::string, std::wstring, std::u16string, std::u32string
		isCharStar,
		isNumber, // short, int, long, long long & unsigned variants, and float, double and long double
		isChar,
	};
	struct TypeValue {
		DispType type;
		std::string value;
		TypeValue(DispType theType, std::string theValue) {
			type = theType;
			value = theValue;
		}
		std::string representationImplyingType() {
			switch (this->type) {
			case DispType::isBool:
				return this->value; // will be True or False
			case DispType::isChar:
				return "'" + this->value + "'"; // use ' to imply char
			case DispType::isString:
				return "\"" + this->value + "\""; // use ' to imply string
			case DispType::isCharStar:
				return "`" + this->value + "`"; // use ` (backtick) to imply char*
			case DispType::isNumber:
				return this->value; // ints, longs, floats etc. are all just represented as their value 
			default:
				return "Invalid DispType";
			}
		}
	};

	static std::string makeString(const std::vector<TypeValue>& items) {
		std::string retString = "";

		for (TypeValue item : items) {
			std::size_t braces = retString.find("{}");
			if (braces != std::string::npos) {
				retString = retString.replace(braces, 2, item.value);
			}
			else {
				std::string asString;
				// if the first message argument was a char* then don't stick any quotes around it...
				if (retString.length() == 0 && item.type == Integrity::DispType::isCharStar) {
					asString = item.value;
				}
				else {
					asString = item.representationImplyingType();
				}
				if (retString.length() == 0) {
					retString = asString;
				}
				else {
					retString = retString + ", " + asString;
				}
			}
		}
		return retString;
	}
	static std::string makeString(std::function<void(std::stringstream&)> messageFunc) {
		if (messageFunc == 0) {
			return defaultExceptionMessage;
		}

		try {
			std::stringstream ss;

			messageFunc(ss);

			return ss.str();
		}
		catch (...) {
			return std::string(defaultExceptionMessage) + ": [error]";
		}
	}

	template<typename T> TypeValue toTypeValue(T primitive) {
		return TypeValue(DispType::isNumber, std::to_string(primitive));
	}
	template<> TypeValue toTypeValue<bool>(bool value) {
		return TypeValue(DispType::isBool, value ? "True" : "False");
	}
	template<> TypeValue toTypeValue<char>(char value) {
		return TypeValue(DispType::isChar, std::string(1, value));
	}
	template<> TypeValue toTypeValue<unsigned char>(unsigned char value) {
		return TypeValue(DispType::isChar, std::string(1, value));
	}
	template<> TypeValue toTypeValue<char16_t>(char16_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(char16_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	template<> TypeValue toTypeValue<char32_t>(char32_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(char32_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	template<> TypeValue toTypeValue<wchar_t>(wchar_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(wchar_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	template<> TypeValue toTypeValue<std::string>(std::string value) {
		return TypeValue(DispType::isString, value);
	}
	// string conversions...
	/*
	* see https://dbj.org/c17-codecvt-deprecated-panic/
	*/
	template<typename F> inline std::string toStdString(F str) noexcept
	{
		if (str.empty())
			return {};
		return { std::begin(str), std::end(str) };
	};

	template<> TypeValue toTypeValue<std::wstring>(std::wstring value) {
		return TypeValue(DispType::isString, toStdString(value));
	}
	template<> TypeValue toTypeValue<std::u16string>(std::u16string value) {
		return TypeValue(DispType::isString, toStdString(value));
	}
	template<> TypeValue toTypeValue<std::u32string>(std::u32string value) {
		return TypeValue(DispType::isString, toStdString(value));
	}
	template<> TypeValue toTypeValue<const char*>(const char* str) {
		return TypeValue(DispType::isCharStar, std::string(str));
	}
	/*
	* It is tempting to have a specialisation for T* (i.e. any pointer) which prints out the
	* address of the pointer, but actually having the memory address of a pointer is not very
	* helpful, and it is more likely that the developer intended to print the value of the
	* item being pointed to. (So for example if the have an int* they probably want to print the
	* value of the int rather than the memory address). By not having a T* specialisation then the
	* developer will get a compile error if they don't dereference the pointer (which is probably
	* better)
	*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
	static void throwWithMessage(const std::vector<Integrity::TypeValue>& items) {
		std::string exceptionMessage = makeString(items);
		throw std::logic_error(exceptionMessage);
	}
#pragma GCC diagnostic pop

	template<typename T> static std::string getFloatAppropriateMessage(T value) {
		if (std::isnan(value)) {
			return "NaN";
		}
		if (std::isinf(value)) {
			if (value > 0) {
				return "+Infinity";
			}
			else {
				return "-Infinity";
			}
		}
		return "Error: expected invalid float";
	}

	template<> std::string throwInvalidFloat<float>(float value)
	{
		throw std::logic_error(getFloatAppropriateMessage(value));
	}
	template<> std::string throwInvalidFloat<double>(double value) {
		throw std::logic_error(getFloatAppropriateMessage(value));
	}
	template<> std::string throwInvalidFloat<long double>(long double value) {
		throw std::logic_error(getFloatAppropriateMessage(value));
	}
}

