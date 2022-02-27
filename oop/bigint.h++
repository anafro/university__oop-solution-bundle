/**
 * -----------------------------------------------
 * Big Integer
 * -----------------------------------------------
 * Big integers are used in programs, where default
 * integers type are not enough, e.g. in scientific
 * evaluations or modulations.
 *
 * @since OOP 1.0.0.0
 * @author Anatoly Frolov (contact@anafro.ru)
 */

#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <concepts>
#include <iostream>
#include "utils/type_demangler.h++"

using namespace std::string_literals;

namespace PROJECT_NAME {
    static char minus = '-', plus = '+';

    static bool is_unary_operator(char character) {
        return character == minus || character == plus;
    }

    static int to_int(char character) {
        return character - '0';
    }

    class bigint {
    private:
        std::string numeric_string;
        bool sign;
    public:
        /**
         * Creates a new big integer with value 0.
         */
        bigint();

        /**
         * Creates a new big integer with value passed
         * with 'numeric_string' parameter.
         * @throws std::invalid_argument When 'numeric_string' cannot be used in integer initialization
         * @param numeric_string An initial value of big integer
         */
        bigint(const std::string& numeric_string);

        /**
         * Creates a new big integer with value passed
         * with 'numeric_string' parameter.
         * @param integer An initial value of big integer
         */
        bigint(int integer);

        /**
         * Generates a random big integer with certain
         * digits in it (32 by default).
         *
         * @param digits A count of digits in generated big integer
         * @return A random integer with certain digits in it (32 by default).
         */
        static bigint random(int digits = 32);

        /**
         * Sets a new value to a big integer object.
         * @throws std::invalid_argument When 'new_value' cannot be used in integer initialization
         * @param new_value A new value of a big integer object.
         */
        void set_value(const std::string& new_value);

        /**
         * Returns a numeric string, representation of big integer.
         * @return A numeric string, representation of big integer
         */
        [[nodiscard]]
        std::string to_string() const;

        /**
         * Returns an unsigned part of big integer as a numeric string.
         * @return An unsigned part of big integer as a numeric string
         */
         [[nodiscard]]
         const std::string& get_numeric_string() const;

        /**
         * Gets a value of big integer of type specified in T template parameter.
         * @tparam T The return type parameter.
         * @throws std::runtime_error If this big integer value is too big to be in the T type
         * @return A value of big integer of type specified in T template parameter.
         */
        template<typename T = long long int>
        requires std::is_integral_v<T>
        [[nodiscard]]
        T get_value() const {
            if(bigint(std::numeric_limits<T>::max()) < *this ||
               bigint(std::numeric_limits<T>::min()) > *this) {
                throw std::runtime_error("Type "s + type<T>() + " is too " + (sign ? "small" : "big") + " for containing big integer value '" + to_string());
            }

            return std::stoll(to_string());
        }

        /**
         * Sets a new sign of big integer judging by the 'new_sign' parameter.
         * If 'true' passed, the big integer will be positive, otherwise negative.
         * @param new_sign
         */
        void set_sign(bool new_sign);

        /**
         * Sets a new sign of big integer judging by the 'new_sign' parameter.
         * If '+' passed, the big integer will be positive.
         * If '-' passed, the big integer will be negative.
         * @throws std::invalid_argument When new_sign is neither '+' nor '-'
         * @param new_sign A new sign for a big integer
         */
        void set_sign(char new_sign);

        /**
         * Make the big integer negative regardless the current sign.
         * @returns A copy of negative integer
         */
        bigint make_negative();

        /**
         * Make the big integer positive regardless the current sign.
         * @returns A copy of positive integer
         */
        bigint make_positive();

        /**
         * Returns a current big integer's sign.
         * @return A current big integer's sign.
         */
        [[nodiscard]]
        const bool& get_sign() const;

        /**
         * Returns true if this big integer is positive, otherwise
         * returns false.
         *
         * @return True if this big integer is positive, otherwise
         * returns false.
         */
        [[nodiscard]]
        bool is_positive() const;

        /**
         * Returns true if this big integer is negative, otherwise
         * returns false.
         *
         * @return True if this big integer is negative, otherwise
         * returns false.
         */
        [[nodiscard]]
        bool is_negative() const;

        /**
         * Returns a count of digits. Note that the minus sign is not a digit.
         * @returns A count of digits of this big integer
         */
        [[nodiscard]]
        int count_digits() const&;

