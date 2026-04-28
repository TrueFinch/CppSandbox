//
// Created by Vladimir on 20.04.2026.
//

#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string_view>
#include <vector>
#include <type_traits>
#include <string>
#include <variant>

namespace my {
	// forward declaration to use in detail namespace
	template<typename T>
	struct type_name_impl;

	namespace detail {
		template<typename T>
		constexpr bool always_false_v = false;

		template<typename T>
		struct enforce_fundamental {
			static_assert(std::is_fundamental_v<T>,
						"Fundamental_type_name can only be specialized for fundamental types!");
			using type = T;
		};

		template<typename T>
		concept is_fundamental_type = requires { typename enforce_fundamental<T>::type; };

		// Users can specialize this struct to add support for other fundamental types
		template<typename T>
			requires detail::is_fundamental_type<T>
		struct fundamental_type_name {
			static_assert(detail::always_false_v<T>,
						"Unsupported fundamental type! Please, specialize my::fundamental_type_name<T>.");

			static std::string name() { return "unknown"; }
		};

		template<typename T>
		constexpr std::string_view type_name_v = "unknown";

		template<template<typename...> class C>
		constexpr std::string_view template_type_name_v = "unknown";

		template<typename T>
		inline constexpr bool is_hidden_class_template_arg_v = false;

		template<typename T>
		concept is_hidden_arg = is_hidden_class_template_arg_v<T>;

		template<bool B, typename T>
		struct hidden_arg_appender {
			static void append(std::string&, bool&) {
				// nothing - аргумент скрывается
			}
		};

		template<typename T>
		struct hidden_arg_appender<false, T> {
			static void append(std::string& s, bool& first) {
				s += (first ? (first = false, "") : ", ") + type_name_impl<T>::build();
			}
		};
	}

	template<typename T>
	struct type_name_impl {
		static std::string build(const std::string& decl = "") {
			if constexpr (std::is_const_v<T>) {
				return "const " + type_name_impl<std::remove_const_t<T>>::build(decl);
			} else if constexpr (std::is_fundamental_v<T>) {
				return detail::fundamental_type_name<T>::name() + decl;
			} else {
				return std::string(detail::type_name_v<T>) + decl;
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

	template<template<typename... Args> class C, typename... Args>
	struct type_name_impl<C<Args...>> {
		static std::string build(const std::string& decl = "") {
			std::string args_str = "<";
			bool first = true;
			((detail::hidden_arg_appender<detail::is_hidden_arg<Args>, Args>::append(args_str, first)), ...);
			args_str += ">";

			return std::string(detail::template_type_name_v<C>) + args_str + decl;
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
	struct type_name {
		static std::string name() {
			return type_name_impl<T>::build();
		}
	};
}


#pragma region fundamental_macro

// macro registration for fundamental types
#define REGISTER_FUNDAMENTAL_TYPE(Type, Name)\
namespace my::detail {\
	template<>\
	struct fundamental_type_name<Type> {\
		static std::string name() {\
			return Name;\
		}\
	};\
}

// @formatter:off
REGISTER_FUNDAMENTAL_TYPE(unsigned, "unsigned")
REGISTER_FUNDAMENTAL_TYPE(int, "int")
REGISTER_FUNDAMENTAL_TYPE(float, "float")
REGISTER_FUNDAMENTAL_TYPE(double, "double")
REGISTER_FUNDAMENTAL_TYPE(char, "char")
REGISTER_FUNDAMENTAL_TYPE(bool, "bool")
REGISTER_FUNDAMENTAL_TYPE(long, "long")
REGISTER_FUNDAMENTAL_TYPE(long long, "long long")
REGISTER_FUNDAMENTAL_TYPE(void, "void")
// @formatter:on

#pragma endregion

// macro registration for custom types
#define REGISTER_TYPE_NAME(Type, Name)\
namespace my::detail {\
	template<>\
	constexpr std::string_view type_name_v<Type> = Name;\
}

#pragma region template_type_name_macro

// macro registration for template classes
#define REGISTER_TEMPLATE_TYPE_NAME(Template, Name)\
namespace my::detail {\
	template<>\
	constexpr std::string_view template_type_name_v<Template> = Name;\
}

// @formatter:off
REGISTER_TEMPLATE_TYPE_NAME(std::vector, "vector")
REGISTER_TEMPLATE_TYPE_NAME(std::map, "map")
REGISTER_TEMPLATE_TYPE_NAME(std::set, "set")
REGISTER_TEMPLATE_TYPE_NAME(std::pair, "pair")
REGISTER_TEMPLATE_TYPE_NAME(std::optional, "optional")
REGISTER_TEMPLATE_TYPE_NAME(std::tuple, "tuple")
REGISTER_TEMPLATE_TYPE_NAME(std::variant, "variant")
REGISTER_TEMPLATE_TYPE_NAME(std::shared_ptr, "shared_ptr")
REGISTER_TEMPLATE_TYPE_NAME(type_name, "type_name")

REGISTER_TEMPLATE_TYPE_NAME(std::allocator, "allocator")
REGISTER_TEMPLATE_TYPE_NAME(std::less, "less")
REGISTER_TEMPLATE_TYPE_NAME(std::greater, "greater")
REGISTER_TEMPLATE_TYPE_NAME(std::default_delete, "default_delete")
REGISTER_TEMPLATE_TYPE_NAME(std::hash, "hash")
REGISTER_TEMPLATE_TYPE_NAME(std::equal_to, "equal_to")
// @formatter:on

#pragma endregion

#pragma region hide_template_type_name_macro

// macro registration for hidden template arguments type names
#define HIDE_TEMPLATE_ARG_TYPE_NAME(Type)\
namespace my::detail {\
	template<>\
	inline constexpr bool is_hidden_class_template_arg_v<Type> = true;\
}

// macro registration for showed template arguments type names
#define SHOW_TEMPLATE_ARG_TYPE_NAME(Type)\
namespace my::detail {\
	template<>\
	inline constexpr bool is_hidden_class_template_arg_v<Type> = false;\
}

// macro registration for hidden template arguments template type names
#define HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(TemplateClass)\
namespace my::detail {\
	template<typename... Args>\
	inline constexpr bool is_hidden_class_template_arg_v<TemplateClass<Args...>> = true;\
}

// macro registration for showed template arguments template type names
#define SHOW_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(TemplateClass)\
namespace my::detail {\
	template<typename... Args>\
	inline constexpr bool is_hidden_class_template_arg_v<TemplateClass<Args...>> = false;\
}

// @formatter:off
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::allocator)
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::less)
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::greater)
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::default_delete)
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::hash)
HIDE_TEMPLATE_ARG_TEMPLATE_TYPE_NAME(std::equal_to)
// @formatter:on

#pragma endregion
