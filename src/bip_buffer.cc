#include "bip_buffer.h"
bip_buffer::bip_buffer(std::size_t size) { /*{{{*/
	this->b0 = new uint8_t[size];
	this->b1 = this->b0+size;
	this->p0 = this->p1 = this->q0 = this->q1 = this->b0;

} /*}}}*/
bip_buffer::~bip_buffer(void) { /*{{{*/
	delete[] this->b0;
} /*}}}*/
uint8_t *bip_buffer::reserve(std::size_t& amount) { /*{{{*/

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
uint8_t *bip_buffer::reserve(std::size_t&& amount) { /*{{{*/
	size_t a = amount;
	if (a == 0) {
		return nullptr;
	}
	uint8_t *p = this->reserve(a);
	if (a < amount) {
		this->commit(0);
		return nullptr;
	}
	return p;
} /*}}}*/
void bip_buffer::commit(std::size_t actual) { /*{{{*/
	*this->w += actual;
} /*}}}*/
uint8_t *bip_buffer::access(std::size_t& amount) { /*{{{*/
	amount = this->p1 - this->p0;
	if (amount != 0) {
		return this->p0;
	}
	return nullptr;
} /*}}}*/
uint8_t *bip_buffer::access(std::size_t&& amount) { /*{{{*/
	size_t a = amount;
	uint8_t *p = this->access(a);
	if (a < amount) {
		return nullptr;
	}
	return p;
} /*}}}*/
void bip_buffer::consume(std::size_t actual) { /*{{{*/
	this->p0 += actual;
	if (this->p0 == this->p1) {
		this->p0 = this->q0;
		this->p1 = this->q1;
		this->q0 = this->q1 = this->b0;
	}
} /*}}}*/
bool bip_buffer::empty(void) const { /*{{{*/
	return this->p0 == this->p1;
} /*}}}*/
bool bip_buffer::full(void) const { /*{{{*/
	return (this->p0 == this->b0 && this->p1 == this->b1) || (this->p0 != this->b0 && this->q1 == this->p0);
} /*}}}*/
