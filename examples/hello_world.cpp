#include <sympp/sympp.h>

int main() {
    using namespace sympp;
    using std::cout, std::endl;
    sym a("Hello");
    sym b("World");
    sym c = a + b;
    cout << c << endl;
    return 0;
}