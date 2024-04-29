// fxstring.h --- fixed-width string class for C++11
// License: MIT

#pragma once

#include <string>           // For std::basic_string, std::string, std::wstring, ...
#include <initializer_list> // For std::initializer_list
#include <stdexcept>        // For std::out_of_range, ...
#include <cassert>          // For assert
#include <cstddef>          // For std::ptrdiff_t
#include <cstdio>           // For printf, ...
#include <cstdarg>          // For va_list, va_start, va_end etc.
#include <iterator>         // For std::iterator_traits

namespace khmz
{
    using size_t = std::size_t;

    namespace detail
    {
        template <typename T_VALUE>
        inline T_VALUE _min(T_VALUE value1, T_VALUE value2)
        {
            return (value1 < value2) ? value1 : value2;
        }
    }

    template <typename T_CHAR, size_t t_buf_size, typename T_CHAR_TRAITS = std::char_traits<T_CHAR>>
    class fxstring
    {
    public:
        static_assert(t_buf_size > 0, "template parameter `t_buf_size` must be greater than zero");

        //
        // Types
        //
        using self_type = fxstring<T_CHAR, t_buf_size, T_CHAR_TRAITS>;
        using value_type = T_CHAR;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type *;
        using values_type = value_type[t_buf_size];
        using iterator_category = std::random_access_iterator_tag;
        using traits_type = T_CHAR_TRAITS;

        //
        // Iterators
        // See: https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
        //
        struct iterator : public std::iterator_traits<value_type*>
        {
            using self_type = iterator;

            iterator(value_type *ptr = nullptr) : m_ptr(ptr)
            {
            }

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            self_type& operator++() { ++m_ptr; return *this; }
            self_type& operator--() { --m_ptr; return *this; }
            self_type operator++(int)
            {
                self_type tmp = *this;
                ++(*this);
                return tmp;
            }
            self_type operator--(int)
            {
                self_type tmp = *this;
                --(*this);
                return tmp;
            }
            self_type& operator+=(difference_type offset)
            {
                m_ptr += offset;
                return *this;
            }
            self_type& operator-=(difference_type offset)
            {
                m_ptr -= offset;
                return *this;
            }

            friend bool operator==(const self_type& a, const self_type& b)
            {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const self_type& a, const self_type& b)
            {
                return a.m_ptr != b.m_ptr;
            }
            friend bool operator<(const self_type& a, const self_type& b)
            {
                return a.m_ptr < b.m_ptr;
            }
            friend bool operator>(const self_type& a, const self_type& b)
            {
                return a.m_ptr > b.m_ptr;
            }
            friend bool operator<=(const self_type& a, const self_type& b)
            {
                return a.m_ptr <= b.m_ptr;
            }
            friend bool operator>=(const self_type& a, const self_type& b)
            {
                return a.m_ptr >= b.m_ptr;
            }

            friend self_type operator+(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp += index;
                return tmp;
            }
            friend self_type operator-(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp -= index;
                return tmp;
            }
            friend size_type operator-(const self_type& a, const self_type& b)
            {
                return a.m_ptr - b.m_ptr;
            }

        protected:
            value_type *m_ptr;

            friend class khmz::fxstring<T_CHAR, t_buf_size>;
            friend struct const_iterator;
        };

        struct const_iterator : public std::iterator_traits<const value_type*>
        {
            using self_type = const_iterator;

            const_iterator(const value_type *ptr = nullptr) : m_ptr(ptr)
            {
            }
            const_iterator(iterator it) : m_ptr(it.m_ptr)
            {
            }
            self_type& operator=(iterator it)
            {
                m_ptr = it.m_ptr;
                return *this;
            }

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            self_type& operator++() { ++m_ptr; return *this; }
            self_type& operator--() { --m_ptr; return *this; }
            self_type operator++(int)
            {
                self_type tmp = *this;
                ++(*this);
                return tmp;
            }
            self_type operator--(int)
            {
                self_type tmp = *this;
                --(*this);
                return tmp;
            }
            self_type& operator+=(difference_type offset)
            {
                m_ptr += offset;
                return *this;
            }
            self_type& operator-=(difference_type offset)
            {
                m_ptr -= offset;
                return *this;
            }

