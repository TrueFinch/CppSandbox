//
// Created by Vladimir on 16.04.2026.
//

#pragma once

#include <memory>
#include <typeinfo>

namespace myspace {
	class my_any_bad_cast final: public std::bad_cast {};

	class MyAny final {
		class Base {
		public:
			virtual ~Base() = default;

			[[nodiscard]] virtual const std::type_info& Type() const = 0;
		};

		template<typename T>
		class Container final: public Base {
		public:
			[[nodiscard]] const std::type_info& Type() const override {
				return typeid(T);
			}

			const T& GetValue() const {
				return value;
			}

			T& GetValue() {
				return value;
			}

		private:
			T value;

		public:
			explicit Container(T val): value(std::move(val)) {}
		};

		std::unique_ptr<Base> ptr;

	public:
		MyAny(): ptr(nullptr) {}

		template<typename T>
		explicit MyAny(T value) {
			ptr = std::make_unique<Container<T>>(std::move(value));
		}

		MyAny& Reset() {
			ptr.reset(nullptr);
			return *this;
		}

		template<typename T>
		[[nodiscard]] bool HasType() const {
			return ptr && ptr->Type() == typeid(T);
		}

		[[nodiscard]] bool HasValue() const {
			return ptr != nullptr;
		}

		template<typename T>
		T* Get() {
			if (HasType<T>()) {
				return &dynamic_cast<Container<T>*>(ptr.get())->GetValue();
			}
			return nullptr;
		}

		template<typename T>
		const T* Get() const {
			if (HasType<T>()) {
				return &dynamic_cast<const Container<T>*>(ptr.get())->GetValue();
			}
			return nullptr;
		}
	};

	template<typename T>
	T my_any_cast(const MyAny& any) {
		if (any.HasType<T>()) {
			return *any.Get<T>();
		}
		throw my_any_bad_cast();
	}

	template<typename T>
	T my_any_cast(MyAny& any) {
		if (any.HasType<T>()) {
			return *any.Get<T>();
		}
		throw my_any_bad_cast();
	}

	template<typename T>
	T* my_any_cast(MyAny* any) {
		if (!any) {
			return nullptr;
		}
		return any->Get<T>();
	}

	template<typename T>
	const T* my_any_cast(const MyAny* any) {
		if (!any) {
			return nullptr;
		}
		return any->Get<T>();
	}
}
