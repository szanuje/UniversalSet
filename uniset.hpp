#pragma once

#include <cstdint>
#include <array>
#include <bitset>

using namespace std;

template<typename T, std::size_t UNIVERSE_SIZE>
class UniversalSet
{
public:
    class Vector
    {
    public:
        class Iterator
        {
        private:
            Iterator(const Vector *vector_ptr);
            Iterator(const Vector *vector_ptr, std::size_t i);

        public:
            Iterator(const Iterator&) = default;
            Iterator& operator=(const Iterator&) = default;
            Iterator(Iterator&&) = default;
            Iterator& operator=(Iterator&&) = default;

            friend class Vector;

        private:
            const Vector *vector_ptr_;
            std::size_t i_;

        public:
            bool operator==(const Iterator &it) const;
            bool operator!=(const Iterator &it) const;
            // Przechodzi na kolejny (istniejacy!) element w "Vector"
            Iterator& operator++();
            Iterator operator++(int);
            const T& operator*() const;
            const T* operator->() const;
            operator bool() const;
        };

    public:
        Vector(const UniversalSet *universe_ptr);
        Vector(const UniversalSet *universe_ptr, const std::bitset<UNIVERSE_SIZE> &bs);
        Vector(const Vector &v);
        Vector& operator=(const Vector &universe);
        Vector(Vector &&universe);
        Vector& operator=(Vector &&universe);
        friend class Iterator;

    private:
        std::bitset<UNIVERSE_SIZE> bs_;
        const UniversalSet<T, UNIVERSE_SIZE> *universe_ptr_;

    public:
        // Zwraca liczbe elementów reprezentowanych przez "Vector"
        std::size_t count() const;
        // Jezeli element o indeksie "i" nie nalezy do "Vector" to dodaje ten element oraz zwraca "Iterator" do dodanego elementu i "true", w przeciwnym razie zwraca samo co "end()" i "false"
        std::pair<Iterator, bool> insert(std::size_t i);
        // Jezeli element o indeksie "i" nalezy do "Vector" to zwraca "true", w przeciwnym razie zwraca "false"
        bool isMember(std::size_t i) const;
        // Jeżeli element o indeksie "i" należy do "Vector" to zwraca "Iterator" do tego elementu, w przeciwnym razie zwraca to samo co "end()"
        Iterator elemIterator(std::size_t i) const;
        // Jezeli element o indeksie "i" nalezy do "Vector" to usuwa ten element i zwraca "true", w przeciwnym razie zwraca "false"
        bool remove(std::size_t i);
        // Zwraca "true" jeżeli obiekt "v2" reprezentuje ten sam zestaw elementow z tego samego uniwersum, w przeciwnym razie zwraca "false"
        bool operator==(const Vector &v2) const;
        bool operator!=(const Vector &v2) const;
        // Zwraca sume zbiorow
        Vector operator+(const Vector &v2) const;
        // Zwraca roznice zbiorow
        Vector operator-(const Vector &v2) const;
        // Zwraca czesc wspolna zbiorow
        Vector operator*(const Vector &v2) const;
        // Zwraca roznice symetryczna zbiorow
        Vector operator/(const Vector &v2) const;
        // Zwraca "Iterator" na poczatek
        Iterator begin() const;
        // Zwraca "Iterator" za koniec
        Iterator end() const;
    };

public:
    UniversalSet(const std::array<T, UNIVERSE_SIZE> &elems);
    UniversalSet(const UniversalSet&) = default;
    UniversalSet& operator=(const UniversalSet&) = default;
    UniversalSet(UniversalSet&&) = default;
    UniversalSet& operator=(UniversalSet&&) = default;

private:
    std::array<T, UNIVERSE_SIZE> elems_;

public:
    constexpr std::size_t universeSize() const;
    const T& elem(std::size_t i) const;
    const T& operator[](std::size_t i) const;
    // Zwraca pusty "Vector"
    Vector makeVector() const;
};

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::UniversalSet(const std::array<T, UNIVERSE_SIZE> &elems) : elems_(elems) {}

template<typename T, std::size_t UNIVERSE_SIZE>
constexpr std::size_t UniversalSet<T, UNIVERSE_SIZE>::universeSize() const { return UNIVERSE_SIZE; }

template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::elem(std::size_t i) const { return elems_[i]; }

