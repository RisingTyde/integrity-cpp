#pragma once

#include <functional>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iomanip> // for the string formating functions like setfill 
#include <cstring>
#include <cmath>
 

/*
* Notes
* Compiler does not allow default arguments on function templates
* Has to be in a namespace not in a class otherwise g++ does not compile it
*/
namespace Integrity {

	static const char* VERSION = "1.0.0";

	static constexpr const char* defaultExceptionMessage = "Integrity check failed";
	static constexpr const char* defaultNullPointerMessage = "Null pointer";
	static constexpr const char* defaultEmptyStringMessage = "Empty string";

	enum class DispType;
	struct TypeValue;
	static std::string makeString(const char* defaultMessage, const std::vector<TypeValue>& items);
	static std::string makeString(const std::function<void(std::stringstream&)>& messageFunc);
	static void throwWithMessage(const char* defaultMessage, const std::vector<Integrity::TypeValue>& items);
	template<typename T> static const char* getFloatAppropriateMessage(T value);
	template<typename T> TypeValue toTypeValue(T primitive);

	typedef std::stringstream& out;

	class NonType {
	};

	NonType nt;

	// ******************************************************************************************************************
	// * -------------------------------------------------- check------------------------------------------------------ *
	// ******************************************************************************************************************

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error with default message
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	inline void check(bool condition) {
		if (!condition) {
			throw std::logic_error(defaultExceptionMessage);
		}
	}

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error with given message
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	inline void check(bool condition, const char* message) {
		if (!condition) {
			throw std::logic_error(message);
		}
	}

