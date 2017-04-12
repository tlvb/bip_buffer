#include <cstddef>
#include <cstdint>
class bip_buffer {
	private:
		uint8_t *b0;
		uint8_t *b1;

		uint8_t *p0, *p1;
		uint8_t *q0, *q1;

		uint8_t **w;

	public:
		bip_buffer(std::size_t size);
		~bip_buffer(void);

		uint8_t *reserve(std::size_t& amount);
		uint8_t *reserve(std::size_t&& amount);
		void commit(std::size_t actual);

		uint8_t *access(std::size_t& amount);
		uint8_t *access(std::size_t&& amount);
		void consume(std::size_t actual);

		bool empty(void) const;
		bool full(void) const;
};
