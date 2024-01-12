#include "aoc/problems/2015_04.h"

#include <execution>
#include <functional>
#include <istream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#define BOOST_UUID_COMPAT_PRE_1_71_MD5
#include "boost/uuid/detail/md5.hpp"

namespace
{
union md5_digest
{
    boost::uuids::detail::md5::digest_type digest;

    using digest_bytes = unsigned char[ sizeof(digest) ];
    digest_bytes bytes;
};

template <size_t leading_zeroes>
bool check_hash_has_leading_zeroes(const std::string& secret_key)
{
    boost::uuids::detail::md5 md5;
    md5.process_bytes(secret_key.data(), secret_key.size());

    md5_digest digest;
    md5.get_digest(digest.digest);

    switch (leading_zeroes)
    {
        case 2: return digest.bytes[ 0 ] == 0;
        case 5: return digest.bytes[ 0 ] == 0 && digest.bytes[ 1 ] == 0 && (digest.bytes[ 2 ] & 0xf0) == 0;
        case 6: return digest.bytes[ 0 ] == 0 && digest.bytes[ 1 ] == 0 && digest.bytes[ 2 ] == 0;
    }

    std::unreachable();
}

char next_char(const char c)
{
    if (c == '9') return '0';
    return c + 1;
}

void inc_num_str(std::string& num_str)
{
    for (size_t i = num_str.size() - 1; i > 0; --i)
    {
        char& c = num_str[ i ];
        if (!std::isdigit(c))
        {
            num_str.insert(num_str.cbegin() + i + 1, '1');
            return;
        }

        c = next_char(c);

        if (c != '0') return;
    }
}

template <size_t leading_zeroes>
std::optional<size_t> try_find_hash(const size_t from, std::string secret_key, const size_t nums_to_check)
{
    const size_t to = from + nums_to_check;

    secret_key += std::to_string(from);
    for (size_t n = from; n < to; ++n)
    {
        if (check_hash_has_leading_zeroes<leading_zeroes>(secret_key)) { return n; }
        inc_num_str(secret_key);
    }

    return {};
}

template <size_t leading_zeroes>
size_t solve(std::istream& input)
{
    std::string secret_key;
    input >> secret_key;

    size_t block            = 0;
    const size_t block_size = 4000;
    std::vector<size_t> blocks(std::thread::hardware_concurrency());

    for (;;)
    {
        std::ranges::generate(blocks,
                              [ &block ]()
                              {
                                  const auto from = block++ * block_size;
                                  return from;
                              });

        static constexpr auto reduce = [](const std::optional<size_t>& res1, const std::optional<size_t>& res2)
        {
            if (!res1) return res2;
            if (!res2) return res1;

            return std::optional<size_t>{ std::min(*res1, *res2) };
        };

        const auto min_num =
            std::transform_reduce(std::execution::par_unseq,
                                  blocks.cbegin(),
                                  blocks.cend(),
                                  std::optional<size_t>{},
                                  reduce,
                                  std::bind_back(try_find_hash<leading_zeroes>, std::ref(secret_key), block_size));

        if (min_num) { return *min_num; }
    }
}
}  // namespace

namespace aoc
{
namespace year_2015
{

size_t solve04_a(std::istream& input)
{
    return solve<5>(input);
}

size_t solve04_b(std::istream& input)
{
    return solve<6>(input);
}

size_t solve04_test(std::istream& input)
{
    return solve<2>(input);
}

}  // namespace year_2015
}  // namespace aoc