            friend bool operator==(const self_type& a, const self_type& b)
            {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const self_type& a, const self_type& b)
            {
                return a.m_ptr != b.m_ptr;
            }
            friend bool operator<(const self_type& a, const self_type& b)
            {
                return a.m_ptr < b.m_ptr;
            }
            friend bool operator>(const self_type& a, const self_type& b)
            {
                return a.m_ptr > b.m_ptr;
            }
            friend bool operator<=(const self_type& a, const self_type& b)
            {
                return a.m_ptr <= b.m_ptr;
            }
            friend bool operator>=(const self_type& a, const self_type& b)
            {
                return a.m_ptr >= b.m_ptr;
            }

            friend self_type operator+(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp += index;
                return tmp;
            }
            friend self_type operator-(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp -= index;
                return tmp;
            }
            friend size_type operator-(const self_type& a, const self_type& b)
            {
                return a.m_ptr - b.m_ptr;
            }

        protected:
            const value_type *m_ptr;

            iterator _cit2it()
            {
                return const_cast<value_type*>(m_ptr);
            }

            friend class khmz::fxstring<T_CHAR, t_buf_size>;
            friend struct iterator;
        };

        struct reverse_iterator : public std::iterator_traits<value_type*>
        {
            using self_type = reverse_iterator;
            friend struct const_reverse_iterator;

            reverse_iterator(value_type *ptr = nullptr) : m_ptr(ptr)
            {
            }

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            self_type& operator++() { --m_ptr; return *this; }
            self_type& operator--() { ++m_ptr; return *this; }
            self_type operator++(int)
            {
                self_type tmp = *this;
                ++(*this);
                return tmp;
            }
            self_type operator--(int)
            {
                self_type tmp = *this;
                --(*this);
                return tmp;
            }
            self_type& operator+=(difference_type offset)
            {
                m_ptr -= offset;
                return *this;
            }
            self_type& operator-=(difference_type offset)
            {
                m_ptr += offset;
                return *this;
            }

            friend bool operator==(const self_type& a, const self_type& b)
            {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const self_type& a, const self_type& b)
            {
                return a.m_ptr != b.m_ptr;
            }
            friend bool operator<(const self_type& a, const self_type& b)
            {
                return a.m_ptr > b.m_ptr;
            }
            friend bool operator>(const self_type& a, const self_type& b)
            {
                return a.m_ptr < b.m_ptr;
            }
            friend bool operator<=(const self_type& a, const self_type& b)
            {
                return a.m_ptr >= b.m_ptr;
            }
            friend bool operator>=(const self_type& a, const self_type& b)
            {
                return a.m_ptr <= b.m_ptr;
            }

            friend self_type operator+(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp -= index;
                return tmp;
            }
            friend self_type operator-(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp += index;
                return tmp;
            }
            friend size_type operator-(const self_type& a, const self_type& b)
            {
                return b.m_ptr - a.m_ptr;
            }

        protected:
            value_type *m_ptr;

            friend class khmz::fxstring<T_CHAR, t_buf_size>;
            friend struct const_reverse_iterator;
        };

        struct const_reverse_iterator : public std::iterator_traits<const value_type*>
        {
            using self_type = const_reverse_iterator;

            const_reverse_iterator(const value_type *ptr = nullptr) : m_ptr(ptr)
            {
            }
            const_reverse_iterator(reverse_iterator it) : m_ptr(it.m_ptr)
            {
            }
            self_type& operator=(reverse_iterator it)
            {
                m_ptr = it.m_ptr;
                return *this;
            }

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            self_type& operator++() { --m_ptr; return *this; }
            self_type& operator--() { ++m_ptr; return *this; }
            self_type operator++(int)
            {
                self_type tmp = *this;
                ++(*this);
                return tmp;
            }
            self_type operator--(int)
            {
                self_type tmp = *this;
                --(*this);
                return tmp;
            }
            self_type& operator+=(difference_type offset)
            {
                m_ptr -= offset;
                return *this;
            }
            self_type& operator-=(difference_type offset)
            {
                m_ptr += offset;
                return *this;
            }

            friend bool operator==(const self_type& a, const self_type& b)
            {
                return a.m_ptr == b.m_ptr;
            }
            friend bool operator!=(const self_type& a, const self_type& b)
            {
                return a.m_ptr != b.m_ptr;
            }
            friend bool operator<(const self_type& a, const self_type& b)
            {
                return a.m_ptr > b.m_ptr;
            }
            friend bool operator>(const self_type& a, const self_type& b)
            {
                return a.m_ptr < b.m_ptr;
            }
            friend bool operator<=(const self_type& a, const self_type& b)
            {
                return a.m_ptr >= b.m_ptr;
            }
            friend bool operator>=(const self_type& a, const self_type& b)
            {
                return a.m_ptr <= b.m_ptr;
            }

