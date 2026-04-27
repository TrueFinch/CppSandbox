//
// Created by Vladimir on 20.04.2026.
//
#include <catch2/catch_test_macros.hpp>
#include "type_name.h"
#include <string>
#include <map>

TEST_CASE("TypePrinter fundamental types", "[TypePrinter]") {
	SECTION("simple types") {
		CHECK(my::type_name<int>::name() == "int");
		CHECK(my::type_name<float>::name() == "float");
		CHECK(my::type_name<double>::name() == "double");
		CHECK(my::type_name<char>::name() == "char");
		CHECK(my::type_name<bool>::name() == "bool");
		CHECK(my::type_name<long>::name() == "long");
		CHECK(my::type_name<long long>::name() == "long long");
		CHECK(my::type_name<void>::name() == "void");
	}

	SECTION("complex types") {
		// pointer and const variations from old "int" section on int
		CHECK(my::type_name<int&>::name() == "int&");
		CHECK(my::type_name<const int&>::name() == "const int&");
		CHECK(my::type_name<int*>::name() == "int*");
		CHECK(my::type_name<const int*>::name() == "const int*");
		CHECK(my::type_name<int* const>::name() == "int* const");
		CHECK(my::type_name<const int* const>::name() == "const int* const");
		CHECK(my::type_name<int* const*>::name() == "int* const*");
		CHECK(my::type_name<const int* const*>::name() == "const int* const*");

		// double pointers on int
		CHECK(my::type_name<int**>::name() == "int**");
		CHECK(my::type_name<const int**>::name() == "const int**");
		CHECK(my::type_name<int** const>::name() == "int** const");
		CHECK(my::type_name<int* const* const>::name() == "int* const* const");
		CHECK(my::type_name<const int* const*>::name() == "const int* const*");
		CHECK(my::type_name<const int** const>::name() == "const int** const");

		// triple pointers
		CHECK(my::type_name<int***>::name() == "int***");
		CHECK(my::type_name<int*** const>::name() == "int*** const");
		CHECK(my::type_name<const int***>::name() == "const int***");
		CHECK(my::type_name<const int** const>::name() == "const int** const");
		CHECK(my::type_name<int* const**>::name() == "int* const**");

		// rvalue references
		CHECK(my::type_name<int&&>::name() == "int&&");
		CHECK(my::type_name<const int&&>::name() == "const int&&");
		CHECK(my::type_name<double&&>::name() == "double&&");
		CHECK(my::type_name<double&>::name() == "double&");
		CHECK(my::type_name<const double&>::name() == "const double&");
		CHECK(my::type_name<float&>::name() == "float&");
		CHECK(my::type_name<const float&>::name() == "const float&");

		// const on fundamental types
		CHECK(my::type_name<const int>::name() == "const int");
		CHECK(my::type_name<const double>::name() == "const double");
		CHECK(my::type_name<const char>::name() == "const char");
		CHECK(my::type_name<const bool>::name() == "const bool");
		CHECK(my::type_name<const float>::name() == "const float");
		CHECK(my::type_name<const long>::name() == "const long");
		CHECK(my::type_name<const long long>::name() == "const long long");

		// rvalue ref on float/double
		CHECK(my::type_name<float&&>::name() == "float&&");
	}

	// SECTION("unsupported fundamental types") {
	// 	CHECK_FALSE(my::type_name<unsigned>::name() == "unsigned");
	// }
}

TEST_CASE("TypePrinter containers", "[TypePrinter]") {
	SECTION("vector<int>") {
		CHECK(my::type_name<std::vector<int>>::name() == "vector<int>");
		CHECK(my::type_name<std::vector<int>&>::name() == "vector<int>&");
		CHECK(my::type_name<const std::vector<int>&>::name() == "const vector<int>&");
		CHECK(my::type_name<std::vector<int>*>::name() == "vector<int>*");
		CHECK(my::type_name<const std::vector<int>*>::name() == "const vector<int>*");
		CHECK(my::type_name<std::vector<int>* const>::name() == "vector<int>* const");
		CHECK(my::type_name<const std::vector<int>* const>::name() == "const vector<int>* const");
	}

	SECTION("vector variations") {
		CHECK(my::type_name<std::vector<double>>::name() == "vector<double>");
		CHECK(my::type_name<std::vector<double>>::name() == "vector<double>");
		CHECK(my::type_name<std::vector<const int>>::name() == "vector<const int>");
		CHECK(my::type_name<std::vector<int&>>::name() == "vector<int&>");
		CHECK(my::type_name<std::vector<int*>>::name() == "vector<int*>");
	}

	SECTION("vector of func pointers") {
		using pf4 = void (*)();
		CHECK(my::type_name<std::vector<pf4>>::name() == "vector<void(*)()>");
	}

	SECTION("unordered_map<int, int>") {}
}

