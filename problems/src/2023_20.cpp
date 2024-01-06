#include "aoc/problems/2023_20.h"

#include "ensure.h"
#include "parse.h"

#include <istream>
#include <memory>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/integer/common_factor_rt.hpp>

namespace
{
enum pulse_level
{
    none = 0,
    low,
    high
};

struct module;
struct signal
{
    pulse_level level;
    module* from        = nullptr;
    module* destination = nullptr;
};

struct module
{
    module(std::string name, std::vector<module*> destinations) :m_name(std::move(name)),
        m_destination_modules(std::move(destinations))
    {
    }

    virtual ~module() = default;

    virtual pulse_level process_pulse(module* from, pulse_level) = 0;
    virtual void initial_input_module_notification(module& /*input*/){};

    const std::string& get_name() const
    {
        return m_name;
    }

    const std::vector<module*>& get_destination_modules() const
    {
        return m_destination_modules;
    }

    auto make_signals(const pulse_level level)
    {
        assert(level != pulse_level::none);

        return m_destination_modules | std::ranges::views::transform(
                                           [ level, this ](module* m) {
                                               return signal{ level, this, m };
                                           });
    }

    virtual bool is_flip_flop() const
    {
        return false;
    }

  private:
    std::vector<module*> m_destination_modules;
    std::string m_name;
};

struct module_deleter
{
    // since the graph of modules has cycles, some raw memory tricks ( separate allocation/construction ) are used to
    // create the modules map amidst the single-pass parsing process
    void operator()(module* m)
    {
        m->~module();
        ::operator delete(m);
    }
};

using owning_modules = std::vector<std::unique_ptr<module, module_deleter>>;

struct flip_flop : module
{
    using module::module;

  private:
    virtual pulse_level process_pulse(module*, const pulse_level level) override
    {
        if (level == pulse_level::high) return pulse_level::none;

        is_on = !is_on;

        return is_on ? pulse_level::high : pulse_level::low;
    }

    virtual bool is_flip_flop() const override
    {
        return true;
    }

  private:
    bool is_on = false;
};

struct conjunction : module
{
    using module::module;

  private:
    virtual pulse_level process_pulse(module* from, const pulse_level level) override
    {
        const std::string& source = from->get_name();

        pulse_history[ source ] = level;

        return are_all_high() ? pulse_level::low : pulse_level::high;
    }

    bool are_all_high() const
    {
        return std::ranges::all_of(pulse_history | std::ranges::views::values,
                                   [](const pulse_level p) { return p == pulse_level::high; });
    }

    virtual void initial_input_module_notification(module& input) override
    {
        pulse_history.emplace(input.get_name(), pulse_level::low);
    }

  private:
    std::unordered_map<std::string, pulse_level> pulse_history;
};

struct broadcaster : module
{
    using module::module;

    static inline const char* const name = "broadcaster";

  private:
    virtual pulse_level process_pulse(module*, const pulse_level level) override
    {
        assert(level == pulse_level::low);
        return level;
    }
};

struct module_creation_info
{
    module* ptr = nullptr;
    std::move_only_function<void(module*, std::vector<module*>&&)> ctor;
    std::vector<std::string> destination_names;
};

using modules_map = std::unordered_map<std::string, module_creation_info>;

template <typename T>
struct module_creator
{
    module_creator(modules_map& m) : map(m) {}

    void operator()(const auto& ctx)
    {
        auto& attr         = boost::spirit::x3::_attr(ctx);
        auto& module_name  = boost::fusion::get<0>(attr);
        auto& destinations = boost::fusion::get<1>(attr);

        std::string key = module_name;

        // since the graph of modules has cycles, some raw memory tricks ( separate allocation/construction ) are used
        // here to create the modules map amidst the single-pass parsing process
        module* module_raw_memory = reinterpret_cast<module*>(operator new(sizeof(T)));
        map.emplace(std::move(key),
                    module_creation_info(
                        module_raw_memory,
                        [ name = std::move(module_name) ](module* module_raw_memory,
                                                          std::vector<module*>&& destinations) mutable
                        { new (module_raw_memory) T(std::move(name), std::move(destinations)); },
                        std::move(destinations)));
    }

