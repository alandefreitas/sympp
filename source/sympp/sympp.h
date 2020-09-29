#ifndef SYMPP_H
#define SYMPP_H

// Main library objects
#include <sympp/core/internal_node_interface.h>
#include <sympp/core/node_interface.h>
#include <sympp/core/sym.h>
#include <sympp/core/sym_error.h>
#include <sympp/core/terminal_node_interface.h>

// Nodes that represent a symbol
#include <sympp/node/terminal/boolean.h>
#include <sympp/node/terminal/constant.h>
#include <sympp/node/terminal/integer.h>
#include <sympp/node/terminal/number_interface.h>
#include <sympp/node/terminal/rational.h>
#include <sympp/node/terminal/real.h>
#include <sympp/node/terminal/variable.h>

#include <sympp/node/operation/product.h>
#include <sympp/node/operation/summation.h>

#include <sympp/node/function/abs.h>
#include <sympp/node/function/cos.h>
#include <sympp/node/function/cosh.h>
#include <sympp/node/function/function_interface.h>
#include <sympp/node/function/log.h>
#include <sympp/node/function/pow.h>
#include <sympp/node/function/sin.h>
#include <sympp/node/function/sinh.h>

#include <sympp/node/statement/statement.h>

// Free-standing functions to manipulate a sym
#include <sympp/functions/mathematics.h>
#include <sympp/functions/operators.h>
#include <sympp/functions/symbolic.h>

#endif // SYMPP_H
