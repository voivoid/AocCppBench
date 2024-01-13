#include "aoc/problems/2015_07.h"

#include "parse.h"

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/tuple/tuple_tie.hpp>


namespace
{
using signal = std::uint16_t;
using wire   = std::string;

using operand = boost::variant<signal, wire>;

struct gate
{
    virtual ~gate() = default;

    using circuit_processor                              = std::function<signal(const operand&)>;
    virtual signal exec(const circuit_processor& reader) = 0;
};

using gate_ptr      = std::unique_ptr<gate>;
using circuit       = std::unordered_map<wire, gate_ptr>;
using signals_cache = std::unordered_map<wire, signal>;

signal calc_operand(const operand& operand, const circuit& circuit, signals_cache& cache);

signal calc_wire(const signal signal, const circuit&, signals_cache&)
{
    return signal;
}

signal calc_wire(const wire& w, const circuit& circuit, signals_cache& cache)
{
    {
        auto cache_iter = cache.find(w);
        if (cache_iter != cache.cend()) return cache_iter->second;
    }

    auto gate_iter = circuit.find(w);
    assert(gate_iter != circuit.cend());

    const auto& gate  = gate_iter->second;
    const auto signal = gate->exec(std::bind_back(calc_operand, std::ref(circuit), std::ref(cache)));

    cache.emplace(w, signal);

    return signal;
}

signal calc_operand(const operand& operand, const circuit& circuit, signals_cache& cache)
{
    const auto visitor = [ & ](const auto& arg) -> signal { return calc_wire(arg, circuit, cache); };

    return boost::apply_visitor(visitor, operand);
}

template <auto calc>
struct unary : gate
{
    virtual signal exec(const circuit_processor& read)
    {
        return calc(read(arg1));
    }
    operand arg1;
};

template <auto calc>
struct binary : gate
{
    virtual signal exec(const circuit_processor& read)
    {
        return calc(read(arg1), read(arg2));
    }
    operand arg1, arg2;
};

// clang-format off

struct load : unary<[](const signal s) -> signal { return s; }> {};
struct not_op : unary<[](const signal s) -> signal { return ~s; }> {};
struct and_op : binary<[](const signal s1, const signal s2) -> signal { return s1 & s2; }> {};
struct or_op : binary<[](const signal s1, const signal s2) -> signal { return s1 | s2; }> {};
struct lshift : binary<[](const signal s1, const signal s2) -> signal { return s1 << s2; }> {};
struct rshift : binary<[](const signal s1, const signal s2) -> signal { return s1 >> s2; }> {};

// clang-format on

template <typename T>
struct unary_gate_creator
{
    void operator()(const auto& ctx)
    {
        T gate;
        gate.arg1 = boost::spirit::x3::_attr(ctx);

        boost::spirit::x3::_val(ctx).reset(new T(std::move(gate)));
    }
};

template <typename T>
struct binary_gate_creator
{
    void operator()(const auto& ctx)
    {
        T gate;
        boost::fusion::tie(gate.arg1, gate.arg2) = boost::spirit::x3::_attr(ctx);

        boost::spirit::x3::_val(ctx).reset(new T(std::move(gate)));
    }
};

circuit parse_circuit(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    const auto signal  = x3::uint16;
    const auto wire    = +x3::lower;
    const auto operand = x3::rule<struct _op, ::operand>{} = signal | wire;

    const auto make_binary_gate_parser = [ & ](const auto name) { return operand >> name > operand; };
    const auto make_unary_gate_parser  = [ & ](const auto name) { return name >> operand; };

    const auto not_   = make_unary_gate_parser(x3::lit("NOT"))[ unary_gate_creator<not_op>{} ];
    const auto load   = make_unary_gate_parser(x3::eps)[ unary_gate_creator<::load>{} ];
    const auto and_   = make_binary_gate_parser(x3::lit("AND"))[ binary_gate_creator<and_op>{} ];
    const auto or_    = make_binary_gate_parser(x3::lit("OR"))[ binary_gate_creator<or_op>{} ];
    const auto lshift = make_binary_gate_parser(x3::lit("LSHIFT"))[ binary_gate_creator<::lshift>{} ];
    const auto rshift = make_binary_gate_parser(x3::lit("RSHIFT"))[ binary_gate_creator<::rshift>{} ];

    const auto gate_parser = x3::rule<struct _gate, gate_ptr>{} = and_ | or_ | lshift | rshift | not_ | load;

    const auto parser = gate_parser > x3::lit("->") > wire;

    circuit map;
    for (auto& [ gate, destination ] : aoc::parse_lines<std::tuple<gate_ptr, ::wire>>(input, parser))
    {
        map.emplace(std::move(destination), std::move(gate));
    }

    return map;
}

}  // namespace

namespace aoc
{
namespace year_2015
{
namespace details
{
size_t solve07_a(std::istream& input, const char* wire)
{
    const auto circuit = parse_circuit(input);

    signals_cache signals_cache;
    return calc_wire(wire, circuit, signals_cache);
}
}  // namespace details

size_t solve07_a(std::istream& input)
{
    return details::solve07_a(input, "a");
}

size_t solve07_b(std::istream& input)
{
    const auto circuit = parse_circuit(input);
    signals_cache signals_cache;
    const auto a_signal = calc_wire("a", circuit, signals_cache);

    signals_cache.clear();
    signals_cache[ "b" ] = a_signal;
    return calc_wire("a", circuit, signals_cache);
}

}  // namespace year_2015
}  // namespace aoc
