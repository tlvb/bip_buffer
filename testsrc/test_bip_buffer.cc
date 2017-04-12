#include "bip_buffer.h"
#include "bip_buffer.tcc"
#include <cassert>
#include <iostream>

using namespace std;

int  main(void) {
	bip_buffer<uint32_t> b(8);

	uint32_t *p = nullptr;
	uint32_t *base = nullptr;
	std::size_t n = 0;

	assert( b.empty() );
	assert( !b.full() );
	// The upper row shows the primary (p0,p1) postd::size_ters, W means they postd::size_t at the same index
	// otherwise a 0 or a 1 will be placed at the index they postd::size_t at. On the lowermost row
	// the secondary (q0,q1) postd::size_ters are shown, same thing, but an M is used instead of W
	// pW
	// [0|1|2|3|4|5|6|7]
	// qM

	p = b.reserve(n);

	assert( p != nullptr );
	assert( n == 8 );

	n = 0;
	p = b.reserve(n);

	assert( p != nullptr );
	assert( n == 8 );

	n = 1000;
	p = b.reserve(n);

	assert( p != nullptr );
	assert( n == 8 );

	//  W
	// [0|1|2|3|4|5|6|7]
	//  M

	b.commit(6);

	// p0           1
	// [0|1|2|3|4|5|6|7]
	// qM

	assert( !b.empty() );
	assert( !b.full()  );

	n = 0;
	base = b.access(n);

	assert( n == 6 );
	assert( p == base );

	b.consume(1);
	// p  0         1
	// [0|1|2|3|4|5|6|7]
	// qM

	n = 0;
	p = b.access(n);

	assert( n == 5 );
	assert( p == base+1  );

	n = 3;
	p = b.reserve(n);
	assert( n == 2 );
	assert( p == base+6 );

	// when using an rvalue reference we will only
	// receive a valid postd::size_ter if at least as much
	// as we asked for is able to be reserved, because
	// we have no way of knowing the available amount
	p = b.reserve(3);
	assert( p == nullptr );

	p = b.reserve(2);
	assert( p == base+6 );

	n = 0;
	b.reserve(n);

	assert( n == 2 );
	assert( p = base + 6 );

	b.consume(2);
	// p      0     1
	// [0|1|2|3|4|5|6|7]
	// qM

	n = 1;
	p = b.reserve(n);
	assert( n == 2 );
	assert( p == base+6 );

	n = 0;
	p = b.reserve(n);
	assert( n == 3 );
	assert( p == base );

	b.commit(1);
	// p      0     1
	// [0|1|2|3|4|5|6|7]
	// q0 1

	n = 0;
	p = b.reserve(n);
	assert( n == 2 );
	assert( p == base+1 );

	b.commit(2);
	// p      0     1
	// [0|1|2|3|4|5|6|7]
	// q0     1

	assert( !b.empty() );
	assert( b.full()   );

	n = 0;
	p = b.reserve(n);
	assert( n == 0 );
	assert( p == nullptr );
	// p      0     1
	// [0|1|2|3|4|5|6|7]
	// q0     1

	n = 0;
	p = b.access(n);

	assert( n == 3 );
	assert( p = base+3 );

	b.consume(1);
	// p        0   1
	// [0|1|2|3|4|5|6|7]
	// q0     1

	assert( !b.empty() );
	assert( !b.full()   );

	n = 0;
	p = b.access(n);

	assert( n == 2 );
	assert( p == base+4 );

	b.consume(2);
	// p            W    -> p0     1
	// [0|1|2|3|4|5|6|7] -> [0|1|2|3|4|5|6|7]
	// q0     1          -> qM


	// same thing with rvalue references for access
	p = b.access(4);
	assert( p == nullptr );
	p = b.access(3);
	assert( p == base );

	b.consume(3);
	//
	// p      W          -> pW
	// [0|1|2|3|4|5|6|7] -> [0|1|2|3|4|5|6|7]
	// qM                -> qM

	assert( b.empty() );
	assert( !b.full() );

	n = 0;
	p = b.access(n);

	assert( n == 0 );
	assert( p == nullptr );

	std::cout << "the test sequence reached its end without error" << std::endl;
	return 0;
}











