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

	/// <summary>
	/// Placeholder to prevent non-bool being passed in as first parameter.
	/// </summary>
	/// <param name="youNeedABool">Did you accidentally do = instead of ==?</param>
	/// <remarks>If this is not here then common errors like check(a = b) instead of check(a == b) do not get caught by compiler</remarks>
	template <typename B> void check(B youNeedABool);
	
	/// <summary>
	/// Placeholder to prevent non-bool being passed in as first parameter.
	/// </summary>
	/// <param name="youNeedABool">Did you accidentally do = instead of ==?</param>
	/// <remarks>If this is not here then common errors like check(a = b) instead of check(a == b) do not get caught by compiler</remarks>
	template <typename B, typename T1> void check(B youNeedABool, const T1&);
	
	/// <summary>
	/// Placeholder to prevent non-bool being passed in as first parameter.
	/// </summary>
	/// <param name="youNeedABool">Did you accidentally do = instead of ==?</param>
	/// <remarks>If this is not here then common errors like check(a = b) instead of check(a == b) do not get caught by compiler</remarks>
	template <typename B, typename T1, typename T2> void check(B youNeedABool, T1, T2);
	
	/// <summary>
	/// Placeholder to prevent non-bool being passed in as first parameter.
	/// </summary>
	/// <param name="youNeedABool">Did you accidentally do = instead of ==?</param>
	/// <remarks>If this is not here then common errors like check(a = b) instead of check(a == b) do not get caught by compiler</remarks>
	template <typename B, typename T1, typename T2, typename T3> void check(B youNeedABool, T1, T2, T3);

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error with default message
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	template<> inline void check<bool>(bool condition) {
		if (!condition) {
			throw std::logic_error(defaultExceptionMessage);
		}
	}

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="T1">String or primitive to build exception message from</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	template<typename T1> inline void check(bool condition, const T1 messageArg1) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="T1">String or primitive to build exception message from, use {} for optional substitution of T2</param>
	/// <param name="T2">String or primitive to build exception message from</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	template<typename T1, typename T2> inline void check(bool condition, T1 messageArg1, T2 messageArg2) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	
	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="T1">String or primitive to build exception message from, use {} for optional substitution of T2 and T3</param>
	/// <param name="T2">String or primitive to build exception message from</param>
	/// <param name="T3">String or primitive to build exception message from</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	template<typename T1, typename T2, typename T3> inline void check(bool condition, T1 messageArg1, T2 messageArg2, T3 messageArg3) {
		if (!condition) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2), toTypeValue(messageArg3) });
		}
	}

	template<typename B> inline void checkM(B condition, const std::function<void(std::stringstream&)> messageFunc);

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error where you can pass a lambda function to build the message
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks> 
	template<> inline void checkM<bool>(bool condition, const std::function<void(std::stringstream&)> messageFunc) {
		if (!condition) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// ******************************************************************************************************************
	// * -------------------------------------------------- fail ------------------------------------------------------ *
	// ******************************************************************************************************************
			
	/// <summary>
	/// Raises a logic_error with a default message
	/// </summary>
	/// <exception cref="logic_error"></exception>
	inline void fail() {
		throw std::logic_error(defaultExceptionMessage);
	}

	/// <summary>
	/// Raises a logic_error where you can pass a lambda function to build the message
	/// </summary>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks>
	inline void failM(std::function<void(std::stringstream&)> messageFunc) {
		throw std::logic_error(makeString(messageFunc));
	}

	/// <summary>
	/// Raises a logic_error
	/// </summary>
	/// <param name="T1">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename T1> inline void fail(T1 messageArg1) {
		throwWithMessage({ toTypeValue(messageArg1) });
	}

	/// <summary>
	/// Raises a logic_error
	/// </summary>
	/// <param name="T1">String or primitive to build exception message from, use {} for optional substitution of T2</param>
	/// <param name="T2">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename T1, typename T2> inline void fail(T1 messageArg1, T2 messageArg2) {
		throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
	}

	// ******************************************************************************************************************
	// * -------------------------------------- checkIsValidNumber ---------------------------------------------------- *
	// ******************************************************************************************************************

	/// <summary>
	/// Raises a logic_error if the number is NaN or +-Infinity
	/// </summary>
	/// <param name="value">float, double or long double</param>
	/// <exception cref="logic_error">Message will be 'Nan' or '+Infinity' or '-Infinity'</exception>
	template<typename N> inline void checkIsValidNumber(const N value) {
		if (std::isnan(value) || std::isinf(value)) {
			throwInvalidFloat(value);
		}
	}

	/// <summary>
	/// Raises a logic_error if the number is NaN or +-Infinity
	/// </summary>
	/// <param name="value">float, double or long double</param>
	/// <param name="T1">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename N, typename T1> inline void checkIsValidNumber(const N value, T1 messageArg1) {
		if (std::isnan(value) || std::isinf(value)) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}

	/// <summary>
	/// Raises a logic_error if the number is NaN or +-Infinity
	/// </summary>
	/// <param name="value">float, double or long double</param>
	/// <param name="T1">String or primitive to build exception message from, use {} for optional substitution of T2</param>
	/// <param name="T2">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename N, typename T1, typename T2> inline void checkIsValidNumber(const N value, T1 messageArg1, T2 messageArg2) {
		if (std::isnan(value) || std::isinf(value)) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}

	/// <summary>
	/// Raises a logic_error if the number is NaN or +-Infinity
	/// </summary>
	/// <param name="value">float, double or long double</param>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error"></exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks>
	template<typename N> inline void checkIsValidNumberM(const N value, std::function<void(std::stringstream&)> messageFunc) {
		if (std::isnan(value) || std::isinf(value)) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// ******************************************************************************************************************
	// * ---------------------------------------------- checkNotNull -------------------------------------------------- *
	// ******************************************************************************************************************

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <exception cref="logic_error">Null pointer</exception>
	inline void checkNotNull(const void* pointer) {
		if (pointer == 0) {
			throw std::logic_error("Null pointer");
		}
	}

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <param name="T1">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename T1> inline void checkNotNull(const void* pointer, T1 messageArg1) {
		if (pointer == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <param name="T1">String or primitive to build exception message from, use {} for optional substitution of T2</param>
	/// <param name="T2">String or primitive to build exception message from</param>
	/// <exception cref="logic_error"></exception>
	template<typename T1, typename T2> inline void checkNotNull(const void* pointer, T1 messageArg1, T2 messageArg2) {
		if (pointer == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error"></exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks>
	inline void checkNotNullM(const void* pointer, std::function<void(std::stringstream&)> messageFunc) {
		if (pointer == 0) {
			throw std::logic_error(makeString(messageFunc));
		}
	}

	// ******************************************************************************************************************
	// * ---------------------------------------- checkStringNotNullOrEmpty ------------------------------------------- *
	// ******************************************************************************************************************


	template<typename S> inline void checkStringNotNullOrEmpty(const S& s) {
		if (s.empty() == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}

	inline void checkStringNotNullOrEmpty(const char* s) {
		if (s == 0) {
			throw std::logic_error(defaultNullPointerMessage);
		}
		if (std::strlen(s) == 0) {
			throw std::logic_error(defaultEmptyStringMessage);
		}
	}

	template<typename T1> inline void checkStringNotNullOrEmpty(const char* s, T1 messageArg1) {
		if (s == 0 || std::strlen(s) == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}
	template<typename S,typename T1> inline void checkStringNotNullOrEmpty(const S& s, T1 messageArg1) {
		if (s.empty() == 0) {
			throwWithMessage({ toTypeValue(messageArg1) });
		}
	}

	template<typename T1, typename T2> inline void checkStringNotNullOrEmpty(const char* s, T1 messageArg1, T2 messageArg2) {
		if (s == 0 || std::strlen(s) == 0) {
			throwWithMessage({ toTypeValue(messageArg1), toTypeValue(messageArg2) });
		}
	}
	template<typename S, typename T1, typename T2> inline void checkStringNotNullOrEmpty(const S& s, T1 messageArg1, T2 messageArg2) {
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
		TypeValue(DispType theType, const std::string& theValue) {
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
	template<> TypeValue toTypeValue<bool>(const bool value) {
		return TypeValue(DispType::isBool, value ? "True" : "False");
	}
	template<> TypeValue toTypeValue<char>(const char value) {
		return TypeValue(DispType::isChar, std::string(1, value));
	}
	template<> TypeValue toTypeValue<unsigned char>(const unsigned char value) {
		return TypeValue(DispType::isChar, std::string(1, value));
	}
	template<> TypeValue toTypeValue<char16_t>(const char16_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(char16_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	template<> TypeValue toTypeValue<char32_t>(const char32_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(char32_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	template<> TypeValue toTypeValue<wchar_t>(const wchar_t value) {
		std::stringstream ss;
		if (value > 0xff) {
			ss << "0x" << std::hex << std::setfill('0') << std::setw(2 * sizeof(wchar_t)) << std::uppercase << (long) value;
		}
		else {
			ss << (char) value;
		}
		return TypeValue(DispType::isChar, ss.str());
	}
	TypeValue toTypeValue(const std::string& value) {
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

