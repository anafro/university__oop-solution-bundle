#include <bigint.h++>

namespace PROJECT_NAME {
    bigint::bigint() : bigint("0") {
        //
    }

    bigint::bigint(const std::string& numeric_string) {
        if(numeric_string.empty()) {
            throw std::invalid_argument("An empty string cannot be used as a value of big integer. If you want to create 0 big integer, use \"0\"");
        }


        for(unsigned int index = 0; index < numeric_string.length(); index++) {
            char character = numeric_string[index];
            //if(!isdigit(numeric_string[index]) || !(index == 0 && numeric_string[index] != minus && numeric_string[index] != plus)) {
            if(!(isdigit(character) || (index == 0 && is_unary_operator(character)))) {
                throw std::invalid_argument("Character '"s + numeric_string[index] + "' cannot be used in big integer");
            }
        }

        set_sign(numeric_string[0] != minus || numeric_string == "0");
        this->numeric_string = is_unary_operator(numeric_string[0]) ? numeric_string.substr(1) : numeric_string;
    }

    bigint::bigint(int integer) : bigint(std::to_string(integer)) {
        //
    }

    bigint bigint::random(int digits) {
        std::stringstream numeric_string_buffer;

        if(digits <= 0) {
            throw std::invalid_argument("Random big integer cannot have length "s + std::to_string(digits));
        }

        for(int i = 0; i < digits; i++) {
            numeric_string_buffer << ('0' + rand() % 9);
        }

        return (rand() % 2 == 0 ? plus : minus) + numeric_string_buffer.str();
    }

    void bigint::set_value(const std::string& new_value) {
        bigint new_integer = new_value;

        this->numeric_string = new_integer.numeric_string;
        this->sign = new_integer.sign;
    }

    [[nodiscard]]
    std::string bigint::to_string() const {
        std::string result;
        if(is_negative())
            result += minus;

        return result + numeric_string;
    }

    [[nodiscard]]
    const std::string& bigint::get_numeric_string() const {
        return numeric_string;
    }

    void bigint::set_sign(bool new_sign) {
        this->sign = new_sign;
    }

    void bigint::set_sign(char new_sign) {
        if(is_unary_operator(new_sign)) {
            this->sign = new_sign == plus;
        } else {
            throw std::invalid_argument("Cannot set '"s + new_sign + "' as a sign of big integer. Use " + plus + " or " + minus);
        }
    }

    bigint bigint::make_negative() {
        set_sign(minus);
        return *this;
    }

    bigint bigint::make_positive() {
        set_sign(plus);
        return *this;
    }

    [[nodiscard]]
    const bool& bigint::get_sign() const {
        return sign;
    }

    [[nodiscard]]
    bool bigint::is_positive() const {
        return get_sign();
    }

    [[nodiscard]]
    bool bigint::is_negative() const {
        return !is_positive();
    }

    [[nodiscard]]
    int bigint::count_digits() const& {
        return numeric_string.length();
    }

    bigint& bigint::operator=(const bigint& new_value) = default;

    [[nodiscard]]
    bool bigint::operator==(const bigint& comparing_with) const& {
        return (numeric_string == comparing_with.numeric_string) && (sign == comparing_with.sign);
    }

    [[nodiscard]]
    bool bigint::operator!=(const bigint& comparing_with) const& {
        return !(*this == comparing_with);
    }

    [[nodiscard]]
    bool bigint::operator>(const bigint& comparing_with) const& {
        if(*this == comparing_with) {
            return false;
        }

        if(sign != comparing_with.sign) {
            return sign;
        }

        if(count_digits() != comparing_with.count_digits()) {
            return count_digits() > comparing_with.count_digits();
        }

        for(int index = 0; index < count_digits(); index++) {
            if(numeric_string[index] != comparing_with.numeric_string[index]) {
                return numeric_string[index] > comparing_with.numeric_string[index];
            }
        }

        return false; // This will never be reached.
    }

    [[nodiscard]]
    bool bigint::operator<(const bigint& comparing_with) const& {
        return !(*this > comparing_with) && *this != comparing_with;
    }

    [[nodiscard]]
    bool bigint::operator>=(const bigint& comparing_with) const& {
        return !(*this < comparing_with);
    }

    [[nodiscard]]
    bool bigint::operator<=(const bigint& comparing_with) const& {
        return !(*this > comparing_with);
    }

    bigint& bigint::operator++() {
        *this += 1;
        return *this;
    }

    bigint bigint::operator++(int) {
        bigint old_value = *this;
        *this += 1;
        return old_value;
    }

    bigint& bigint::operator--() {
        *this -= 1;
        return *this;
    }

    bigint bigint::operator--(int) {
        bigint old_value = *this;
        *this -= 1;
        return old_value;
    }

