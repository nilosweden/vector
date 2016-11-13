#ifndef FSX_VECTOR_HPP
#define FSX_VECTOR_HPP

#include "iterator.hpp"
#include <new>
#include <memory.h>
#include <utility>

namespace fsx
{
    template <typename T>
    class vector
    {
        private:
            long m_endIndex;
            long m_beginIndex;
            size_t m_capacity;
            T* m_array;

        public:
            using iterator = fsx::iterator<T>;

            vector() :
                m_endIndex(-1),
                m_beginIndex(0),
                m_capacity(5),
                m_array(static_cast<T*>(::operator new(sizeof(T) * this->m_capacity)))
            {}

            const size_t& capacity() const noexcept
            {
                return this->m_capacity;
            }

            T& operator[](const unsigned int& index) const noexcept
            {
                return this->m_array[this->m_beginIndex + index];
            }

            T& at(const unsigned int& index) const
            {
                return this->m_array[this->m_beginIndex + index];
            }

            bool empty() noexcept
            {
                return !this->size();
            }

            template <typename... Args>
            void push_back(Args&&... args) noexcept
            {
                if (this->size() == this->m_capacity)
                {
                    this->resize(this->m_capacity * 2);
                }
                ++this->m_endIndex;
                new(&this->m_array[this->m_endIndex]) T(std::forward<Args>(args)...);
            }

            template <typename... Args>
            void emplace_back(Args&&... args) noexcept
            {
                if (this->size() == this->m_capacity)
                {
                    this->resize(this->m_capacity * 2);
                }
                ++this->m_endIndex;
                new(&this->m_array[this->m_endIndex]) T(std::forward<Args>(args)...);
            }

            T* data() const noexcept
            {
                return this->m_array;
            }

            void clear() noexcept
            {
                for (size_t i = 0; i < this->size(); ++i)
                {
                    this->m_array[this->m_beginIndex + i].~T();
                }
                this->m_beginIndex = 0;
                this->m_endIndex = -1;
            }

            T& back() const noexcept
            {
                return this->m_array[this->m_endIndex];
            }

            T& front() const noexcept
            {
                return this->m_array[this->m_beginIndex];
            }

            fsx::iterator<T> begin() const noexcept
            {
                return fsx::iterator<T>(this);
            }

            fsx::iterator<T> end() const noexcept
            {
                return fsx::iterator<T>(this, this->size());
            }

            void pop_back() noexcept
            {
                this->m_array[this->m_endIndex].~T();
                --this->m_endIndex;
            }

            void pop_front() noexcept
            {
                this->m_array[this->m_beginIndex].~T();
                ++this->m_beginIndex;
            }

            void repack() noexcept
            {
                if (this->m_beginIndex > 0)
                {
                    memmove(
                        &this->m_array[0],
                        &this->m_array[this->m_beginIndex],
                        sizeof(T) * this->size()
                    );
                    this->m_endIndex -= this->m_beginIndex;
                    this->m_beginIndex = 0;
                }
            }

            void shrink_to_fit() noexcept
            {
                this->resize(this->size());
            }

            inline size_t size() const noexcept
            {
                return this->m_endIndex - this->m_beginIndex + 1;
            }

            fsx::iterator<T> erase(const fsx::iterator<T>& iterator)
            {
                const auto& index = iterator.index();
                this->m_array[this->m_beginIndex + index].~T();

                if (index < static_cast<size_t>(this->m_endIndex / 2))
                {
                    memmove(
                        &this->m_array[this->m_beginIndex + 1],
                        &this->m_array[this->m_beginIndex],
                        sizeof(T) * index
                    );
                    ++this->m_beginIndex;
                    return fsx::iterator<T>(this, index + 1, 1, 0);
                }
                else
                {
                    if (index >= static_cast<size_t>(this->m_endIndex - this->m_beginIndex))
                    {
                        --this->m_endIndex;
                        return fsx::iterator<T>(this, index, 0, 1);
                    }
                    else
                    {
                        int elements = (this->m_endIndex - this->m_beginIndex - index);
                        if (elements > 0)
                        {
                            memmove(
                                &this->m_array[this->m_beginIndex + index],
                                &this->m_array[this->m_beginIndex + index + 1],
                                sizeof(T) * elements
                            );
                        }
                        --this->m_endIndex;
                        return fsx::iterator<T>(this, index + 1, 1, 0);
                    }
                }
            }

            fsx::iterator<T> erase(const fsx::iterator<T>& iterBegin, const fsx::iterator<T>& iterEnd) noexcept
            {
                const auto& beginIndex = iterBegin.index();
                const auto& endIndex = iterEnd.index();
                size_t elementsToRemove = endIndex - beginIndex;
                size_t elementsToMove = 0;

                for (size_t i = 0; i < elementsToRemove; ++i)
                {
                    this->m_array[this->m_beginIndex + beginIndex + i].~T();
                }

                if ((beginIndex - this->m_beginIndex) < (endIndex - this->m_endIndex))
                {
                    elementsToMove = iterBegin.m_position - this->m_beginIndex;
                    memmove(
                        &this->m_array[this->m_beginIndex + endIndex - elementsToMove],
                        &this->m_array[this->m_beginIndex],
                        sizeof(T) * elementsToMove
                    );
                    this->m_beginIndex += elementsToRemove;
                    return fsx::iterator<T>(
                        this,
                        iterEnd.m_position,
                        iterEnd.m_start + elementsToRemove,
                        iterEnd.m_end
                    );
                }
                else
                {
                    elementsToMove = this->m_endIndex - endIndex;
                    memmove(
                        &this->m_array[this->m_beginIndex + beginIndex],
                        &this->m_array[this->m_beginIndex + endIndex],
                        sizeof(T) * elementsToMove
                    );
                    this->m_endIndex -= elementsToRemove;
                    return fsx::iterator<T>(this, iterEnd.m_position, this->m_beginIndex, elementsToRemove);
                }
            }

            inline void reserve(const size_t& size) noexcept
            {
                if (size > this->m_capacity)
                {
                    this->resize(size);
                }
            }

            inline void resize(const size_t& size) noexcept
            {
                if (size < this->size())
                {
                    T* tmp{static_cast<T*>(::operator new(sizeof(T) * this->m_capacity))};
                    memmove(tmp, &this->m_array[this->m_beginIndex], sizeof(T) * size);
                    ::operator delete(this->m_array);
                    this->m_array = tmp;
                    this->m_endIndex = size - 1;
                    this->m_beginIndex = 0;
                }
                else
                {
                    this->m_capacity = size;
                    T* tmp{static_cast<T*>(::operator new(sizeof(T) * this->m_capacity))};
                    memmove(
                        tmp,
                        &this->m_array[this->m_beginIndex],
                        sizeof(T) * this->size()
                    );
                    ::operator delete(this->m_array);
                    this->m_array = tmp;
                    this->m_endIndex -= this->m_beginIndex;
                    this->m_beginIndex = 0;
                }
            }

            ~vector()
            {
                this->clear();
                ::operator delete(this->m_array);
            }
        };
}

#endif