template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::operator[](std::size_t i) const { return elems_[i]; }

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector UniversalSet<T, UNIVERSE_SIZE>::makeVector() const { return Vector(this); }

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Vector(const UniversalSet *universe_ptr) : universe_ptr_(universe_ptr) {
    for (unsigned int i = 0; i < universe_ptr->universeSize(); i++) {
        this->bs_[i] = false;
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Vector(const UniversalSet *universe_ptr, const std::bitset<UNIVERSE_SIZE> &bs) : universe_ptr_(universe_ptr), bs_(bs) {}

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Vector(const Vector &v) {
    this->universe_ptr_ = v.universe_ptr_;
    this->bs_ = v.bs_;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector& UniversalSet<T, UNIVERSE_SIZE>::Vector::operator=(const Vector &universe) {
    Vector vec(universe);
    this->universe_ptr_ = vec.universe_ptr_;
    this->bs_ = vec.bs_;
    return *this;
}

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Vector(Vector &&universe) {
    this->universe_ptr_ = universe.universe_ptr_;
    this->bs_ = universe.bs_;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector& UniversalSet<T, UNIVERSE_SIZE>::Vector::operator=(Vector &&universe) {
    this->universe_ptr_ = universe.universe_ptr_;
    this->bs_ = universe.bs_;
    return *this;
}

template<typename T, std::size_t UNIVERSE_SIZE>
std::size_t UniversalSet<T,UNIVERSE_SIZE>::Vector::count() const { return bs_.count(); }

template<typename T, std::size_t UNIVERSE_SIZE>
std::pair<typename UniversalSet<T,UNIVERSE_SIZE>::Vector::Iterator, bool> UniversalSet<T,UNIVERSE_SIZE>::Vector::insert(std::size_t i) {
    if(bs_[i] == false) {
        bs_[i] = true;
        return make_pair(Iterator(this, i), true);
    }
    else {
        return make_pair(Iterator(this, bs_.size()), false);
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T,UNIVERSE_SIZE>::Vector::isMember(std::size_t i) const { return bs_[i]; }

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T,UNIVERSE_SIZE>::Vector::elemIterator(std::size_t i) const { return bs_[i] ? Iterator(this, i) : Iterator(this, bs_.size()); }

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T,UNIVERSE_SIZE>::Vector::remove(std::size_t i) {
    if(bs_[i]) {
        bs_[i] = false;
        return true;
    }
    else {
        return false;
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T,UNIVERSE_SIZE>::Vector::operator==(const Vector &v2) const { return universe_ptr_ == v2.universe_ptr_ && bs_ = v2.bs_; }

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T,UNIVERSE_SIZE>::Vector::operator!=(const Vector &v2) const { return universe_ptr_ != v2.universe_ptr_ && bs_ = v2.bs_; }

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector UniversalSet<T,UNIVERSE_SIZE>::Vector::operator+(const Vector &v2) const {
    Vector vec(*this);
    for(unsigned int i = 0; i < bs_.size(); ++i) {
        vec.bs_ |= v2.bs_;
    }
    return vec;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector UniversalSet<T,UNIVERSE_SIZE>::Vector::operator-(const Vector &v2) const {
    Vector vec(*this);
    for(unsigned int i = 0; i < bs_.size(); ++i) {
        if(vec.bs_[i] && !v2.bs_[i]) vec.bs_[i] = true;
        if(!vec.bs_[i] && v2.bs_[i]) vec.bs_[i] = false;
        if(vec.bs_[i] && v2.bs_[i]) vec.bs_[i] = false;
        if(!vec.bs_[i] && !v2.bs_[i]) vec.bs_[i] = false;
    }
    return vec;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector UniversalSet<T,UNIVERSE_SIZE>::Vector::operator*(const Vector &v2) const {
    Vector vec(*this);
    for(unsigned int i = 0; i < bs_.size(); ++i) {
        vec.bs_ &= v2.bs_;
    }
    return vec;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector UniversalSet<T,UNIVERSE_SIZE>::Vector::operator/(const Vector &v2) const {
    Vector vec(*this);
    for(unsigned int i = 0; i < bs_.size(); ++i) {
        vec.remove(i);
        if(this->bs_[i] && !v2.bs_[i]) vec.insert(i);
        if(!this->bs_[i] && v2.bs_[i]) vec.insert(i);
    }
    return vec;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T,UNIVERSE_SIZE>::Vector::begin() const {

    return Iterator(this);
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T,UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T,UNIVERSE_SIZE>::Vector::end() const { return Iterator(this, bs_.size()); }

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::Iterator(const Vector *vector_ptr) : vector_ptr_(vector_ptr) {
    for(unsigned int i = 0; i < vector_ptr->bs_.size(); i++) {
        if(vector_ptr_->bs_[i]) {
            this->i_ = i;
            break;
        }
    }
}

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::Iterator(const Vector *vector_ptr, std::size_t i) : vector_ptr_(vector_ptr), i_(i) {}


template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator==(const Iterator &it) const { return it.i_ == i_; }

template<typename T, std::size_t UNIVERSE_SIZE>
bool UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator!=(const Iterator &it) const { return it.i_ != i_; }

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator& UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++() {
    while( this->i_ != vector_ptr_->universe_ptr_->universeSize() ) {
        this->i_++;
        if(this->vector_ptr_->bs_[i_] == true) {
            break;
        }
    }
    return *this;
}

template<typename T, std::size_t UNIVERSE_SIZE>
typename UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator++(int) {
    Iterator temp(*this);
    ++(*this);
    return temp;
}

template<typename T, std::size_t UNIVERSE_SIZE>
const T& UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator*() const { return vector_ptr_->universe_ptr_->elems_[i_]; }

template<typename T, std::size_t UNIVERSE_SIZE>
const T* UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator->() const { return &vector_ptr_->universe_ptr_->elems_[i_]; }

template<typename T, std::size_t UNIVERSE_SIZE>
UniversalSet<T, UNIVERSE_SIZE>::Vector::Iterator::operator bool() const { return !(this->i_ >= vector_ptr_->universe_ptr_->universeSize()); }
