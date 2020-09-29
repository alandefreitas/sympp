#include <sympp/sympp.h>

int main() {
    using namespace sympp;
    using std::cout, std::endl;
    // Constants
    sym A("A", 10);
    sym n("n", 3);
    sym pi = constant::pi();

    // Variables
    sym x1("x_1");
    sym x2("x_2");
    sym x3("x_3");

    // Function terms
    sym begin = A * n;
    sym term1 = sympp::pow(x1, sym(2)) - A * sympp::cos(2 * pi * x1);
    sym term2 = power(x2, sym(2)) - A * cosine(2 * pi * x2);
    sym term3 = power(x3, sym(2)) - A * cosine(2 * pi * x3);

    // Function
    sym rastrigin = begin + term1 + term2 + term3;

    cout << rastrigin << endl;
    return 0;
}