#pragma once

#include <utility/Definitions.hpp>


namespace Ruby::STL {
    template<typename Tx>
    interface IContainsType {
        using value_type = Tx;
    };

    template<typename Key, typename Value>
    interface IContainsPair {
        using key_type = Key;
        using value_type = Value;
    };


    template<typename IteratorType, typename ConstIteratorType>
    interface IIterable {
    public:
        using iterator = IteratorType;
        using const_iterator = ConstIteratorType;

    public:
        virtual iterator begin() = 0;
        virtual iterator end() = 0;

        virtual const_iterator cbegin() = 0;
        virtual const_iterator cend() = 0;
    };


    template<typename IteratorType, typename ConstIteratorType, typename AllocatorType> 
    interface IBasicContainer : IIterable<IteratorType, ConstIteratorType> {
    public:
        using iterator = IteratorType;
        using const_iterator = ConstIteratorType;
        using allocator_type = AllocatorType; 

    public:
        virtual void clear() = 0;

        virtual bool emtpy() = 0;
        virtual size_t size() = 0;
    };


    template<typename Tx, typename IteratorType, typename ConstIteratorType, typename AllocatorType>
    interface IRandomAccessContainer : public IBasicContainer<IteratorType, ConstIteratorType, AllocatorType> {
    public:
        using value_type = Tx;

        using pointer = Tx*;
        using const_pointer = const Tx*;

        using reference = Tx&;
        using const_reference = const Tx&;

    public:
        virtual const_reference at(size_t pos) const
        virtual const_reference operator[](size_t pos) const;
        virtual reference operator[](size_t pos);

        virtual const_reference front() const;
        virtual reference front();

        virtual const_reference back() const;
        virtual reference back();

        virtual const_pointer data() const;
        virtual pointer data();
    };



    template<typename Tx>
    interface ISmartPoitner {
    public:
        using element_type = Tx;

    public:
        virtual void reset() = 0;
        virtual void swap(ISmartPoitner* other) = 0;


        virtual element_type* get() const = 0;

        virtual element_type& operator*() const = 0;
        virtual element_type* operator->() const = 0;

        virtual element_type& operator[](ptrdiff_t idx) const;

        virtual i32 use_count() const;

        virtual explicit operator bool() const = 0;
    };
}