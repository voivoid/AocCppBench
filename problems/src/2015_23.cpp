#include "aoc/problems/2015_23.h"

#include "ensure.h"
#include "parse.h"

#include <memory>
#include <vector>

namespace
{
struct state
{
    size_t a, b;
    int ip;
};

using registr = size_t state::*;

class instruction
{
  public:
    virtual ~instruction() = default;

    void run(state& state) const
    {
        exec(state);
        next(state);
    }

  protected:
    virtual void exec(state& state) const = 0;
    virtual void next(state& state) const
    {
        ++state.ip;
    }
};

using instruction_ptr = std::unique_ptr<instruction>;

using instructions = std::vector<instruction_ptr>;

class hlf : public instruction
{
  public:
    hlf(registr r) : reg(r) {}

  private:
    virtual void exec(state& state) const override
    {
        (state.*reg) /= 2;
    }

    registr reg;
};

class tpl : public instruction
{
  public:
    tpl(registr r) : reg(r) {}

  private:
    virtual void exec(state& state) const override
    {
        (state.*reg) *= 3;
    }

    registr reg;
};

class inc : public instruction
{
  public:
    inc(registr r) : reg(r) {}

  private:
    virtual void exec(state& state) const override
    {
        ++(state.*reg);
    }

    registr reg;
};

class jmp : public instruction
{
  public:
    jmp(int off) : offset(off) {}

  private:
    virtual void exec(state&) const override {}
    virtual void next(state& state) const override
    {
        state.ip += offset;
    }

    int offset;
};

class jie : public instruction
{
  public:
    jie(registr r, int off) : reg(r), offset(off) {}

  private:
    virtual void exec(state&) const override {}
    virtual void next(state& state) const override
    {
        state.ip += (state.*reg % 2 == 0) ? offset : 1;
    }

    registr reg;
    int offset;
};

class jio : public instruction
{
  public:
    jio(registr r, int off) : reg(r), offset(off) {}

  private:
    virtual void exec(state&) const override {}
    virtual void next(state& state) const override
    {
        state.ip += (state.*reg == 1) ? offset : 1;
    }

    registr reg;
    int offset;
};

}  // namespace

namespace
{

template <typename T>
struct create_unary_instruction
{
    void operator()(const auto& ctx)
    {
        auto arg1 = boost::spirit::x3::_attr(ctx);
        boost::spirit::x3::_val(ctx).reset(new T(arg1));
    }
};

template <typename T>
struct create_binary_instruction
{
    void operator()(const auto& ctx)
    {
        const auto& attr = boost::spirit::x3::_attr(ctx);
        auto arg1        = boost::fusion::at_c<0>(attr);
        auto arg2        = boost::fusion::at_c<1>(attr);
        boost::spirit::x3::_val(ctx).reset(new T(arg1, arg2));
    }
};

instructions parse_instructions(std::istream& input)
{
    namespace x3 = boost::spirit::x3;

    x3::symbols<size_t state::*> reg{ { "a", &state::a }, { "b", &state::b } };

    const auto offset = x3::int_;

#define AOC_2015_23_DEFINE_UNARY_PARSER(instr, arg1)                                                                   \
    const auto instr = x3::rule<struct _instr, instruction_ptr>{} =                                                    \
        (x3::lit(#instr) > arg1)[ create_unary_instruction<::instr>{} ];

#define AOC_2015_23_DEFINE_BINARY_PARSER(instr, arg1, arg2)                                                            \
    const auto instr = x3::rule<struct _instr, instruction_ptr>{} =                                                    \
        (x3::lit(#instr) > arg1 > ',' > arg2)[ create_binary_instruction<::instr>{} ];

    AOC_2015_23_DEFINE_UNARY_PARSER(hlf, reg);
    AOC_2015_23_DEFINE_UNARY_PARSER(tpl, reg);
    AOC_2015_23_DEFINE_UNARY_PARSER(inc, reg);
    AOC_2015_23_DEFINE_UNARY_PARSER(jmp, offset);
    AOC_2015_23_DEFINE_BINARY_PARSER(jie, reg, offset);
    AOC_2015_23_DEFINE_BINARY_PARSER(jio, reg, offset);

    const auto instrs = *(hlf | tpl | inc | jmp | jie | jio);

    auto instructions = aoc::x3_parse_attr<::instructions>(input, instrs, x3::space);
    ensure(instructions);

    return std::move(*instructions);


    //
    //    AOC_2015_23_DEFINE_INSTR_PARSER(hlf) > reg;
    //    AOC_2015_23_DEFINE_INSTR_PARSER(tpl) > reg;
    //    AOC_2015_23_DEFINE_INSTR_PARSER(inc) > reg;
    //    AOC_2015_23_DEFINE_INSTR_PARSER(jmp) > offset;
    //    AOC_2015_23_DEFINE_INSTR_PARSER(jie) > reg > offset;
    //    AOC_2015_23_DEFINE_INSTR_PARSER(jio) > reg > offset;
    //
    //    auto instr = hlf | tpl | inc | jmp | jie | jio;
    //    auto instructions = *instr;
}

state run_instructions(const instructions& instructions, state state)
{
    const auto sz = instructions.size();

    while (state.ip >= 0 && state.ip < sz)
    {
        const auto& instruction = instructions[ state.ip ];
        instruction->run(state);
    }

    return state;
}

size_t solve(std::istream& input, size_t initial_A_reg_value)
{
    instructions instructions = parse_instructions(input);

    state initial_state{ .a = initial_A_reg_value };
    const state final_state = run_instructions(instructions, initial_state);

    return final_state.b;
}

}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve23_a(std::istream& input)
{
    return solve(input, 0);
}

size_t solve23_b(std::istream& input)
{
    return solve(input, 1);
}

}  // namespace year_2015
}  // namespace aoc
