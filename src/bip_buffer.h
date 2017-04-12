#pragma once
#include <cstddef>
template <typename T> class bip_buffer {
	private:
		T *b0;
		T *b1;

		T *p0, *p1;
		T *q0, *q1;

		T **w;

	public:
		bip_buffer(std::size_t size);
		~bip_buffer(void);

		T *reserve(std::size_t& amount);
		T *reserve(std::size_t&& amount);
		void commit(std::size_t actual);

		T *access(std::size_t& amount);
		T *access(std::size_t&& amount);
		void consume(std::size_t actual);

		bool empty(void) const;
		bool full(void) const;
};