namespace test_functions {
	void f(int) {}
}

namespace test_functions2 {
	void f_void_empty() {}

	int f_return_int(int a, double b) { return a; }

	double f_return_void_ref(const int& a, std::string b) { return 0.0; }

	void f_rvalue_arg(int&& a) {}

	const double* f_return_ptr() { return nullptr; }

	int& f_return_lvalue_ref(int a) { return a; }

	void f_four_args(double, float, char, bool) {}

	void f_ptr_and_const_ref(int* a, const double& b) {}

	float* f_mixed_return(float* a, const char& b) { return a; }

	void f_all_fundamental(int, float, double, char, bool, long, long long) {}

	const int*& f_return_const_ref_ptr(const int*& a, const int* const& b) { return a; }
}

TEST_CASE("TypePrinter functions", "[TypePrinter]") {
	SECTION("function") {
		using namespace test_functions;
		CHECK(my::type_name<decltype(f)>::name() == "void(int)");
	}

	SECTION("function variations") {
		using namespace test_functions2;
		// empty args
		CHECK(my::type_name<decltype(f_void_empty)>::name() == "void()");

		// 2 different parameter types
		CHECK(my::type_name<decltype(f_return_int)>::name() == "int(int, double)");

		// const reference + value
		CHECK(my::type_name<decltype(f_return_void_ref)>::name() == "double(const int&, string)");

		// rvalue reference parameter
		CHECK(my::type_name<decltype(f_rvalue_arg)>::name() == "void(int&&)");

		// pointer return type
		CHECK(my::type_name<decltype(f_return_ptr)>::name() == "const double*()");

		// lvalue reference return type
		CHECK(my::type_name<decltype(f_return_lvalue_ref)>::name() == "int&(int)");

		// many parameters
		CHECK(my::type_name<decltype(f_four_args)>::name() == "void(double, float, char, bool)");

		// pointer + const reference parameters
		CHECK(my::type_name<decltype(f_ptr_and_const_ref)>::name() == "void(int*, const double&)");

		// mixed return with const reference
		CHECK(my::type_name<decltype(f_mixed_return)>::name() == "float*(float*, const char&)");

		// all fundamental types as parameters
		CHECK(my::type_name<decltype(f_all_fundamental)>::name() ==
			"void(int, float, double, char, bool, long, long long)");

		// complex return: const reference to pointer
		CHECK(my::type_name<decltype(f_return_const_ref_ptr)>::name() == "const int*&(const int*&, const int* const&)");
	}

	SECTION("reference to function") {
		using namespace test_functions;
		auto f = test_functions::f;
		auto& f2 = f;
		// reference to function
		CHECK(my::type_name<void(&)()>::name() == "void(&)()");
		// reference to function
		CHECK(my::type_name<decltype(f)>::name() == "void(*)(int)");
		// reference to pointer to function
		CHECK(my::type_name<decltype(f2)>::name() == "void(*&)(int)");
	}
}

namespace test_func_ptrs {
	void func_ptr1(int x) {}

	double func_ptr2(double d, char c) { return d; }

	void func_ptr3() {}

	const int* func_ptr4(const int& a) { return &a; }
}


TEST_CASE("TypePrinter function pointers", "[TypePrinter]") {
	SECTION("function pointer types") {
		using pf1 = void(*)(int);
		using pf2 = double(*)(double, char);
		using pf3 = void(*)();
		using pf4 = const int*(*)(const int&);

		CHECK(my::type_name<pf1>::name() == "void(*)(int)");
		CHECK(my::type_name<pf2>::name() == "double(*)(double, char)");
		CHECK(my::type_name<pf3>::name() == "void(*)()");
		CHECK(my::type_name<pf4>::name() == "const int*(*)(const int&)");
	}

	// SECTION("function pointer const variations") {
	// 	auto cfp1 = static_cast<void (*)(int) const>(nullptr);
	// 	auto cfp2 = static_cast<double (*)(double, char) const>(nullptr);
	//
	// 	CHECK(my::type_name<std::remove_pointer_t<decltype(cfp1)>>::name() == "void(int) const");
	// 	CHECK(my::type_name<std::remove_pointer_t<decltype(cfp2)>>::name() == "double(double, char) const");
	// }

	SECTION("double pointer to function") {
		using pfp1 = void(**)(int);
		using pfp2 = double(**)(double, char);

		CHECK(my::type_name<pfp1>::name() == "void(**)(int)");
		CHECK(my::type_name<pfp2>::name() == "double(**)(double, char)");
	}

	SECTION("triple pointer to function") {
		using pfp1 = void (***)(int);
		using pfp2 = double (***)(double, char);

		CHECK(my::type_name<pfp1>::name() == "void(***)(int)");
		CHECK(my::type_name<pfp2>::name() == "double(***)(double, char)");
	}

	SECTION("pointer to function receiving pointer to function") {
		using pfp1 = void(*)(void (*)());
		CHECK(my::type_name<pfp1>::name() == "void(*)(void(*)())");
	}

	SECTION("const pointer to function") {
		using pfp1 = void(* const)(int);
		CHECK(my::type_name<pfp1>::name() == "void(* const)(int)");
	}
}