            friend self_type operator+(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp -= index;
                return tmp;
            }
            friend self_type operator-(const self_type& a, difference_type index)
            {
                self_type tmp = a;
                tmp += index;
                return tmp;
            }
            friend size_type operator-(const self_type& a, const self_type& b)
            {
                return b.m_ptr - a.m_ptr;
            }

        protected:
            const value_type *m_ptr;

            reverse_iterator _cit2it()
            {
                return const_cast<value_type*>(m_ptr);
            }

            friend class khmz::fxstring<T_CHAR, t_buf_size>;
            friend struct reverse_iterator;
        };

    protected:
        values_type m_values;

        size_type _length(const T_CHAR *str) const
        {
            size_type ich;
            for (ich = 0; ich < t_buf_size && str[ich]; ++ich)
                ;
            return ich;
        }

        size_type _insert_prologue(size_type index, size_type& count)
        {
            if (index > max_size())
                index = max_size();

            const size_type end_index = index + count;
            if (end_index < max_size())
            {
                const size_type move_count = max_size() - end_index;
                traits_type::move(&m_values[index + count], &m_values[index], move_count);
                m_values[index + count + move_count] = 0;
            }
            else
            {
                m_values[max_size()] = 0;
            }

            if (index + count > max_size())
                count = max_size() - index;

            return index;
        }

    public:
        static constexpr size_type npos = -1;

        //
        // Basic information
        //
        bool empty() const { return !m_values[0]; }
        size_type size() const
        {
            assert(is_terminated());
            return _length(data());
        }
        size_type length() const { return size(); }
        size_type max_size() const { return t_buf_size - 1; }
        size_type buf_size() const { return t_buf_size; }
        pointer data() { return m_values; }
        const_pointer data() const { return m_values; }
        void clear() { m_values[0] = 0; }
        const_pointer c_str() const
        {
            assert(is_terminated());
            return data();
        }
        reference operator[](size_type index)
        {
            assert(index <= max_size());
            return m_values[index];
        }
        const_reference operator[](size_type index) const
        {
            assert(index <= max_size());
            return m_values[index];
        }
        reference at(size_type index)
        {
            if (index > max_size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::at");
            }
            return m_values[index];
        }
        const_reference at(size_type index) const
        {
            if (index > max_size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::at");
            }
            return m_values[index];
        }
        bool is_terminated() const
        {
            return _length(m_values) < t_buf_size;
        }
        void ensure_terminated()
        {
            m_values[max_size()] = 0;
            assert(is_terminated());
        }

        //
        // Iterator
        //
              iterator begin()        { return       iterator(data()); }
        const_iterator begin()  const { return const_iterator(c_str()); }
              iterator end()          { return       iterator(&m_values[size()]); }
        const_iterator end()    const { return const_iterator(&m_values[size()]); }
        const_iterator cbegin() const { return begin(); }
        const_iterator cend()   const { return end(); }

        //
        // Reverse iterator
        //
              reverse_iterator rbegin()        { return       reverse_iterator(&m_values[size() - 1]); }
        const_reverse_iterator rbegin()  const { return const_reverse_iterator(&m_values[size() - 1]); }
              reverse_iterator rend()          { return       reverse_iterator(&data()[-1]); }
        const_reverse_iterator rend()    const { return const_reverse_iterator(&data()[-1]); }
        const_reverse_iterator crbegin() const { return rbegin(); }
        const_reverse_iterator crend()   const { return rend(); }

