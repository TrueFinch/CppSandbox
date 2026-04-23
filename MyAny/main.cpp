//
// Created by Vladimir on 14.04.2026.
//

#include <iostream>
#include <vector>

auto f(const std::string& str) {
	return str;
}

decltype(auto) f1(const std::string& str) {
	return str;
}

// template<typename T>
// auto ff(T&& arg) {
// 	return std::forward<T>(arg);
// }
template<typename T>
decltype(auto) ff(T&& arg) {
	return std::forward<T>(arg);
}

int main() {
	auto g = f("test");
	auto g1 = f1("test");
	decltype(auto) g2 = f1("test");
	decltype(auto) g3 = f("test");
	auto g4 = f("test");

	{
		const auto copy = ff(std::vector<int>{}); // объект скопируется
		const auto& ref = ff(std::vector<int>{10}); // сохраним ссылку на временный объект

		std::cout << ref.back();
	}
	return 0;
}
