#include "aoc/problems/2015_04.h"

#include "ensure.h"

#include <cassert>
#include <functional>
#include <istream>
#include <ranges>
#include <string>

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
    for (auto r_iter = num_str.rbegin(); r_iter != num_str.rend(); ++r_iter)
    {
        if (!std::isdigit(*r_iter))
        {
            num_str.insert(r_iter.base(), '1');
            return;
        }

        *r_iter = next_char(*r_iter);

        if (*r_iter != '0') return;
    }
}

template <size_t leading_zeroes>
size_t solve(std::istream& input)
{
    std::string secret_key;
    input >> secret_key;

    for (const size_t n : std::ranges::views::iota(0))
    {
        if (check_hash_has_leading_zeroes<leading_zeroes>(secret_key)) { return n; }

        inc_num_str(secret_key);
    }

    std::unreachable();
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
