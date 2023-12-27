#pragma once

#include <istream>
#include <ranges>

#include <boost/circular_buffer.hpp>

namespace aoc
{

template <typename T, size_t buffer_size>
class istream_buffered_view
{
  public:
    istream_buffered_view() = default;
    istream_buffered_view(std::istream& s) : m_stream(&s), m_buffer(buffer_size)
    {
        T elem{};
        for (size_t i = 0; s && i < buffer_size; ++i)
        {
            (*m_stream) >> elem;
            m_buffer.push_back(elem);
        }
    }

    class istream_buffered_iterator
    {
      public:
        using value_type        = T;
        using reference         = T&;
        using pointer           = T*;
        using difference_type   = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using iterator_concept  = std::forward_iterator_tag;

        istream_buffered_iterator() = default;
        istream_buffered_iterator(const istream_buffered_view& view) : m_view(&view) {}

        const T& operator*() const noexcept
        {
            return m_view->get_elem(m_idx);
        }

        istream_buffered_iterator& operator++() noexcept
        {
            ++m_idx;
            m_view->read_elems(m_idx);
            return *this;
        }

        istream_buffered_iterator operator++(int) noexcept
        {
            istream_buffered_iterator iter(*this);
            ++m_idx;
            m_view->read_elems(m_idx);
            return std::move(iter);
        }

        friend bool operator==(const istream_buffered_iterator& lhs, const istream_buffered_iterator& rhs) noexcept
        {
            if (rhs.m_view == nullptr) return lhs.is_last();

            return lhs.m_view == rhs.m_view && lhs.m_idx == rhs.m_idx;
        }

      private:
        bool is_last() const
        {
            return m_view->is_last(m_idx);
        }

      private:
        size_t m_idx = 0;
        const istream_buffered_view* m_view;
    };

    istream_buffered_iterator begin() const
    {
        return { *this };
    }
    istream_buffered_iterator end() const
    {
        return {};
    }

  private:
    const auto& get_elem(const size_t idx) const
    {
        assert(idx >= m_idx);
        return m_buffer[ idx - m_idx ];
    }

    void read_elems(size_t idx) const
    {
        const auto d = idx - m_idx + 1;
        if (d > buffer_size)
        {
            auto n = d - buffer_size;

            T t;
            while (n--)
            {
                (*m_stream) >> t;
                m_buffer.push_back(t);
            }

            m_idx += (d - buffer_size);

            if (!*m_stream) { m_last = m_idx + buffer_size - 1; }
        }
    }

    bool is_last(const size_t n) const
    {
        return n == m_last;
    }

  private:
    std::istream* m_stream;
    mutable size_t m_idx  = 0;
    mutable size_t m_last = size_t(-1);
    mutable boost::circular_buffer<T> m_buffer;
};

}  // namespace aoc