#include <iostream>
#include <sstream>
#include "integrity.h"

using namespace std;
//using namespace Integrity;

void fail(const char* message) {
    std::cout << "FAIL\n";
    if (message != 0) {
        std::cout << message << endl;
    }
}

void tests_which_should_not_throw() {
    cout << "Tests which should not throw an exception...\n";

    float f1 = 1.1f;
    double d1 = 1.1;
    long double d2 = 1.2;

    try {

        Integrity::check(true, 1);
        Integrity::check('a' == 'a', 1.1);
        Integrity::check(1.0 == 1.0, "hihi");
        Integrity::check(1 == 1, 1, "hihi");
        Integrity::checkM(7 == 7, [](stringstream& ss) { ss << "hi"; });

        Integrity::check(true, "hihi");
        Integrity::check(true);
        Integrity::check(true, 1);
        Integrity::checkM(true, [](stringstream& ss) { ss << "hi"; });


        Integrity::checkIsValidNumber(f1);
        Integrity::checkIsValidNumber(d1);
        Integrity::checkIsValidNumber(d2);
        Integrity::checkIsValidNumber(d2, d2);
        Integrity::checkIsValidNumber(d2, "d2 is {}", d2);
        Integrity::checkIsValidNumberM(d2, [=](Integrity::out out) { out << "d2 is " << d2; });

        Integrity::checkNotNull(&f1);

        Integrity::checkNotNull(&f1, "message");
        Integrity::checkNotNull(&f1, "message", d1);
        Integrity::checkNotNullM(&f1, [=](Integrity::out out) { out << "message" << d1 << d2; });

        cout << "... passed" << endl;
    }
    catch (exception& e) {
        cout << "test failed" << endl;
        fail(e.what());
        cout << "... finished tests which should not throw exceptions" << endl;
    }
}

void tests_which_should_not_compile() {
    const char* aCharStarPointer = 0; // prevented by private const void*
    int anInt = -10; // prevented by private long long option
    unsigned int anUnsignedInt = 10; // prevented by private long long option
    long aLong = -200; // prevented by private long long option
    unsigned long anUnsignedLong = 200; // prevented by private long long option
    char aChar = 'c'; // prevented by private long long option
    long double aLongDouble = 1.1; // prevented by private long long option

    //Integrity::check(aCharStarPointer);
    //Integrity::check(anInt);
    //Integrity::check(anUnsignedInt);
    //Integrity::check(aLong);
    //Integrity::check(anUnsignedLong);
    //Integrity::check(aChar);
    //Integrity::check(aLongDouble);

    //Integrity::check(aCharStarPointer);

    //Integrity::check(long1);

    //Integrity::checkM(22, [](std::stringstream& ss) {});
    //Integrity::checkIsValidNumber(1);
    //Integrity::checkIsValidNumber("a");
    //Integrity::checkIsValidNumber('a');
}

void expect_throw(function<void()> func, const char* expectMessage) {
    try {
        func();
        cout << "expected  '" << expectMessage << "'" << endl;
        fail("test failed, no exception thrown");
    }
    catch (exception& e) {
        string got = e.what();
        if (got != expectMessage) {
            cout << "expected  '" << expectMessage << "'" << endl;
            cout << "got '" << got << "'" << endl;
            fail("test failed");
        }
    }
}

void tests_which_should_throw() {
    cout << "Tests which SHOULD throw an exception...\n";
    int x = 1;
    int y = 2;
    float zero = 0;
    float f1 = 1.1f / zero;
    const char* aPointer = "omg";
    int* intP = &x;

    expect_throw([=]() {
        bool b = x == x;
        Integrity::check(false, b);
        }, "True");

    expect_throw([=]() {
        Integrity::check(false, *intP);
        }, "1");

    expect_throw([=]() {
        bool b = 22; // will convert to 1
        Integrity::check(false, (int) b);
        }, "1");

    expect_throw([=]() {
        Integrity::checkM(x == y, [=](stringstream& ss) { ss << x << " != " << y; });
        }, "1 != 2");

    expect_throw([=]() {
        Integrity::check(x == y);
        }, "Integrity check failed");

    expect_throw([=]() {
        Integrity::check(x == y, "Oopsies");
        }, "Oopsies");

    expect_throw([=]() {
        Integrity::check(x == y, "Expected {} to be same as {}", x, y);
        }, "Expected 1 to be same as 2");

    expect_throw([=]() {
        Integrity::checkIsValidNumber(f1);
        }, "+Infinity");

    float* nullFP = 0;

    expect_throw([=]() {
        Integrity::checkNotNull(nullFP);
        }, "Null pointer");

    unsigned char unsignedChar = 'c';
    expect_throw([=]() {
        Integrity::fail(unsignedChar);
        }, "'c'");

    char16_t c16 = 0x0BCD;
    expect_throw([=]() {
        Integrity::fail(c16);
        }, "'0x0BCD'");

    char32_t c32 = 0xFEDCBA;
    expect_throw([=]() {
        Integrity::fail(c32);
        }, "'0x00FEDCBA'");

    wchar_t wc = 0xFCD;
    expect_throw([=]() {
        Integrity::fail(wc);
        }, "'0x0FCD'");
    
    //char8_t char8 = 'a'; // does not compile

    wstring awstring = L"a w string";
    expect_throw([=]() {
        Integrity::fail(awstring);
        }, "\"a w string\"");
    
    u16string au16string = u"a u16 string";
    expect_throw([=]() {
        Integrity::fail(au16string);
        }, "\"a u16 string\"");

    u32string au32string = U"a u32 string";
    expect_throw([=]() {
        Integrity::fail(au32string);
        }, "\"a u32 string\"");

    expect_throw([=]() {
        Integrity::fail(__func__);
        }, "operator ()");

    cout << "...Tests which SHOULD throw an exception finished\n";

}



int main()
{
#ifdef _MSC_VER
    // https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
    cout << "Microsoft compiler " << _MSC_VER << endl;
    cout << "C++ target version " << _MSVC_LANG << endl;
#endif
    
#ifdef __clang__
    cout << "clang++ " << __VERSION__ << endl;
#endif

#ifdef __GNUC__
    cout << "g++ " << __VERSION__ << endl;
#endif

    tests_which_should_not_throw();
    tests_which_should_throw();
}
