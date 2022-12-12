#include "aoc/problems/2022_07.h"

#include "parse.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "boost/fusion/adapted/struct.hpp"

namespace
{

struct fs_info
{
    size_t size;
    std::string name;

    bool is_dir() const
    {
        return size == 0;
    }
};

struct fs_entry
{
    fs_entry& add_child(const std::string& name, const size_t size)
    {
        auto child_iter = children.find(name);
        if (child_iter == children.cend())
        {
            fs_entry fs;
            fs.info.name = name;
            fs.info.size = size;
            fs.parent    = this;

            child_iter = children.emplace(name, std::move(fs)).first;
        }

        return child_iter->second;
    }

    fs_entry& add_child_dir(const std::string& name)
    {
        return add_child(name, 0);
    }

    fs_entry& add_child(const fs_info& fi)
    {
        return add_child(fi.name, fi.size);
    }

    fs_info info;
    fs_entry* parent = nullptr;
    std::unordered_map<std::string, fs_entry> children;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT(fs_info, size, name)

namespace
{

fs_entry parse(std::istream& input)
{
    static const char* const root_dir = "/";
    static const char* const prev_dir = "..";

    fs_entry root;
    root.info.size    = 0;
    root.info.name    = "/";
    fs_entry* current = &root;

    namespace x3 = boost::spirit::x3;

    const auto cd_action = [ &root, &current ](auto& ctx)
    {
        const std::string& dir_name = x3::_attr(ctx);

        if (dir_name == root_dir)
            current = &root;
        else if (dir_name == prev_dir)
        {
            assert(current->parent);
            current = current->parent;
        }
        else
        {
            current = &current->add_child_dir(dir_name);
        }
    };

    const auto ls_action = [ &current ](auto& ctx)
    {
        const auto& files = x3::_attr(ctx);
        for (const fs_info& fi : files)
        {
            current->add_child(fi);
        }
    };

    const auto name_parser      = x3::lexeme[ +(x3::alnum) ];
    const auto dir_name_parser  = name_parser;
    const auto file_name_parser = x3::rule<struct _name, std::string>{} = name_parser >> -(x3::char_('.') > name_parser);

    const auto prompt_parser = x3::lit('$');
    const auto cd_parser     = x3::rule<struct _cd, std::string>{} =
        prompt_parser >> x3::lit("cd") > (dir_name_parser | x3::string(prev_dir) | x3::string(root_dir));
    const auto dir_parser     = x3::lit("dir") > x3::attr(size_t(0)) > dir_name_parser;
    const auto file_parser    = aoc::x3_size_t_ > file_name_parser;
    const auto fs_info_parser = x3::rule<struct _fileinfo, fs_info>{} = dir_parser | file_parser;
    const auto ls_parser                                              = prompt_parser >> x3::lit("ls") > (*fs_info_parser);

    const auto command_parser = cd_parser[ cd_action ] | ls_parser[ ls_action ];

    const auto parser = +command_parser;

    bool is_parsed = aoc::x3_parse(input, parser, x3::space);
    if (!is_parsed)
        throw std::runtime_error("failed to parse input");

    return root;
}

size_t calc_directory_sizes(const fs_entry& current_dir, std::vector<size_t>& sizes)
{
    size_t current_dir_size = 0;
    for (const auto& [ _, child_fs ] : current_dir.children)
    {
        if (child_fs.info.is_dir())
            current_dir_size += calc_directory_sizes(child_fs, sizes);
        else
            current_dir_size += child_fs.info.size;
    }

    sizes.push_back(current_dir_size);

    return current_dir_size;
}

}  // namespace

namespace aoc
{
namespace year_2022
{

size_t solve07_a(std::istream& input)
{
    const auto root_fs = parse(input);

    std::vector<size_t> all_dir_sizes;
    calc_directory_sizes(root_fs, all_dir_sizes);

    static constexpr size_t max_dir_size_to_search = 100000;
    auto less_than_max_sizes = all_dir_sizes | std::views::filter([](const auto size) { return size <= max_dir_size_to_search; });

    return std::accumulate(less_than_max_sizes.begin(), less_than_max_sizes.end(), size_t(0));
}

size_t solve07_b(std::istream& input)
{
    const auto root_fs = parse(input);

    std::vector<size_t> all_dir_sizes;
    const size_t total_occupied = calc_directory_sizes(root_fs, all_dir_sizes);

    static constexpr size_t total_fs     = 70000000;
    static constexpr size_t space_needed = 30000000;
    assert(total_occupied <= total_fs);
    const auto current_free = total_fs - total_occupied;
    assert(current_free <= space_needed);
    const auto should_free = space_needed - current_free;

    auto big_enough_sizes = all_dir_sizes | std::views::filter([ should_free ](const auto size) { return size >= should_free; });

    return std::ranges::min(big_enough_sizes);
}

}  // namespace year_2022
}  // namespace aoc