  private:
    modules_map& map;
};

owning_modules parse_modules(std::istream& input)
{
    modules_map modules_map;

    {
        namespace x3 = boost::spirit::x3;

        const auto module_name  = x3::lexeme[ *x3::lower ];
        const auto destinations = x3::lit("->") > (module_name % ',');

        const auto broadcaster_parser =
            (x3::string(broadcaster::name) > destinations)[ module_creator<broadcaster>{ modules_map } ];
        const auto flipflop_parser =
            (x3::lit('%') > module_name > destinations)[ module_creator<flip_flop>{ modules_map } ];
        const auto conj_parser =
            (x3::lit('&') > module_name > destinations)[ module_creator<conjunction>{ modules_map } ];

        const auto modules_parser = +(flipflop_parser | conj_parser | broadcaster_parser);

        const bool parsed = aoc::x3_parse(input, modules_parser, x3::space);
        ensure(parsed);
    }

    owning_modules modules;
    modules.reserve(modules_map.size());

    for (auto& [ module_name, module_info ] : modules_map)
    {
        std::vector<module*> destination_modules;
        for (const std::string& dest_name : module_info.destination_names)
        {
            const auto dest_iter = modules_map.find(dest_name);
            if (dest_iter != modules_map.cend()) { destination_modules.push_back(dest_iter->second.ptr); }
            else { destination_modules.push_back(nullptr); /* output module */ }
        }

        module_info.ctor(module_info.ptr, std::move(destination_modules));

        modules.emplace_back(module_info.ptr);
    }

    for (const auto& module : modules)
    {
        for (::module* destination : module->get_destination_modules())
        {
            if (destination != nullptr)
                destination->initial_input_module_notification(*module);  // output destination is nullptr
        }
    }

    return modules;
}

module& find_broadcaster(const owning_modules& modules)
{
    auto broadcaster_iter = std::ranges::find(modules, broadcaster::name, &module::get_name);
    assert(broadcaster_iter != modules.cend());

    return *broadcaster_iter->get();
}

struct processor
{
    processor(owning_modules modules) : m_modules(std::move(modules)), m_broadcaster(&find_broadcaster(m_modules)) {}

    bool handle_push_button(const bool wait_for_low_output = false)
    {
        assert(signal_queue.empty());
        signal_queue.emplace(pulse_level::low, nullptr, m_broadcaster);

        while (!signal_queue.empty())
        {
            const auto signal = signal_queue.front();
            signal_queue.pop();

            if (signal.level == pulse_level::low) ++low_pulses;
            else
                ++high_pulses;

            if (signal.destination == nullptr)  // output?
            {
                if (signal.level == pulse_level::low && wait_for_low_output) return true;

                continue;
            }

            const pulse_level next_pulse_level = signal.destination->process_pulse(signal.from, signal.level);
            if (next_pulse_level != pulse_level::none)
            {
                signal_queue.push_range(signal.destination->make_signals(next_pulse_level));
            }
        }

        return false;
    }

    owning_modules m_modules;
    std::queue<signal> signal_queue;
    module* m_broadcaster = nullptr;
    size_t high_pulses    = 0;
    size_t low_pulses     = 0;
};

size_t bits_to_number(const std::vector<bool>& bits)
{
    return std::ranges::fold_right(bits, size_t(0), [](size_t bit, size_t acc) { return (acc << 1) + bit; });
}

}  // namespace

namespace aoc
{
namespace year_2023
{

size_t solve20_a(std::istream& input)
{
    processor processor(parse_modules(input));
    size_t button_pushes = 1000;

    while (button_pushes--) { processor.handle_push_button(); }

    return processor.high_pulses * processor.low_pulses;
}

size_t solve20_b(std::istream& input)
{
    const auto modules        = parse_modules(input);
    const module& broadcaster = find_broadcaster(modules);

    std::vector<size_t> conj_lcms;
    for (const module* broadcaster_dest : broadcaster.get_destination_modules())
    {
        const module* module = broadcaster_dest;

        std::vector<bool> flipflop_bits;
        for (;;)
        {
            const auto& dests     = module->get_destination_modules();
            const auto dests_size = dests.size();

            if (dests_size == 1 && dests[ 0 ]->is_flip_flop())
            {
                flipflop_bits.push_back(0);
                module = dests[ 0 ];
            }
            else if (dests_size == 2 && dests[ 0 ]->is_flip_flop())
            {
                flipflop_bits.push_back(1);
                module = dests[ 0 ];
            }
            else if (dests_size == 2 && dests[ 1 ]->is_flip_flop())
            {
                flipflop_bits.push_back(1);
                module = dests[ 1 ];
            }
            else if (dests_size == 1 && !dests[ 0 ]->is_flip_flop())
            {
                flipflop_bits.push_back(1);
                break;
            }
            else
                std::unreachable();
        }

        conj_lcms.push_back(bits_to_number(flipflop_bits));
    }

    const auto button_pushes = boost::integer::lcm_range(conj_lcms.cbegin(), conj_lcms.cend()).first;
    return button_pushes;
}

}  // namespace year_2023
}  // namespace aoc
