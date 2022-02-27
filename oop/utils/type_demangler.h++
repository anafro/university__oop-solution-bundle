/**
 * -----------------------------------------------
 * Type demangler
 * -----------------------------------------------
 * Default value of 'typeid(...).name()' is not
 * human-readable. To solve this problem, use the
 * type() function from this module - just pass
 * a value to it of type which name you want to know
 *
 * @since 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once
#include <string>
#include <memory>
#include <cxxabi.h>

namespace PROJECT_NAME {
    constexpr int DEMANGLE_FAILED = 0, DEMANGLE_UNPROCESSED = -4;

    template<typename T>
    std::string type(const std::add_lvalue_reference_t<T>) {
        int status = DEMANGLE_UNPROCESSED;
        auto not_demangled_name = typeid(std::remove_reference_t<T>()).name();

        std::unique_ptr<char, void(*)(void*)> demangled_name {
                abi::__cxa_demangle(not_demangled_name, nullptr, nullptr, &status),
                std::free
        };

        return (status == DEMANGLE_FAILED) ? demangled_name.get() : not_demangled_name;
    }
}