    [[nodiscard]]
    bigint bigint::operator+(const bigint& with) const {
        if(with == 0)
            return *this;

        if(*this == 0)
            return with;


        if(this->is_positive() && with.is_positive()) {
            if(*this < with) {
                return with + *this;
            } else {
                std::string result;
                std::string first = this->get_numeric_string();
                std::string second = std::string(count_digits() - with.count_digits(), '0') + with.get_numeric_string();

                int decimal_overflow = 0;
                for(int i = count_digits() - 1; i >= 0; i--) {
                    int sum = to_int(first[i]) + to_int(second[i]);
                    result = std::to_string(sum % 10 + decimal_overflow) + result;
                    decimal_overflow = sum / 10;
                }

                if(decimal_overflow != 0)
                    result = (std::to_string(decimal_overflow) + result);

                return {  result };
            }
        }

        if(this->is_positive() && with.is_negative()) {
            return (*this - (with.clone().make_positive()));
        }

        // -10 + 5 = -5
        if(this->is_negative() && with.is_positive()) {
            // 5 - 10
            return with - (this->clone().make_positive());
        }

        if(this->is_negative() && with.is_negative()) {
            return -(this->clone().make_positive() + with.clone().make_positive());
        }

        throw std::logic_error("The end of operator+ should not be reached, but it did.");
    }

    [[nodiscard]]
    bigint bigint::operator-(const bigint& what) const& {
        if(*this == 0 || what == 0)
            return *this + what;

        if(this->is_positive() && what.is_positive()) {
            if(*this < what) {
                return -(what - *this);
            } else {
                std::string result;
                std::string first = this->get_numeric_string();
                std::string second = std::string(count_digits() - what.count_digits(), '0') + what.get_numeric_string();


                int decimal_underflow = 0;
                for(int i = count_digits() - 1; i >= 0; i--) {
                    int difference = to_int(first[i]) - to_int(second[i]) - decimal_underflow;

                    result = std::to_string(difference % 10 + (difference >= 0 ? 0 : 10)) + result;

                    if(difference < 0) {
                        decimal_underflow = 1;
                    } else {
                        decimal_underflow = 0;
                    }
                }

                return { result };
            }
        }

        if(this->is_positive() && what.is_negative()) {
            return (*this + what.clone().make_positive());
        }

        if(this->is_negative() && what.is_positive()) {
            return (what + this->clone().make_positive());
        }

        if(this->is_negative() && what.is_negative()) {
            return -(this->clone().make_positive() - what.clone().make_positive());
        }

        throw std::logic_error("The end of operator- should not be reached, but it did.");
    }

    [[nodiscard]]
    bigint bigint::operator*(const bigint& by) const& {
        if(by == 0 || *this == 0)
            return 0;

        bigint result = 0;

        if(by > *this) {
            return by * *this;
        } else {
            std::string first = this->get_numeric_string();
            std::string second = by.get_numeric_string();

            for(int second_number_digit_index = second.length() - 1; second_number_digit_index >= 0; second_number_digit_index--) {
                int decimal_overflow = 0;
                std::string single_multiplication_element;

                for(int first_number_digit_index = first.length() - 1; first_number_digit_index >= 0; first_number_digit_index--) {
                    int multiplication = to_int(first[first_number_digit_index]) * to_int(second[second_number_digit_index]) + decimal_overflow % 10;
                    single_multiplication_element = std::to_string(multiplication % 10) + single_multiplication_element;
                    decimal_overflow = multiplication / 10;
                }

                single_multiplication_element += std::string(((int) second.length() - 1) - second_number_digit_index, '0');
                std::cout << single_multiplication_element << '\n';

                result += bigint(single_multiplication_element);
            }
        }

        result.set_sign(this->get_sign() == by.get_sign());
        return result;
    }

    bigint& bigint::operator+=(const bigint& what) {
        return *this = *this + what;
    }

    bigint& bigint::operator-=(const bigint& what) {
        return *this = *this - what;
    }

    bigint& bigint::operator*=(const bigint& by) {
        return *this = *this * by;
    }

    bigint bigint::operator-() {
        return sign ? this->clone().make_negative() : this->clone().make_positive();
    }

    [[nodiscard]]
    bigint::operator std::string() const& {
        return to_string();
    }

    [[nodiscard]]
    bigint bigint::clone() const {
        return *this;
    }

    auto operator<<(std::ostream& stream, const bigint& integer) -> std::ostream& {
        return stream << integer.to_string();
    }

    auto operator>>(std::istream &stream, bigint &integer) -> std::istream & {
        std::string numeric_string;
        stream >> numeric_string;
        integer.set_value(numeric_string);
        return stream;
    }
}