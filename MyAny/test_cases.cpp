#include <catch2/catch_test_macros.hpp>
#include "MyAny.h"

TEST_CASE("MyAny basic operations", "[MyAny]") {
	using namespace myspace;

	SECTION("Construction and type checking") {
		MyAny a;
		CHECK_FALSE(a.HasValue());
		CHECK_FALSE(a.HasType<int>());

		MyAny b(42);
		CHECK(b.HasValue());
		CHECK(b.HasType<int>());
		CHECK_FALSE(b.HasType<float>());

		MyAny c(std::string("hello"));
		CHECK(c.HasType<std::string>());
	}

	SECTION("Value retrieval using Get") {
		MyAny a(100);
		CHECK(a.HasValue());
		int* ptr = a.Get<int>();
		REQUIRE(ptr != nullptr);
		CHECK(*ptr == 100);

		CHECK(a.Get<float>() == nullptr);
	}

	SECTION("my_any_cast(nullptr)") {
		MyAny* a = nullptr;
		CHECK(my_any_cast<int>(a) == nullptr);
	}

	SECTION("Value retrieval using my_any_cast(MyAny&)") {
		MyAny a(3.14f);
		CHECK(a.HasValue());
		CHECK(my_any_cast<float>(a) == 3.14f);
		CHECK_THROWS_AS(my_any_cast<int>(a), my_any_bad_cast);
		a.Reset();
		CHECK_FALSE(a.HasValue());
		CHECK_THROWS_AS(my_any_cast<int>(a), my_any_bad_cast);
		const MyAny b(3.14f);
		CHECK(b.HasValue());
		CHECK(my_any_cast<float>(b) == 3.14f);
	}

	SECTION("Value retrieval using my_any_cast(MyAny*)") {
		auto a = std::make_shared<MyAny>(3.14f);
		CHECK(a->HasValue());
		CHECK(*my_any_cast<float>(a.get()) == 3.14f);
		CHECK(my_any_cast<int>(a.get()) == nullptr);
		a->Reset();
		CHECK_FALSE(a->HasValue());
		CHECK(my_any_cast<int>(a.get()) == nullptr);
		auto b = std::make_shared<MyAny>(3.14f);
		CHECK(*my_any_cast<float>(b.get()) == 3.14f);
	}

	SECTION("std::string support") {
		MyAny a(std::string("test"));
		CHECK(my_any_cast<std::string>(a) == "test");
	}

	SECTION("") {

	}
}
