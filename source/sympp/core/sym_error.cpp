#include <string>
#include <sympp/core/sym_error.h>

namespace sympp {
    sym_error::sym_error(const error &e)
        : std::runtime_error(this->message()), error_number_(e) {}

    std::string sym_error::message() const {
        switch (error_number_) {
        case error::IncompatibleNumeric:
            return "sympp::sym_error: Tried to use two incompatible number "
                   "together. (error code " +
                   std::to_string(code()) + ")";
        case error::IncompatibleVector:
            return "sympp::sym_error: Tried to use two incompatible types in a "
                   "vector operation. (error code " +
                   std::to_string(code()) + ")";
        case error::NoMatch:
            return "sympp::sym_error: No match found. (error code " +
                   std::to_string(code()) + ")";
        case error::NotDouble:
            return "sympp::sym_error: The value is not of type double. (error "
                   "code " +
                   std::to_string(code()) + ")";
        case error::NotInt:
            return "sympp::sym_error: The value is not of type int. (error "
                   "code " +
                   std::to_string(code()) + ")";
        case error::NotMatrix:
            return "sympp::sym_error: The value is not a matrix. (error code " +
                   std::to_string(code()) + ")";
        case error::NotNumeric:
            return "sympp::sym_error: The value is not numeric. (error code " +
                   std::to_string(code()) + ")";
        case error::NotVector:
            return "sympp::sym_error: The value is not a vector. (error code " +
                   std::to_string(code()) + ")";
        case error::Unsupportednumeric:
            return "sympp::sym_error: The data type is not supprted by "
                   "numeric. (error code " +
                   std::to_string(code()) + ")";
        default:
            return "sympp::sym_error: Unknown error. (error code " +
                   std::to_string(code()) + ")";
        }
    }

    unsigned short sym_error::code() const {
        return static_cast<short int>(error_number());
    }

    sym_error::error sym_error::error_number() const { return error_number_; }

} // namespace sympp