#include "bip_buffer.h"
#include <utility>
template<typename T>bip_buffer<T>::bip_buffer(void) : /*{{{*/
	b0(nullptr),
	b1(nullptr),
	p0(nullptr),
	p1(nullptr),
	q0(nullptr),
	q1(nullptr),
	w(nullptr)
{} /*}}}*/
template<typename T>bip_buffer<T>::bip_buffer(std::size_t size) : /*{{{*/
	b0(new T[size]),
	b1(b0+size),
	p0(b0),
	p1(b0),
	q0(b0),
	q1(b0),
	w(nullptr)
{} /*}}}*/
template<typename T>bip_buffer<T>::bip_buffer(bip_buffer<T>&& other) : /*{{{*/
	b0(other.b0),
	b1(other.b1),
	p0(other.p0),
	p1(other.p1),
	q0(other.q0),
	q1(other.q1),
	w(other.w)
{
	other.b0 = other.b1 = other.p0 = other.p1 = other.q0 = other.q1 = nullptr;
	other.w = nullptr;
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
	other.w = nullptr;
	return *this;
} /*}}}*/
template<typename T>bip_buffer<T>::~bip_buffer(void) { /*{{{*/
	if (b0 != nullptr) {
		delete[] b0;
	}
} /*}}}*/
template<typename T>T *bip_buffer<T>::reserve(std::size_t& amount) { /*{{{*/

	if (full()) {
		amount = 0;
		return nullptr;
	}

	std::size_t p_avail = b1 - p1;
	std::size_t q_avail = p0 - q1;

	if (q1 == b0 && ((amount > 0 && amount <= p_avail) || (p_avail >= q_avail))) {
		amount = p_avail;
		w = &p1;
	}
	else {
		amount = q_avail;
		w = &q1;
	}
	if (amount == 0) {
		w = nullptr;
		return nullptr;
	}
	return *w;

} /*}}}*/
template<typename T>T *bip_buffer<T>::reserve(std::size_t&& amount) { /*{{{*/
	std::size_t a = amount;
	if (a == 0) {
		return nullptr;
	}
	T *p = reserve(a);
	if (a < amount) {
		commit(0);
		return nullptr;
	}
	return p;
} /*}}}*/
template<typename T>void bip_buffer<T>::commit(std::size_t actual) { /*{{{*/
	*w += actual;
} /*}}}*/
template<typename T>T *bip_buffer<T>::access(std::size_t& amount) { /*{{{*/
	amount = p1 - p0;
	if (amount != 0) {
		return p0;
	}
	return nullptr;
} /*}}}*/
template<typename T>T *bip_buffer<T>::access(std::size_t&& amount) { /*{{{*/
	std::size_t a = amount;
	T *p = access(a);
	if (a < amount) {
		return nullptr;
	}
	return p;
} /*}}}*/
template<typename T>void bip_buffer<T>::consume(std::size_t actual) { /*{{{*/
	p0 += actual;
	if (p0 == p1) {
		p0 = q0;
		p1 = q1;
		q0 = q1 = b0;
	}
} /*}}}*/
template<typename T>bool bip_buffer<T>::empty(void) const { /*{{{*/
	return p0 == p1;
} /*}}}*/
template<typename T>bool bip_buffer<T>::full(void) const { /*{{{*/
	return (p0 == b0 && p1 == b1) || (p0 != b0 && q1 == p0);
} /*}}}*/