namespace test_classes {
	class MyClass {
	public:
		constexpr static std::string_view name = "MyClass";
	};

	struct MyStruct {
		// constexpr static std::string_view name = "MyStruct";
	};


	template<typename T>
	struct MyTemplateClass {
		constexpr static std::string_view name = "MyTemplateClass";
	};
}

REGISTER_TYPE_NAME(test_classes::MyStruct, "MyStruct");

REGISTER_TYPE_NAME(test_classes::MyClass, "MyClass");

REGISTER_TEMPLATE_TYPENAME(test_classes::MyTemplateClass, "MyTemplateClass");


TEST_CASE("TypePrinter custom classes", "[TypePrinter]") {
	using namespace test_classes;

	SECTION("MyClass") {
		CHECK(my::type_name<MyClass>::name() == "MyClass");
		CHECK(my::type_name<MyClass&>::name() == "MyClass&");
		CHECK(my::type_name<const MyClass&>::name() == "const MyClass&");
		CHECK(my::type_name<MyClass*>::name() == "MyClass*");
		CHECK(my::type_name<const MyClass*>::name() == "const MyClass*");
		CHECK(my::type_name<MyClass* const>::name() == "MyClass* const");
		CHECK(my::type_name<const MyClass* const>::name() == "const MyClass* const");
	}

	SECTION("MyStruct") {
		CHECK(my::type_name<MyStruct>::name() == "MyStruct");
		CHECK(my::type_name<MyStruct&>::name() == "MyStruct&");
		CHECK(my::type_name<const MyStruct*>::name() == "const MyStruct*");
	}

	SECTION("MyTemplateClass") {
		CHECK(my::type_name<MyTemplateClass<int>>::name() == "MyTemplateClass<int>");
	}
}


TEST_CASE("Template class parameters", "[TypePrinter]") {
	SECTION("map types") {
		CHECK(my::type_name<std::map<int, int>>::name() == "unknown<int, int>");
		CHECK(my::type_name<std::map<int, std::vector<double>>>::name() == "unknown<int, vector<double>>");
	}

	SECTION("pair types") {
		CHECK(my::type_name<std::pair<int, double>>::name() == "unknown<int, double>");
		CHECK(my::type_name<std::pair<std::string, std::string>>::name() == "unknown<string, string>");
	}

	SECTION("optional types") {
		CHECK(my::type_name<std::optional<int>>::name() == "unknown<int>");
		CHECK(my::type_name<std::optional<std::string>>::name() == "unknown<string>");
	}

	SECTION("tuple types") {
		CHECK(my::type_name<std::tuple<int, std::string, double>>::name() == "unknown<int, string, double>");
	}

	SECTION("variant types") {
		CHECK(my::type_name<std::variant<int, double>>::name() == "unknown<int, double>");
	}

	SECTION("shared_ptr types") {
		CHECK(my::type_name<std::shared_ptr<int>>::name() == "unknown<int>");
		CHECK(my::type_name<std::shared_ptr<std::string>>::name() == "unknown<string>");
	}

	SECTION("const template params") {
		CHECK(my::type_name<const std::map<int, int>&>::name() == "const unknown<int, int>&");
		CHECK(my::type_name<const std::map<int, int>*>::name() == "const unknown<int, int>*");
		CHECK(my::type_name<std::map<int, int>* const>::name() == "unknown<int, int>* const");
	}

	SECTION("pointer template params") {
		CHECK(my::type_name<std::map<int, int>*>::name() == "unknown<int, int>*");
	}
}
