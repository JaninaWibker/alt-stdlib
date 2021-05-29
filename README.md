# alternative stdlib

> **WIP**

This is intended to be a replacement for some of the standard library data structures as well as (maybe) other utilities (such as optionals).

This project is heavily inspired by [stb](https://github.com/nothings/stb), [serenityOS](https://github.com/SerenityOS/serenity), my operating systems class and my algorithms class at university.

There is a `common.hpp` file with some commonly used things but other than that everything is self-contained and can just be included as is.

## components

### (unbounded) array

An unbounded array implementation with a growth factor of 2.

> *TODO*


### hashtable

A hashtable implementation based on a unique technique taking inspiration from both doubly-linked lists and linear probing.

> The hashtable uses doubly-linked lists for saving values with hashes that clash but instead of allocating these elements randomly using malloc this is done manually.
> The twist is that all elements are allocated inside of the hashtable itself using a freelist which also spans across the hashtable containing all currently free "slots"
> Whenever a slot needs to be used it is unhooked from the freelist; whenever a slot is free'd it is returned to the freelist

**hashtableinit**: Create a new hashtable

Create a new hashtable using an initial capacity and a given hash function
Use as follows (`[x]` marks things to replace):
```c
auto hash = []([your type] x) -> int { [your hash implementation] };
auto ht = hashtableinit<[your type], decltype(hash)>([initial capacity], hash);
```

`hashtable<T, H> hashtableinit(size_t capacity, H hash)`


**hashtablefree**: Free hashtable

`void hashtablefree(hashtable<T, H>* ht)`


**::resize**: Resize the hashtable

This can be called manually but will also be called internally whenever needed.
It might be useful to call this if you already know that you'll add a specific amount of items soon.

`void hashtable<T, H>::resize(size_t new_size)`


**::dbg**: Print debug information about the state of the hashtable

`void hashtable<T, H>::dbg()`


**::dbg**: Print debug information about the state of the hashtable

Includes a name which will be displayed alongside the debug information to tell debug logs apart.

`void hashtable<T, H>::dbg(char* name)`


**::ins**: Insert something into the hashtable

If the element already exists it will be overwritten.
Handles hash collisions.

`T hashtable<T, H>::ins(T value)`


**::del**: Delete something from the hashtable

Returns wether or not the action was successful.
Handles hash collisions.

`bool hashtable<T, H>::del(T value)`


**::fnd**: Search for a value in the hashtable

`T hashtable<T, H>::fnd(T value)`

**::fnd**: Search for a value in the hashtable by predicate

> **Warning**: Expensive action

`T hashtable<T, H>::fnd(F&& pred)`


**::has**: Check if the hashtable contains a value

`bool hashtable<T, H>::has(T value)`


**::has**: Check if the hashtable contains a value by predicate lambda

> **Warning**: Expensive action

`bool hashtable<T, H>::has(F&& pred)`


**::each**: Iterate through all elements
Iterate through all elements of the hashtable and call fn for every one of them

> **Warning**: Expensive action

`void hashtable<T, H>::each(F&& fn)`


**::clr**: Clear out all elements of the hashtable

> **Warning**: Expensive action

`void hashtable<T, H>::clr()`


**::filter**: Filter hashtable elements

Takes a predicate lambda which decides which elements can stay and which get removed.

> **Warning**: Expensive action

`void hashtable<T, H>::filter(F&& pred)`


**::cat**: Concatenate hashtables
Combine another hashtable with the current one

This goes through all elements of the other hashtable and adds the elements to the current one.

> **Warning**: Expensive action

`void hashtable<T, H>::cat(hashtable<T, H> other)`



### hashmap

A hashmap implementation based on the hashtable implementation inspired by doubly-linked lists, linear probing and freelists.

**hashmapinit**: Create hashmap

`hashmap<K, V, H> hashmapinit(size_t capacity, H hash)`

**hashmapfree**: Free hashmap

`void hashmapfree(hashmap<K, V, H>* hm)`


**::resize**: Resize the hashmap

This can be called manually but will also be called internally whenever needed.
It might be useful to call this if you already know that you'll add a specific amount of items soon.

`void hashmap<K, V, H>::resize(size_t new_size)`


**::dbg**: Print debug information about the state of the hashmap

`void hashmap<K, V, H>::dbg()`


**::dbg**: Print debug information about the state of the hashmap

Includes a name which will be displayed alongside the debug information to tell debug logs apart.

`void hashmap<K, V, H>::dbg(char* name)`


**::ins**: Insert something into the hashmap

If an element with the same key already exists it will be overwritten.
Handles hash collisions.

`V hashmap<K, V, H>::ins(K key, V value)`


**::del**: Delete something from the hashmap by key

Returns the value of the deleted element.
Handles hash collisions.

`V hashmap<K, V, H>::del(K key)`


**::fnd**: Search for a value in the hashmap (by key)

`V hashmap<K, V, H>::fnd(K key)`


**::fnd**: Search for a value in the hashmap by predicate (called with both key and value)

> **Warning**: Expensive action

`template<class F> V hashmap<K, V, H>::fnd(F&& pred)`
   

**::has**: Check if the hashmap contains a value (by key)

`bool hashmap<K, V, H>::has(K key)`


**::has**: Check if the hashmap contains a value by predicate lambda (called with both key and value)

> **Warning**: Expensive action

`template<class F> bool hashmap<K, V, H>::has(F&& pred)`


**::each**: Iterate through all elements of the hashmap and call fn for every one of them (with both key and value)

> **Warning**: Expensive action

`template<class F> void hashmap<K, V, H>::each(F&& fn)`


**::clr**: Clear out all elements of the hashmap

> **Warning**: Expensive action

`void hashmap<K, V, H>::clr()`
   

**::filter**: Filter hashmap elements.

Takes a predicate lambda which decides which elements can stay and which get removed (called with both key and value).

> **Warning**: Expensive action

`template<class F> void hashmap<K, V, H>::filter(F&& pred)`


**::cat**: Merge another hashmap with the current one

This goes through all elements of the other hashmap and adds the elements to the current one.

> **Warning**: Expensive action

`void hashmap<K, V, H>::cat(hashmap<K, V, H> other)`


### minheap

A minheap implementation.


**minheapinit**: Create a new minheap using an initial capacity.

`template<class T> static minheap<T> minheapinit(size_t capacity)`


**minheapfree**: Free minheap

`template<class T> void minheapfree(minheap<T> heap)`


**::resize**: Resize the minheap

This can be called manually but will also be called internally whenever needed.
It might be useful to call this if you already know that you'll add a specific amount of items soon.

`template<class T> void minheap<T>::resize(size_t new_size)`


**::dbg**: Print debug information about the state of the minheap

`template<class T> void minheap<T>::dbg()`


**::dbg**: Print debug information about the state of the minheap

Includes a name which will be displayed alongside the debug information to tell debug logs apart.

`template<class T> void minheap<T>::dbg(char* name)`


**::insert**: Insert something into the minheap

O(log n)

`template<class T> void minheap<T>::insert(T el)`


**::sift_up**: Correct minheap invariant from the bottom up

Mostly used internally.
It is assumed that the minheap invariant holds true everywhere except at m_data[i]

`template<class T> void minheap<T>::sift_up(size_t i)`


**::sift_down**: Correct minheap invariant from the top down

Mostly used internally.
It is assumed that the minheap invariant holds true everywhere except at L_CHILD(i) and R_CHILD(i).

`template<class T> void minheap<T>::sift_down(size_t i)`


**::min**: Get the minimum element of the minheap

O(1)

`template<class T> T minheap<T>::min()`


**::delete_min**: Delete minimum element of minheap and return it

O(log n)

`template<class T> T minheap<T>::delete_min()`
