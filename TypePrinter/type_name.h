//
// Created by Vladimir on 20.04.2026.
//

#pragma once

#include <iostream>
#include <string_view>
#include <vector>

namespace my {
	template<typename T>
	concept has_valid_name = requires
	{
		{ T::name } -> std::convertible_to<std::string_view>;
	};

	template<typename T>
	struct type_name {
		static std::string name() {
			if (has_valid_name<T>) {
				return std::string(T::name);
			}
			return "unknown";
		}
	};

	template<typename T>
	struct type_name<T*> {
		static std::string name() {
			return type_name<T>::name() + "*";
		}
	};

	template<typename T>
	struct type_name<T* const> {
		static std::string name() {
			return type_name<T*>::name() + " const";
		}
	};

	template<typename T>
	struct type_name<T&> {
		static std::string name() {
			return type_name<T>::name() + "&";
		}
	};

	template<typename T>
	struct type_name<const T> {
		static std::string name() {
			return "const " + type_name<T>::name();
		}
	};

	template<>
	struct type_name<int> {
		static std::string name() {
			return "int";
		}
	};

	template<>
	struct type_name<float> {
		static std::string name() {
			return "float";
		}
	};


	template<typename T>
	struct type_name<std::vector<T>> {
		static std::string name() {
			return "vector<" + type_name<T>::name() + ">";
		}
	};

	template<typename R, typename... Args>
	struct type_name<R(Args...)> {
		static std::string name() {
			auto result = type_name<R>::name();

			bool first = true;
			((result += (first ? (first = false, "") : ", ") + type_name<Args>::name()), ...);

			result += ")";
			return result;
		}
	};
}