        //
        // Constructors
        //
        fxstring()
        {
            clear();
        }
        fxstring(size_type count, value_type ch)
        {
            assign(count, ch);
        }
        fxstring(const std::basic_string<T_CHAR>& str)
        {
            assign(str);
        }
        fxstring(const std::basic_string<T_CHAR>& str, size_type pos)
        {
            assign(str, pos);
        }
        fxstring(const std::basic_string<T_CHAR>& str, size_type pos, size_type count)
        {
            assign(str, pos, count);
        }
        template <size_t t_buf_size_2>
        fxstring(const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            assign(str);
        }
        template <size_t t_buf_size_2>
        fxstring(const fxstring<T_CHAR, t_buf_size_2>& str, size_type pos)
        {
            assign(str, pos);
        }
        template <size_t t_buf_size_2>
        fxstring(const fxstring<T_CHAR, t_buf_size_2>& str, size_type pos, size_type count)
        {
            assign(str, pos, count);
        }
        fxstring(const value_type *str)
        {
            assign(str);
        }
        fxstring(const value_type *str, size_type count)
        {
            assign(str, count);
        }
        template <typename InputIterator>
        fxstring(InputIterator first, InputIterator last)
        {
            assign(first, last);
        }
        fxstring(std::initializer_list<value_type> init)
        {
            assign(init.begin(), init.end());
        }

        //
        // Assignments
        //
        self_type& operator=(const std::basic_string<T_CHAR>& str)
        {
            assign(str);
            return *this;
        }
        template <size_t t_buf_size_2>
        self_type& operator=(const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            return assign(str);
        }
        self_type& operator=(const value_type *str)
        {
            return assign(str);
        }
        self_type& operator=(value_type ch)
        {
            return assign(1, &ch);
        }
        self_type& operator=(std::initializer_list<value_type> init)
        {
            return assign(init);
        }
        self_type& assign(const std::basic_string<T_CHAR>& str, size_type pos = 0, size_type count = npos)
        {
            if (pos > str.size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::assign");
            }
            if (count == npos || pos + count > str.size())
                count = str.size() - pos;
            if (count > max_size())
                count = max_size();
            traits_type::copy(data(), &str[pos], count);
            m_values[count] = 0;
            return *this;
        }
        template <size_t t_buf_size_2>
        self_type& assign(const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            if (this == (void*)&str)
                return *this;
            size_type count = str.size();
            if (count > max_size())
                count = max_size();
            traits_type::copy(data(), str.c_str(), count);
            m_values[count] = 0;
            return *this;
        }
        template <size_t t_buf_size_2>
        self_type& assign(const fxstring<T_CHAR, t_buf_size_2>& str, size_type pos, size_type count = npos)
        {
            assert(this != &str);
            if (pos > str.size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::assign");
            }
            if (count == npos || pos + count > str.size())
                count = str.size() - pos;
            if (count > max_size())
                count = max_size();
            traits_type::copy(data(), &str[pos], count);
            m_values[count] = 0;
            return *this;
        }
        self_type& assign(size_type count, value_type ch)
        {
            if (count > max_size())
                count = max_size();
            traits_type::assign(data(), count, ch);
            m_values[count] = 0;
            return *this;
        }
        self_type& assign(const value_type *str)
        {
            return assign(str, traits_type::length(str));
        }
        self_type& assign(const value_type *str, size_type count)
        {
            if (count > max_size())
                count = max_size();
            traits_type::copy(data(), str, count);
            m_values[count] = 0;
            return *this;
        }
        template <typename InputIterator>
        self_type& assign(InputIterator first, InputIterator last)
        {
            size_type i;
            for (i = 0; i < max_size() && first != last; ++i)
            {
                traits_type::assign(m_values[i], *first++);
            }
            m_values[i] = 0;
            return *this;
        }
        self_type& assign(std::initializer_list<value_type> init)
        {
            return assign(init.begin(), init.end());
        }

        //
        // Front and back
        //
        reference front()
        {
            return m_values[0];
        }
        const_reference front() const
        {
            return m_values[0];
        }
        reference back()
        {
            assert(!empty());
            return m_values[size() - 1];
        }
        const_reference back() const
        {
            assert(!empty());
            return m_values[size() - 1];
        }
        void push_back(value_type ch)
        {
            size_type len = size();
            if (len + 1 >= max_size())
                return;
            traits_type::assign(m_values[len++], ch);
            m_values[len] = 0;
        }
        void pop_back()
        {
            size_type len = size();
            if (len > 0)
                m_values[len - 1] = 0;
        }

