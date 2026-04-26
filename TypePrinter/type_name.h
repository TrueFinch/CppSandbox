//
// Created by Vladimir on 20.04.2026.
//

#pragma once

#include <string_view>
#include <vector>
#include <type_traits>
#include <string>

namespace my {
	namespace detail {
		template<typename T>
		constexpr bool always_false_v = false;

		template<typename T>
		struct enforce_fundamental {
			static_assert(std::is_fundamental_v<T>,
						"Error: fundamental_type_name can only be specialized for fundamental types!");
			using type = T;
		};

		template<typename T>
		concept is_fundamental_type = requires { typename enforce_fundamental<T>::type; };

		template<typename T>
		concept has_valid_name = requires
		{
			{ T::name } -> std::convertible_to<std::string_view>;
		};
	}

	// Users can specialize this struct to add support for other fundamental types
	template<typename T>
		requires detail::is_fundamental_type<T>
	struct fundamental_type_name {
		static_assert(detail::always_false_v<T>,
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
	struct type_name_impl {
		static std::string build(const std::string& decl = "") {
			if constexpr (std::is_const_v<T>) {
				return "const " + type_name_impl<std::remove_const_t<T>>::build(decl);
			} else if constexpr (std::is_fundamental_v<T>) {
				return fundamental_type_name<T>::name() + decl;
			} else if constexpr (detail::has_valid_name<T>) {
				return std::string(T::name) + decl;
			} else {
				return "unknown" + decl;
			}
		}
	};

	template<typename T>
	struct type_name_impl<T*> {
		static std::string build(const std::string& decl = "") {
			return type_name_impl<T>::build("*" + decl);
		}
	};

	template<typename T>
	struct type_name_impl<T* const> {
		static std::string build(const std::string& decl = "") {
			return type_name_impl<T>::build("* const" + decl);
		}
	};

	template<typename T>
	struct type_name_impl<T&> {
		static std::string build(const std::string& decl = "") {
			return type_name_impl<T>::build("&" + decl);
		}
	};

	template<typename T>
	struct type_name_impl<T&&> {
		static std::string build(const std::string& decl = "") {
			return type_name_impl<T>::build("&&" + decl);
		}
	};

	template<typename R, typename... Args>
	struct type_name_impl<R(Args...)> {
		static std::string build(const std::string& decl = "") {
			std::string args_str = "(";
			bool first = true;
			((args_str += (first ? (first = false, "") : ", ") + type_name_impl<Args>::build()), ...);
			args_str += ")";

			std::string new_decl = decl.empty() ? args_str : "(" + decl + ")" + args_str;
			return type_name_impl<R>::build(new_decl);
		}
	};

	template<>
	struct type_name_impl<std::string> {
		static std::string build(const std::string& decl = "") {
			return "string" + decl;
		}
	};

	template<>
	struct type_name_impl<std::string_view> {
		static std::string build(const std::string& decl = "") {
			return "string_view" + decl;
		}
	};

	template<typename T>
	struct type_name_impl<std::vector<T>> {
		static std::string build(const std::string& decl = "") {
			return "vector<" + type_name_impl<T>::build() + ">" + decl;
		}
	};

	template<typename T>
	struct type_name {
		static std::string name() {
			return type_name_impl<T>::build();
		}
	};
}
