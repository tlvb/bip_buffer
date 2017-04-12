# bip_buffer
bipartite buffer implementation

The code is based on a loose read of the description that I stumbled upon [here](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist).

The gist of it is this:

You have a buffer of some sort, where you increase one pointer as you add data,
and you increase another one as you remove data. The difference between the pointers
is the amount of data stored in the buffer currently, and the difference between
the end of the buffer and the first pointer is the amount of free space left.

But what about the space you free up in the beginning of the buffer when you remove data?

A circular buffer takes care of this by warping the pointers at the end of the buffer.
This has the downside that you can not expect your data to be stored in a contiguous chunk,
which makes it trickier to copy data to and from the buffer.

A bipartite buffer takes care of this by keeping a second set of pointers. You begin by
requesting a pointer for writing, and if at that time the free space at the beginning of the
buffer is larger than the space at the end, it will return a pointer to that space instead,
and use the second pointer pair to keep track of that data. Of course when you request more
space again, it will always make you use the second pointer pair, otherwise the data
removal order would be corrupted. When all the data pointed at by the first pointer has been
removed, it takes the values of the second pair, and the second pair is reset.

It is of course possible, depending on strategy, that you will need to do non-contiguous
copying with a bipartite buffer, but it does however depend on the amount of free space,
and the size of the objects you want to reand and write. With a large enough buffer
that you keep emptying in a timely manner, it can be avoided.

With a circular buffer on the other hand, you *will* run into non-contiguous reads/writes
(unless you always read and write fixed size blocks and the buffer size is an integral
multiple of that), because of how the pointers travel in the buffer.

This implementation of *this* bip buffer is different from the one described on the linked
website in that you can hint the amount of data you need to write when you make a request,
and if there is enough space available at the end of the buffer to fulfill the request,
you will get a pointer to that space, instead of the beginning of the buffer, even if
there is more space available there.