        //
        // Appending
        //
        self_type& append(size_type count, value_type ch)
        {
            return insert(end(), count, ch);
        }
        self_type& append(const value_type *str)
        {
            return append(str, traits_type::length(str));
        }
        self_type& append(const value_type *str, size_type count)
        {
            size_type len = size();
            if (len + count > max_size())
                count = max_size() - len;
            traits_type::copy(&m_values[len], str, count);
            m_values[len + count] = 0;
            return *this;
        }
        self_type& append(value_type ch)
        {
            return append(&ch, 1);
        }
        self_type& append(const std::basic_string<T_CHAR>& str)
        {
            return append(str.c_str(), str.size());
        }
        template <typename InputIterator>
        self_type& append(InputIterator first, InputIterator last)
        {
            size_type i;
            for (i = size(); i < max_size() && first != last; ++i)
                traits_type::assign(m_values[i], *first++);
            m_values[i] = 0;
        }
        self_type& append(std::initializer_list<value_type> init)
        {
            self_type str(init);
            return append(str);
        }
        template <size_t t_buf_size_2>
        self_type& append(const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            return append(str.c_str(), str.size());
        }
        template <size_t t_buf_size_2>
        self_type& append(const fxstring<T_CHAR, t_buf_size_2>& str, size_type pos)
        {
            if (pos > str.size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::append");
            }
            return append(str.c_str() + pos, str.size() - pos);
        }
        template <size_t t_buf_size_2>
        self_type& append(const fxstring<T_CHAR, t_buf_size_2>& str, size_type pos, size_type count)
        {
            if (count == npos || pos + count > str.size())
                count = str.size() - pos;
            return append(str.c_str() + pos, count);
        }
        template <size_t t_buf_size_2>
        self_type& operator+=(const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            return append(str);
        }
        self_type& operator+=(value_type ch)
        {
            return append(ch);
        }
        self_type& operator+=(const value_type *str)
        {
            return append(str);
        }
        self_type& operator+=(const std::basic_string<T_CHAR>& str)
        {
            return append(str.data(), str.size());
        }
        self_type& operator+=(std::initializer_list<value_type> init)
        {
            return append(init);
        }

        //
        // Comparison
        //
        template <size_t t_buf_size_2>
        int compare(const fxstring<T_CHAR, t_buf_size_2>& str) const
        {
            return compare(str.c_str());
        }
        int compare(const std::basic_string<T_CHAR>& str) const
        {
            return compare(str.c_str());
        }
        int compare(const value_type *str) const
        {
            size_type len = traits_type::length(str);
            size_type count = khmz::detail::_min(len, size());
            int cmp = traits_type::compare(data(), str, count);
            if (cmp)
                return cmp;
            if (traits_type::lt(m_values[count], str[count]))
                return -1;
            if (traits_type::lt(str[count], m_values[count]))
                return +1;
            return 0;
        }

        //
        // Sub-string
        //
        std::basic_string<T_CHAR>
        substr(size_type pos = 0, size_type count = npos)
        {
            if (count == npos)
                count = size();
            else
                count += pos;

            return std::basic_string<T_CHAR>(&m_values[pos], &m_values[count]);
        }

        //
        // Copying characters
        //
        size_type copy(value_type *dest, size_type count, size_type pos = 0)
        {
            if (pos > max_size())
            {
                assert(0);
                throw std::out_of_range("khmz::fxstring::copy");
            }

            if (count == npos)
                count = size() - pos;

            traits_type::copy(dest, &m_values[pos], count);
        }

        //
        // Resizing
        //
        void resize(size_type count)
        {
            resize(count, value_type());
        }
        void resize(size_type count, value_type ch)
        {
            if (count > max_size())
                count = max_size();

            size_type old_len = size();
            if (old_len == count)
                return;

            if (count < old_len)
            {
                m_values[count] = 0;
                return;
            }

            size_type i;
            for (i = old_len; i < count; ++i)
            {
                traits_type::assign(m_values[i], ch);
            }
            m_values[i] = 0;
        }

        //
        // Erase
        //
        self_type& erase()
        {
            m_values[0] = 0;
            return *this;
        }
        self_type& erase(size_type index)
        {
            if (index > max_size())
                index = max_size();
            m_values[index] = 0;
            return *this;
        }
        self_type& erase(size_type index, size_type count)
        {
            const size_type len = size();
            if (index >= len || count == 0)
                return *this;
            if (count == npos || index + count > len)
                count = len - index;
            size_type remaining_count = len - (index + count);
            traits_type::move(&m_values[index], &m_values[index + count], remaining_count);
            m_values[len - count] = 0;
            return *this;
        }
        iterator erase(const_iterator position)
        {
            return erase(position, position + 1);
        }
        iterator erase(const_iterator first, const_iterator last)
        {
            const size_type i0 = first - cbegin(), i1 = last - cbegin();
            erase(i0, i1 - i0);
            return first._cit2it();
        }

