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

class AnExampleClass {
public:
    int* x;
    int* y;
    int* z; 
    AnExampleClass(int* a, int* b, int* c);
    bool isOrth();
    std::string* getName();
};

AnExampleClass::AnExampleClass(int* a, int*b, int* c): x(a), y(b), z(c) {
}
bool AnExampleClass::isOrth() {
    return true;
}

string* AnExampleClass::getName() {
    return 0;
}

int exampleFunc(AnExampleClass a) {

    int i = 10 / *a.x;
    //throw logic_error("oops");
    a.getName()->find('c');

    if (a.isOrth()) {
        return (*a.x) * (*a.y);
    }
    else {
        return (*a.x) * (*a.z);
    }

    return a.isOrth() ? (*a.x) * (*a.y) : (*a.x) * (*a.z);
}

void tests_which_should_not_throw() {
    cout << "Tests which should not throw an exception...\n";

    float f1 = 1.1f;
    double d1 = 1.1;
    long double d2 = 1.2;
    const char* aCharStar = "abc";
    int aa = 0;
    AnExampleClass anExampleClass(&aa, &aa, &aa);

    try {

        //exampleFunc(anExampleClass);

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
        Integrity::checkNotNullM(aCharStar, [=](Integrity::out out) { out << "message" << d1 << d2; });
        Integrity::checkNotNull(aCharStar);
        Integrity::checkNotNull(aCharStar, "message");
        Integrity::checkNotNull(aCharStar, "m1", "m2");
        Integrity::checkNotNull(&anExampleClass);
        Integrity::checkNotNull(&anExampleClass, "m1");
        Integrity::checkNotNull(&anExampleClass, "m1", "m2");

        cout << "... passed" << endl;
    }
    catch (exception& e) {
        cout << "test failed" << endl;
        fail(e.what());
        cout << "... finished tests which should not throw exceptions" << endl;
    }
    catch (char* e) {
        cout << "test failed" << endl;
        fail(e);
        cout << "... finished tests which should not throw exceptions" << endl;
    }
    catch (int& e) {
        cout << "test failed, signal " << e << endl;
        cout << "... finished tests which should not throw exceptions" << endl;
    }
    catch (...) {
        cout << "test failed, unknown exception" << endl;
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
    AnExampleClass anExampleClass(&anInt, &anInt, &anInt);

    //Integrity::checkNotNullM(anExampleClass, [=](Integrity::out out) { out << "message"; });
    //Integrity::check(aLong = anInt, "oopsies");
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

    //Integrity::checkNotNullM(anInt, [](std::stringstream& ss) {});

    //Integrity::checkStringNotNullOrEmpty(1);
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

    expect_throw([=]() {
        Integrity::fail();
        }, "Integrity check failed");

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
        Integrity::fail(__func__); // just curious what would haoppen when you use __fail__ inside a lambda
        }, "operator ()");

    string std_string = "";
    const string* std_string_ptr = &std_string;
    string* std_string_ptr2 = 0;

    expect_throw([=]() {
        Integrity::checkStringNotNullOrEmpty(std_string);
        }, "Empty string");

    expect_throw([=]() {
        Integrity::checkStringNotNullOrEmpty("");
        }, "Empty string");

    expect_throw([=]() {
        Integrity::checkStringNotNullOrEmpty(std_string_ptr);
        }, "Empty string");

    expect_throw([=]() {
        Integrity::checkStringNotNullOrEmpty(std_string_ptr2);
        }, "Empty string");

    const char* const_char_star = 0;

    expect_throw([=]() {
        Integrity::checkStringNotNullOrEmpty(const_char_star);
        }, "Null pointer");

    expect_throw([=]() {
        char* cc = (char*) const_char_star;
        Integrity::checkStringNotNullOrEmpty(cc);
        }, "Null pointer");

    cout << "...Tests which SHOULD throw an exception finished\n";

}

void signalHandler(int sig) {
    cout << "signal raised: " << sig;
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
