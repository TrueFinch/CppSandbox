//
// Created by Vladimir on 20.04.2026.
//

#pragma once

#include <string_view>
#include <vector>
#include <type_traits>
#include <string>

namespace my {
	template<typename T>
	constexpr bool always_false_v = false;

	// Users can specialize this struct to add support for other fundamental types
	template<typename T>
	struct fundamental_type_name {
		static_assert(always_false_v<T>,
					"Unsupported fundamental type! Please, specialize my::fundamental_type_name<T>.");

		static std::string name() { return "unknown"; }
	};

	// @formatter:off
	template<> struct fundamental_type_name<int> { static std::string name() { return "int"; } };
	template<> struct fundamental_type_name<float> { static std::string name() { return "float"; } };
	template<> struct fundamental_type_name<double> { static std::string name() { return "double"; } };
	template<> struct fundamental_type_name<char> { static std::string name() { return "char"; } };
	template<> struct fundamental_type_name<bool> { static std::string name() { return "bool"; } };
	template<> struct fundamental_type_name<long> { static std::string name() { return "long"; } };
	template<> struct fundamental_type_name<long long> { static std::string name() { return "long long"; } };
	template<> struct fundamental_type_name<void> { static std::string name() { return "void"; } };
	// @formatter:on

	template<typename T>
	concept has_valid_name = requires
	{
		{ T::name } -> std::convertible_to<std::string_view>;
	};

	template<typename T>
	struct type_name {
		static std::string name() {
			// 1. Handle references
			if constexpr (std::is_lvalue_reference_v<T>) {
				return type_name<std::remove_reference_t<T>>::name() + "&";
			} else if constexpr (std::is_rvalue_reference_v<T>) {
				return type_name<std::remove_reference_t<T>>::name() + "&&";
			}
			// 2. Handle pointers
			else if constexpr (std::is_pointer_v<T>) {
				if constexpr (std::is_const_v<T>) {
					return type_name<std::remove_pointer_t<T>>::name() + "* const";
				} else {
					return type_name<std::remove_pointer_t<T>>::name() + "*";
				}
			}
			// 3. Handle const
			else if constexpr (std::is_const_v<T>) {
				return "const " + type_name<std::remove_const_t<T>>::name();
			}
			// 4. Handle fundamental types
			else if constexpr (std::is_fundamental_v<T>) {
				return fundamental_type_name<T>::name();
			}
			// 5. Handle custom classes (via concept)
			else if constexpr (has_valid_name<T>) {
				return std::string(T::name);
			}
			// 6. Fallback
			else {
				return "unknown";
			}
		}
	};

	// Specialization for std::string
	template<>
	struct type_name<std::string> {
		static std::string name() {
			return "string";
		}
	};

	// Specialization for std::string_view
	template<>
	struct type_name<std::string_view> {
		static std::string name() {
			return "string_view";
		}
	};

	// Specialization for std::vector to handle its template structure
	template<typename T>
	struct type_name<std::vector<T>> {
		static std::string name() {
			return "vector<" + type_name<T>::name() + ">";
		}
	};

	// Specialization for function pointer types (pointer to R(Args...))
	template<typename R, typename... Args>
	struct type_name<R(*)(Args...)> {
		static std::string name() {
			auto result = type_name<R>::name() + " (*)(";
			bool first = true;
			((result += (first ? (first = false, "") : ", ") + type_name<Args>::name()), ...);
			result += ")";
			return result;
		}
	};

	// Specialization for function signatures
	template<typename R, typename... Args>
	struct type_name<R(Args...)> {
		static std::string name() {
			auto result = type_name<R>::name() + "(";

			bool first = true;
			((result += (first ? (first = false, "") : ", ") + type_name<Args>::name()), ...);

			result += ")";
			return result;
		}
	};
}