        /**
         * Assigns a new value to a big integer object
         * @param new_value A big integer where values will be copied from
         * @returns This big integer
         */
        bigint& operator=(const bigint& new_value);

        /**
         * Returns true if values of this big integer and
         * passed 'comparing_with' big integer are equal,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if values of this big integer and
         * passed 'comparing_with' big integer are equal,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator==(const bigint& comparing_with) const&;

        /**
         * Returns true if values of this big integer and
         * passed 'comparing_with' big integer aren't equal,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if values of this big integer and
         * passed 'comparing_with' big integer aren't equal,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator!=(const bigint& comparing_with) const&;

        /**
         * Returns true if value of this big integer is greater
         * than value passed 'comparing_with' big integer,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if value of this big integer is greater
         * than value passed 'comparing_with' big integer,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator>(const bigint& comparing_with) const&;

        /**
         * Returns true if value of this big integer is less
         * than value passed 'comparing_with' big integer,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if value of this big integer is less
         * than value passed 'comparing_with' big integer,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator<(const bigint& comparing_with) const&;

        /**
         * Returns true if value of this big integer is greater or equal to
         * value passed 'comparing_with' big integer,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if value of this big integer is greater or equal to
         * value passed 'comparing_with' big integer,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator>=(const bigint& comparing_with) const&;

        /**
         * Returns true if value of this big integer is less or equal
         * to value passed 'comparing_with' big integer,
         * otherwise false.
         *
         * @param comparing_with The comparing value
         * @return true if value of this big integer is less or equal
         * to value passed 'comparing_with' big integer,
         * otherwise false.
         */
        [[nodiscard]]
        bool operator<=(const bigint& comparing_with) const&;

        /**
         * Prefix incrementing the big integer
         * @return A reference to this big integer
         */
        bigint& operator++();

        /**
         * Postfix incrementing the big integer
         * @return A copied big integer
         */
        bigint operator++(int);

        /**
         * Prefix decrementing the big integer
         * @return A reference to this big integer
         */
        bigint& operator--();

        /**
         * Postfix decrementing the big integer
         * @return A copied big integer
         */
        bigint operator--(int);

        /**
         * Adds this big integer to another and returns
         * the sum of this operation.
         *
         * @param with The right addition operand
         * @return The sum
         */
        [[nodiscard]]
        bigint operator+(const bigint& with) const;

        /**
         * Subtracts the value of passed big integer
         * from this big integer object and returns
         * the subtraction result of this operation.
         *
         * @param what The right subtraction operand
         * @return The subtraction
         */
        [[nodiscard]]
        bigint operator-(const bigint& what) const&;

        /**
         * Multiplies this big integer by another and returns
         * the sum of this operation.
         *
         * @param by The right multiplier
         * @return The multiplication
         */
        [[nodiscard]]
        bigint operator*(const bigint& by) const&;

        /**
         * Adds a value of passed big integer to the big integer.
         *
         * @param what The big integer that will be added to
         * @return A reference to this big integer
         */
        bigint& operator+=(const bigint& what);

        /**
         * Subtracts a value of passed big integer from the big integer.
         *
         * @param what The big integer that will be subtracted from
         * @return A reference to this big integer
         */
        bigint& operator-=(const bigint& what);

        /**
         * Multiplies a value of passed big integer with the big integer.
         *
         * @param by The big integer that will be multiplied by
         * @return A reference to this big integer
         */
        bigint& operator*=(const bigint& by);

        /**
         * Negates the big integer.
         * @return a negated copy o this big integer
         */
        bigint operator-();

        /**
         * Implicitly converts the big integer to std::string.
         * @return The numeric string value of this big integer
         */
        [[nodiscard]]
        operator std::string() const&;

        /**
         * Clones this big integer.
         * @returns The clone.
         */
        [[nodiscard]]
        bigint clone() const;


        /**
         * Makes big integers printable to output streams.
         * @param stream An output stream where this big integer will be written to
         */
        friend auto operator<<(std::ostream& stream, const bigint& integer) -> std::ostream&;

        /**
         * Makes big integers readable from input streams.
         * @param stream An input stream where this big integer will be read from
         */
        friend auto operator>>(std::istream& stream, bigint& integer) -> std::istream&;
    };
}