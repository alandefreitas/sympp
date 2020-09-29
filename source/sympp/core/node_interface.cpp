//
// Created by Alan de Freitas on 02/08/19.
//

#include <sympp/core/node_interface.h>
#include <sympp/core/sym.h>

namespace sympp {

    node_interface::~node_interface() = default;

    const std::type_info &node_interface::type() const { return typeid(*this); }

    std::vector<sym>::iterator node_interface::begin() {
        return std::vector<sym>::iterator();
    }

    std::vector<sym>::iterator node_interface::end() {
        return std::vector<sym>::iterator();
    }

    std::vector<sym>::const_iterator node_interface::begin() const {
        return std::vector<sym>::const_iterator();
    }

    std::vector<sym>::const_iterator node_interface::end() const {
        return std::vector<sym>::const_iterator();
    }

    std::optional<sym> node_interface::simplify(double, complexity_lambda) {
        return std::nullopt;
    }

    std::optional<sym> node_interface::simplify(double ratio) {
        auto measure_fn = [](const node_interface &n) -> double {
            return n.count_ops();
        };
        return simplify(ratio, measure_fn);
    }

    std::optional<sym> node_interface::simplify() { return simplify(1.7); }

    std::optional<sym> node_interface::expand() { return std::nullopt; }

    std::optional<sym> node_interface::factor() { return std::nullopt; }

    std::optional<sym> node_interface::collect() { return std::nullopt; }

    std::optional<sym> node_interface::cancel() { return std::nullopt; }

    std::optional<sym> node_interface::appart() { return std::nullopt; }

    std::optional<sym> node_interface::trigsimp() { return std::nullopt; }

    std::optional<sym> node_interface::expand_trig() { return std::nullopt; }

    std::optional<sym> node_interface::powsimp() { return std::nullopt; }

    std::optional<sym> node_interface::expand_power_exp() {
        return std::nullopt;
    }

    std::optional<sym> node_interface::expand_power_base() {
        return std::nullopt;
    }

    std::optional<sym> node_interface::powdenest() { return std::nullopt; }

    std::optional<sym> node_interface::expand_log() { return std::nullopt; }

    std::optional<sym> node_interface::logcombine() { return std::nullopt; }

    bool node_interface::is_commutative() const { return false; }

    void node_interface::put_indexes(std::unordered_map<int, sym> &,
                                     std::unordered_map<int, sym> &,
                                     std::unordered_map<int, sym> &,
                                     std::unordered_map<std::string, int> &,
                                     std::unordered_map<std::string, int> &,
                                     std::unordered_map<std::string, int> &) {}

} // namespace sympp