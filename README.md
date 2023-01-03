

# STL

## vector的实现

重点:

- 关于operator new 和 placement new
- 迭代器的实现
- size 和 capacity的区别
- Reallocate的实现

1.operator new 只负责分配内存和placement new只负责构造对象，而我们平常调用的new也就是new operator 在分配内存的同时也构造了对象。而在构造vector中我们需要提前分配一块连续的比较大的内存但是并不需要构造，构造的时机是将对象装入容器时或者是扩容的时候，所以为了将分配内存和构造对象分开来就必须用到operator new 和 placement new。

2.我们在迭代器中封装了对象指针，重载了一系列指针运算符* 、–>等，符合迭代器和指针一样的使用方式，在Vector类中定义了begin和end函数返回迭代器，此操作可以使我们自定义的容器能完美契合迭代器在遍历中的用法，例如：范围for循环语句

3.size和capacity的区别size可以理解为装入对象的数量，而capacity则是对象大小的内存块的个数，两者代表的含义不一样，当对象进入容器的时候要更新size，当size大于capacity的时候，要重新分配内存，并更新capacity。

4.当size大于capacity时，我们就要重新分配内存，调用Reallocate函数，该函数申请一片连续的更大的内存，并将旧内存的对象转移至新内存中，以此来完成“复制”，然后释放旧内存。以此来达到扩容。
