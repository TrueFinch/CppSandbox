//
// Created by Vladimir on 20.04.2026.
//

#include <iostream>

#include "type_name.h"

class MyMainClass {
public:
	constexpr static std::string_view name = "MyMainClass";
};

int main() {
	std::cout << my::has_valid_name<MyMainClass> << std::endl;

	return 0;
}