        //
        // find(ch, ...) / rfind(ch, ...)
        //
        size_type find(value_type ch, size_type pos = 0) const
        {
            return find_first_of(ch, pos);
        }
        size_type rfind(value_type ch, size_type pos = npos) const
        {
            return find_last_of(ch, pos);
        }

        //
        // find(str, ...) / rfind(str, ...)
        //
        size_type find(const value_type *str, size_type pos = 0) const
        {
            assert(is_terminated());
            assert(pos <= size());
            if (!*str)
                return pos;
            size_type count = traits_type::length(&m_values[pos]);
            size_type str_len = traits_type::length(str);
            const T_CHAR *found;
            for (const T_CHAR* ptr = &m_values[pos];; ptr = found + 1)
            {
                found = traits_type::find(ptr, count, *str);
                if (!found)
                    return npos;
                if (traits_type::compare(found, str, str_len) == 0)
                    return found - data();
                count -= found - ptr;
            }
        }
        size_type find(const self_type& str, size_type pos = 0) const
        {
            return find(str.c_str(), pos);
        }
        size_type find(const std::basic_string<T_CHAR>& str, size_type pos = 0) const
        {
            return find(str.c_str(), pos);
        }
        size_type rfind(const value_type *str, size_type pos = npos) const
        {
            assert(is_terminated());
            size_type str_len = traits_type::length(str);
            if (pos == npos)
            {
                if (size() < str_len)
                    return npos;
                pos = size() - str_len;
            }
            while (pos != npos)
            {
                if (traits_type::compare(&m_values[pos], str, str_len) == 0)
                    return pos;
                --pos;
            }
            return pos;
        }
        size_type rfind(const self_type& str, size_type pos = 0) const
        {
            return rfind(str.c_str(), pos);
        }
        size_type rfind(const std::basic_string<T_CHAR>& str, size_type pos = 0) const
        {
            return rfind(str.c_str(), pos);
        }

        //
        // find_*_of(ch, ...)
        //
        size_type find_first_of(value_type ch, size_type pos = 0) const
        {
            assert(is_terminated());
            if (pos >= size())
                return npos;
            for (size_type ich = pos; m_values[ich]; ++ich)
            {
                if (traits_type::eq(m_values[ich], ch))
                    return ich;
            }
            return npos;
        }
        size_type find_first_not_of(value_type ch, size_type pos = 0) const
        {
            assert(is_terminated());
            if (pos >= size())
                return npos;
            for (size_type ich = pos; m_values[ich]; ++ich)
            {
                if (!traits_type::eq(m_values[ich], ch))
                    return ich;
            }
            return npos;
        }
        size_type find_last_of(value_type ch, size_type pos = npos) const
        {
            assert(is_terminated());
            if (empty())
                return npos;
            if (pos == npos)
                pos = size() - 1;
            while (pos != npos)
            {
                if (traits_type::eq(m_values[pos], ch))
                    return pos;
                --pos;
            }
            return pos;
        }
        size_type find_last_not_of(value_type ch, size_type pos = npos) const
        {
            assert(is_terminated());
            if (empty())
                return npos;
            if (pos == npos)
                pos = size() - 1;
            while (pos != npos)
            {
                if (!traits_type::eq(m_values[pos], ch))
                    return pos;
                --pos;
            }
            return pos;
        }