	/// <summary>
	/// Placeholder to prevent non-bool being passed in as first parameter.
	/// </summary>
	/// <param name="youNeedABool">Did you accidentally do = instead of ==?</param>
	/// <remarks>If this is not here then common errors like check(a = b) instead of check(a == b) do not get caught by compiler</remarks>
	template<typename NONBOOL, typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	void check(NONBOOL youNeedABoolHere, M1 m1 = nt, M2 m2 = nt, M3 m3 = nt, M4 m4 = nt);

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="M1">Optional string or primitive</param>
	/// <param name="M2">Optional string or primitive</param>
	/// <param name="M3">Optional string or primitive</param>
	/// <param name="M4">Optional string or primitive</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	template<typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	void check(bool condition, M1 m1 = nt, M2 m2 = nt, M3 m3 = nt, M4 m4 = nt) {
		if (!condition) {
			throwWithMessage(defaultExceptionMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
	}

	template<typename B> inline void checkM(B condition, const std::function<void(std::stringstream&)>& messageFunc)=delete;

	/// <summary>
	/// Checks whether a condition is true, if not raises a logic_error where you can pass a lambda function to build the message
	/// </summary>
	/// <param name="condition">The condition to check is true.</param>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks> 
	template<> inline void checkM<bool>(bool condition, const std::function<void(std::stringstream&)>& messageFunc) {
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

	inline void fail(const char* message) {
		throw std::logic_error(message);
	}

	/// <summary>
	/// Raises a logic_error where you can pass a lambda function to build the message
	/// </summary>
	/// <param name="messageFunc">Example: [=](Integrity::out out) { out &lt;&lt; whateverYouWant; }</param>
	/// <exception cref="logic_error">Raised if condition is false</exception>
	/// <remarks>
	/// This function exists so that you can control the deferred message building by passing in a lambda function which is called if the condition fails.
	/// </remarks>
	inline void failM(const std::function<void(std::stringstream&)>& messageFunc) {
		throw std::logic_error(makeString(messageFunc));
	}

	/// <summary>
	/// Raises a logic_error
	/// </summary>
	/// <param name="M1">Optional string or primitive</param>
	/// <param name="M2">Optional string or primitive</param>
	/// <param name="M3">Optional string or primitive</param>
	/// <param name="M4">Optional string or primitive</param>
	/// <exception cref="logic_error"></exception>
	template<typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	void fail(M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		throwWithMessage(defaultExceptionMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
	}

	// ******************************************************************************************************************
	// * -------------------------------------- checkIsValidNumber ---------------------------------------------------- *
	// ******************************************************************************************************************


	template<typename N>
	inline void checkIsValidNumber(const N value, const char* message) {
		if (std::isnan(value) || std::isinf(value)) {
			throw std::logic_error(message);
		}
	}

	/// <summary>
	/// Raises a logic_error if the number is NaN or +-Infinity
	/// </summary>
	/// <param name="value">float, double or long double</param>
	/// <param name="M1">Optional string or primitive</param>
	/// <param name="M2">Optional string or primitive</param>
	/// <param name="M3">Optional string or primitive</param>
	/// <param name="M4">Optional string or primitive</param>
	/// <exception cref="logic_error"></exception>
	template<typename N, typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkIsValidNumber(const N value, M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		if (std::isnan(value) || std::isinf(value)) {
			const char* defaultMessage = getFloatAppropriateMessage(value);
			throwWithMessage(defaultMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
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
	template<typename N> 
	inline void checkIsValidNumberM(const N value, const std::function<void(std::stringstream&)>& messageFunc) {
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
	/// <exception cref="logic_error">message will be 'Null pointer'</exception>
	inline void checkNotNull(const void* pointer) {
		if (pointer == 0) {
			throw std::logic_error("Null pointer");
		}
	}

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <param name="message">message to use in the exception</param>
	/// <exception cref="logic_error"></exception>
	inline void checkNotNull(const void* pointer, const char* message) {
		if (pointer == 0) {
			throw std::logic_error(message);
		}
	}

	/// <summary>
	/// Raises a logic_error if the pointer is null (i.e. 0)
	/// </summary>
	/// <param name="pointer">a pointer to check for nullness</param>
	/// <param name="M1">Optional string or primitive</param>
	/// <param name="M2">Optional string or primitive</param>
	/// <param name="M3">Optional string or primitive</param>
	/// <param name="M4">Optional string or primitive</param>
	/// <exception cref="logic_error"></exception>
	template<typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkNotNull(const void* pointer, M1 & m1 = nt, M2 & m2 = nt, M3 & m3 = nt, M4 & m4 = nt) {
		if (pointer == 0) {
			throwWithMessage(defaultNullPointerMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
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

	/// <summary>
	/// Checks whether a string is null or zero length.
	/// </summary>
	/// <param name="s">The (wstring, std::string, u16string, u32string) to check for zero length.</param>
	/// <exception cref="logic_error">Raised if s is empty</exception>
	/// <remarks>
	/// Note that an exception is only raised if the string has exactly zero length; a string with a single space (for example) would be fine
	/// </remarks>
	template<typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkStringNotNullOrEmpty(const char* s, M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		if (s == 0) {
			throwWithMessage(defaultNullPointerMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		} else if(std::strlen(s) == 0) {
			throwWithMessage(defaultEmptyStringMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
	}
	template<typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkStringNotNullOrEmpty(char* s, M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		if (s == 0) {
			throwWithMessage(defaultNullPointerMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
		else if (std::strlen(s) == 0) {
			throwWithMessage(defaultEmptyStringMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
	}

	template<typename S, typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkStringNotNullOrEmpty(const S& s, M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		// If you get a compiler error like: left of .empty must have class/struct/union
		// in the line below, then you have not passed a string as firt param to checkStringNotNullOrEmpty 
		if (s.empty()) {
			throwWithMessage(defaultEmptyStringMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
	}

	template<typename S, typename M1 = NonType, typename M2 = NonType, typename M3 = NonType, typename M4 = NonType>
	inline void checkStringNotNullOrEmpty(S* s, M1& m1 = nt, M2& m2 = nt, M3& m3 = nt, M4& m4 = nt) {
		if (s == 0) {
			throwWithMessage(defaultNullPointerMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		} else if (s->empty()) {
			throwWithMessage(defaultEmptyStringMessage, { toTypeValue(m1), toTypeValue(m2), toTypeValue(m3), toTypeValue(m4) });
		}
	}

	inline void checkStringNotNullOrEmptyM(const char* s, const std::function<void(std::stringstream&)>& messageFunc) {
		if (s == 0 || std::strlen(s) == 0) {
			throw std::logic_error(makeString(messageFunc));
		}
	}
	template <typename S>
	inline void checkStringNotNullOrEmptyM(const S& s, const std::function<void(std::stringstream&)>& messageFunc) {
		if (s.empty()) {
			throw std::logic_error(makeString(messageFunc));
		}
	}
	template <typename S>
	inline void checkStringNotNullOrEmptyM(const S* s, const std::function<void(std::stringstream&)>& messageFunc) {
		if (s == 0 || s->empty()) {
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
		nonType, // placeholder for when no message argument was passed in
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

	static std::string makeString(const char* defaultMessage, const std::vector<TypeValue>& items) {
		std::string retString = "";

		bool atLeastOne = false;

		for (TypeValue item : items) {
			if (item.type == DispType::nonType) {
				continue;
			}
			atLeastOne = true;
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
		if (!atLeastOne) {
			return defaultMessage;
		}
		return retString;
	}
		if (messageFunc == 0) {
	static std::string makeString(const std::function<void(std::stringstream&)>& messageFunc) {
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
	template<> TypeValue toTypeValue<NonType>(NonType value) {
		return TypeValue(DispType::nonType, "");
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
	template<typename F> inline std::string toStdString(F str) {
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
	static void throwWithMessage(const char* defaultMessage, const std::vector<Integrity::TypeValue>& items) {
		std::string exceptionMessage = makeString(defaultMessage, items);
		throw std::logic_error(exceptionMessage);
	}
#pragma GCC diagnostic pop

	template<typename T> static const char* getFloatAppropriateMessage(T value) {
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
}

