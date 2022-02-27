/**
 * -----------------------------------------------
 * type_demangler
 * -----------------------------------------------
 * ?
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