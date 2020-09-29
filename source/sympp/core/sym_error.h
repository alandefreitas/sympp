// symerror.h

#ifndef SYMPP_ERRORS
#define SYMPP_ERRORS

#include <stdexcept>
#include <string>

namespace sympp {
    class sym_error : public std::runtime_error {
      public:
        enum error : unsigned short {
            IncompatibleNumeric,
            IncompatibleVector,
            NoMatch,
            NotDouble,
            NotInt,
            NotMatrix,
            NotNumeric,
            NotVector,
            Unsupportednumeric,
            DivideByZero,
            AbstractClass
        };

      public:
        explicit sym_error(const error &e);

        [[nodiscard]] std::string message() const;

        [[nodiscard]] unsigned short code() const;

        [[nodiscard]] error error_number() const;

      private:
        error error_number_;
    };
} // namespace sympp

#endif