        //
        // find_*_of(str, ...)
        //
        size_type find_first_of(const value_type *str, size_type pos = 0) const
        {
            assert(is_terminated());
            if (pos >= size())
                return npos;
            const size_type str_len = traits_type::length(str);
            for (size_type ich = pos; m_values[ich]; ++ich)
            {
                if (traits_type::find(str, str_len, m_values[ich]) != nullptr)
                    return ich;
            }
            return npos;
        }
        size_type find_first_of(const self_type& str, size_type pos = 0) const
        {
            return find_first_of(str.c_str(), pos);
        }
        size_type find_first_of(const std::basic_string<T_CHAR>& str, size_type pos = 0) const
        {
            return find_first_of(str.c_str(), pos);
        }
        size_type find_first_not_of(const value_type *str, size_type pos = 0) const
        {
            assert(is_terminated());
            if (pos >= size())
                return npos;
            const size_type str_len = traits_type::length(str);
            for (size_type ich = pos; m_values[ich]; ++ich)
            {
                if (traits_type::find(str, str_len, m_values[ich]) == nullptr)
                    return ich;
            }
            return npos;
        }
        size_type find_first_not_of(const self_type& str, size_type pos = 0) const
        {
            return find_first_not_of(str.c_str(), pos);
        }
        size_type find_first_not_of(const std::basic_string<T_CHAR>& str, size_type pos = 0) const
        {
            return find_first_not_of(str.c_str(), pos);
        }
        size_type find_last_of(const value_type *str, size_type pos = npos) const
        {
            assert(is_terminated());
            if (empty())
                return npos;
            if (pos == npos)
                pos = size() - 1;
            const size_type str_len = traits_type::length(str);
            while (pos != npos)
            {
                if (traits_type::find(str, str_len, m_values[pos]) != nullptr)
                    break;
                --pos;
            }
            return pos;
        }
        size_type find_last_of(const self_type& str, size_type pos = npos) const
        {
            return find_last_of(str.c_str(), pos);
        }
        size_type find_last_of(const std::basic_string<T_CHAR>& str, size_type pos = npos) const
        {
            return find_last_of(str.c_str(), pos);
        }
        size_type find_last_not_of(const value_type *str, size_type pos = npos) const
        {
            assert(is_terminated());
            if (empty())
                return npos;
            if (pos == npos)
                pos = size() - 1;
            const size_type str_len = traits_type::length(str);
            while (pos != npos)
            {
                if (traits_type::find(str, str_len, m_values[pos]) == nullptr)
                    break;
                --pos;
            }
            return pos;
        }
        size_type find_last_not_of(const self_type& str, size_type pos = npos) const
        {
            return find_last_not_of(str.c_str(), pos);
        }
        size_type find_last_not_of(const std::basic_string<T_CHAR>& str, size_type pos = npos) const
        {
            return find_last_not_of(str.c_str(), pos);
        }

        //
        // Insertion
        //
        self_type& insert(size_type index, const value_type* str)
        {
            return insert(index, str, traits_type::length(str));
        }
        self_type& insert(size_type index, const value_type* str, size_type count)
        {
            index = _insert_prologue(index, count);
            traits_type::copy(&m_values[index], str, count);
            return *this;
        }
        self_type& insert(size_type index, size_type count, value_type ch)
        {
            index = _insert_prologue(index, count);
            traits_type::assign(&m_values[index], count, ch);
            return *this;
        }
        self_type& insert(size_type pos, const std::basic_string<T_CHAR>& str)
        {
            return insert(pos, str.c_str(), str.size());
        }
        self_type& insert(const_iterator pos, value_type ch)
        {
            return insert(pos - cbegin(), 1, ch);
        }
        self_type& insert(const_iterator pos, const value_type *ptr)
        {
            return insert(pos, ptr, ptr + traits_type::length(ptr));
        }
        template <typename InputIterator>
        self_type& insert(const_iterator pos, InputIterator first, InputIterator last)
        {
            size_type index = std::distance(cbegin(), pos);
            size_type count = std::distance(first, last);
            index = _insert_prologue(index, count);
            while (index < max_size() && first != last)
                traits_type::assign(m_values[index++], *first++);
            return *this;
        }
        self_type& insert(const_iterator pos, std::initializer_list<value_type> init)
        {
            return insert(pos, init.begin(), init.end());
        }

        //
        // Replace
        //
        self_type& replace(size_type index, size_type count, const value_type* str)
        {
            size_type str_len = traits_type::length(str);

            if (count > str_len)
            {
                erase(index, count - str_len);
            }
            else if (count < str_len)
            {
                size_type diff_len = str_len - count;
                _insert_prologue(index, diff_len);
            }

            if (index + count > max_size())
                count = max_size() - index;

            traits_type::copy(&m_values[index], str, str_len);
            return *this;
        }
        self_type& replace(const_iterator first, const_iterator last, const value_type* str)
        {
            return replace(first - cbegin(), last - first, str);
        }
        self_type& replace(size_type index, size_type count, const std::basic_string<T_CHAR>& str)
        {
            return replace(index, count, str.c_str());
        }
        self_type& replace(const_iterator first, const_iterator last, const std::basic_string<T_CHAR>& str)
        {
            return replace(first, last, str.c_str());
        }
        template <size_t t_buf_size_2>
        self_type& replace(size_type index, size_type count, const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            return replace(index, count, str.c_str());
        }
        template <size_t t_buf_size_2>
        self_type& replace(const_iterator first, const_iterator last, const fxstring<T_CHAR, t_buf_size_2>& str)
        {
            return replace(first, last, str.c_str());
        }

