//
// Created by Vladimir on 20.04.2026.
//
#include <catch2/catch_test_macros.hpp>
#include "type_name.h"

class MyClass {
public:
	constexpr static std::string_view name = "MyClass";
};

void f(int) {
}

TEST_CASE("TypePrinter c++ primitives", "[TypePrinter]") {
	SECTION("int") {
		CHECK(my::type_name<int>::name() == "int");
		CHECK(my::type_name<int&>::name() == "int&");
		CHECK(my::type_name<const int&>::name() == "const int&");
		CHECK(my::type_name<int*>::name() == "int*");
		CHECK(my::type_name<const int*>::name() == "const int*");
		CHECK(my::type_name<int* const>::name() == "int* const");
		CHECK(my::type_name<const int* const>::name() == "const int* const");
	}

	SECTION("float") {
		CHECK(my::type_name<float>::name() == "float");
		CHECK(my::type_name<float&>::name() == "float&");
		CHECK(my::type_name<const float&>::name() == "const float&");
		CHECK(my::type_name<float*>::name() == "float*");
		CHECK(my::type_name<const float*>::name() == "const float*");
		CHECK(my::type_name<float* const>::name() == "float* const");
		CHECK(my::type_name<const float* const>::name() == "const float* const");
	}

	SECTION("MyClass") {
		CHECK(my::type_name<MyClass>::name() == "MyClass");
		CHECK(my::type_name<MyClass&>::name() == "MyClass&");
		CHECK(my::type_name<const MyClass&>::name() == "const MyClass&");
		CHECK(my::type_name<MyClass*>::name() == "MyClass*");
		CHECK(my::type_name<const MyClass*>::name() == "const MyClass*");
		CHECK(my::type_name<MyClass* const>::name() == "MyClass* const");
		CHECK(my::type_name<const MyClass* const>::name() == "const MyClass* const");
	}

	SECTION("vector<int>") {
		CHECK(my::type_name<std::vector<int>>::name() == "vector<int>");
		CHECK(my::type_name<std::vector<int>&>::name() == "vector<int>&");
		CHECK(my::type_name<const std::vector<int>&>::name() == "const vector<int>&");
		CHECK(my::type_name<std::vector<int>*>::name() == "vector<int>*");
		CHECK(my::type_name<const std::vector<int>*>::name() == "const vector<int>*");
		CHECK(my::type_name<std::vector<int>* const>::name() == "vector<int>* const");
		CHECK(my::type_name<const std::vector<int>* const>::name() == "const vector<int>* const");
	}

	SECTION("function") {
		CHECK(my::type_name<decltype(f)>::name() == "f(int)");
	}
}
