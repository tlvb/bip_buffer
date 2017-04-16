#include "bip_buffer.h"
#include <utility>
template<typename T>bip_buffer<T>::bip_buffer(void) : /*{{{*/
	b0(nullptr),
	b1(nullptr),
	p0(nullptr),
	p1(nullptr),
	q0(nullptr),
	q1(nullptr)
{} /*}}}*/
template<typename T>bip_buffer<T>::bip_buffer(std::size_t size) : /*{{{*/
	b0(new T[size]),
	b1(b0+size),
	p0(b0),
	p1(b0),
	q0(b0),
	q1(b0)
{} /*}}}*/
template<typename T>bip_buffer<T>::bip_buffer(bip_buffer<T>&& other) : /*{{{*/
	b0(other.b0),
	b1(other.b1),
	p0(other.p0),
	p1(other.p1),
	q0(other.q0),
	q1(other.q1)
{
	other.b0 = other.b1 = other.p0 = other.p1 = other.q0 = other.q1 = nullptr;
} /*}}}*/
template<typename T>bip_buffer<T>& bip_buffer<T>::operator=(bip_buffer<T>&& other) { /*{{{*/
	delete[] b0;
	b0 = other.b0;
	b1 = other.b1;
	p0 = other.p0;
	p1 = other.p1;
	q0 = other.q0;
	q1 = other.q1;
	other.b0 = other.b1 = other.p0 = other.p1 = other.q0 = other.q1 = nullptr;
	return *this;
} /*}}}*/
template<typename T>bip_buffer<T>::~bip_buffer(void) { /*{{{*/
	if (b0 != nullptr) {
		delete[] b0;
	}
} /*}}}*/
template<typename T>T *bip_buffer<T>::reserve(std::size_t& amount) { /*{{{*/

	if (this->full()) {
		amount = 0;
		return nullptr;
	}

	std::size_t p_avail = this->b1 - this->p1;
	std::size_t q_avail = this->p0 - this->q1;

	if (this->q1 == this->b0 && ((amount > 0 && amount <= p_avail) || (p_avail >= q_avail))) {
		amount = p_avail;
		this->w = &this->p1;
	}
	else {
		amount = q_avail;
		this->w = &this->q1;
	}
	if (amount == 0) {
		this->w = nullptr;
		return nullptr;
	}
	return *this->w;

} /*}}}*/
template<typename T>T *bip_buffer<T>::reserve(std::size_t&& amount) { /*{{{*/
	std::size_t a = amount;
	if (a == 0) {
		return nullptr;
	}
	T *p = this->reserve(a);
	if (a < amount) {
		this->commit(0);
		return nullptr;
	}
	return p;
} /*}}}*/
template<typename T>void bip_buffer<T>::commit(std::size_t actual) { /*{{{*/
	*this->w += actual;
} /*}}}*/
template<typename T>T *bip_buffer<T>::access(std::size_t& amount) { /*{{{*/
	amount = this->p1 - this->p0;
	if (amount != 0) {
		return this->p0;
	}
	return nullptr;
} /*}}}*/
template<typename T>T *bip_buffer<T>::access(std::size_t&& amount) { /*{{{*/
	std::size_t a = amount;
	T *p = this->access(a);
	if (a < amount) {
		return nullptr;
	}
	return p;
} /*}}}*/
template<typename T>void bip_buffer<T>::consume(std::size_t actual) { /*{{{*/
	this->p0 += actual;
	if (this->p0 == this->p1) {
		this->p0 = this->q0;
		this->p1 = this->q1;
		this->q0 = this->q1 = this->b0;
	}
} /*}}}*/
template<typename T>bool bip_buffer<T>::empty(void) const { /*{{{*/
	return this->p0 == this->p1;
} /*}}}*/
template<typename T>bool bip_buffer<T>::full(void) const { /*{{{*/
	return (this->p0 == this->b0 && this->p1 == this->b1) || (this->p0 != this->b0 && this->q1 == this->p0);
} /*}}}*/