        //
        // Printf
        //
        int printf(const T_CHAR *format, ...)
        {
            va_list va;
            va_start(va, format);
            int len = vprintf(format, va);
            va_end(va);
            return len;
        }
        int printf(const self_type& format, ...)
        {
            va_list va;
            va_start(va, format);
            int len = vprintf(format.c_str(), va);
            va_end(va);
            return len;
        }
        int printf(const std::basic_string<T_CHAR>& format, ...)
        {
            va_list va;
            va_start(va, format);
            int len = vprintf(format.c_str(), va);
            va_end(va);
            return len;
        }
        int vprintf(const char *format, va_list va)
        {
            return _snprintf(data(), t_buf_size, format, va);
        }
        int vprintf(const wchar_t *format, va_list va)
        {
            return _snwprintf(data(), t_buf_size, format, va);
        }

        //
        // Swapping
        //
        void swap(self_type& str)
        {
            self_type tmp = *this;
            *this = str;
            str = tmp;
        }
    }; // fxstring

    template <size_t t_buf_size>
    using fxstring_a = fxstring<char, t_buf_size>;

    template <size_t t_buf_size>
    using fxstring_w = fxstring<wchar_t, t_buf_size>;

#ifdef _UNICODE
    #define fxstring_t fxstring_w
#else
    #define fxstring_t fxstring_a
#endif

    //
    // Comparison: fxstring and fxstring
    //
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator==(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) == 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator!=(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) != 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator>(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) > 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator<(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) < 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator>=(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) >= 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator<=(const fxstring<T_CHAR, t_buf_size_1>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str1.compare(str2) <= 0;
    }

    //
    // Comparison: fxstring and std::basic_string
    //
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator==(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) == 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator!=(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) != 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator>(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) > 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator<(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) < 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator>=(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) >= 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator<=(const fxstring<T_CHAR, t_buf_size_1>& str1, const std::basic_string<T_CHAR>& str2)
    {
        return str1.compare(str2) <= 0;
    }

    //
    // Comparison: psz and fxstring
    //
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator==(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) == 0;
    }
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator!=(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) != 0;
    }
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator>(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) < 0;
    }
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator<(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) > 0;
    }
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator>=(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) <= 0;
    }
    template <typename T_CHAR, size_t t_buf_size_2>
    inline bool operator<=(const T_CHAR *str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) >= 0;
    }

    //
    // Comparison: fxstring and psz
    //
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator==(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) == 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator!=(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) != 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator>(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) > 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator<(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) < 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator>=(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) >= 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1>
    inline bool operator<=(const fxstring<T_CHAR, t_buf_size_1>& str1, const T_CHAR *str2)
    {
        return str1.compare(str2) <= 0;
    }

    //
    // Comparison: std::basic_string and fxstring
    //
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator==(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) == 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator!=(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) != 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator>(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) < 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator<(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) > 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator>=(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) <= 0;
    }
    template <typename T_CHAR, size_t t_buf_size_1, size_t t_buf_size_2>
    inline bool operator<=(const std::basic_string<T_CHAR>& str1, const fxstring<T_CHAR, t_buf_size_2>& str2)
    {
        return str2.compare(str1) >= 0;
    }
} // namespace khmz

namespace std
{
    //
    // Swapping
    //
    template <typename T_CHAR, size_t t_buf_size>
    inline void swap(khmz::fxstring<T_CHAR, t_buf_size>& str1, khmz::fxstring<T_CHAR, t_buf_size>& str2)
    {
        str1.swap(str2);
    }

    //
    // Hash
    //
    template <typename T_CHAR, size_t t_buf_size>
    struct hash<khmz::fxstring<T_CHAR, t_buf_size>>
    {
        inline size_t operator()(const khmz::fxstring<T_CHAR, t_buf_size>& str) const
        {
            size_t ret = str.max_size();
            for (auto ch : str)
            {
                ret *= 3;
                ret ^= ch;
            }
            return ret;
        }
    };
} // namespace std
