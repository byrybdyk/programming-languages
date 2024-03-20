#include "mem_internals.h"
#include "mem.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>


void debug(const char* fmt, ... );
struct block_header* header_from_data(void* data){
    return (struct block_header*)(data - offsetof(struct block_header, contents));
}

// Успешное выделение памяти.
void test_successful_allocation() {
    debug("Test №1: Successful memory allocation\n");
    void* ptr = _malloc(32);
    assert(ptr != NULL && "Allocation failed");
    debug_heap(stdout, HEAP_START);
    _free(ptr);
    assert(header_from_data(ptr)->is_free);
    debug_heap(stdout, HEAP_START);
    debug("Test №1 passed!\n\n");
}


// Освобождение двух блоков из нескольких выделенных.
void test_free_two_blocks() {
    debug("Test №2: Free Two Blocks\n");
    void* ptr1 = _malloc(20);
    void* ptr2 = _malloc(40);
    void* ptr3 = _malloc(60);
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr3 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(ptr2);
    assert(header_from_data(ptr2)->is_free);
    _free(ptr1);
    assert(header_from_data(ptr1)->is_free);
    debug_heap(stdout, HEAP_START);
    _free(ptr3);
    assert(header_from_data(ptr3)->is_free);
    debug_heap(stdout, HEAP_START);
    debug("Test №2 passed!\n\n");
}

// Память закончилась, новый регион памяти расширяет старый
void test_memory_extension() {
    debug("Test №3: Memory extension\n");
    heap_init(REGION_MIN_SIZE);
    void* ptr1 = _malloc(REGION_MIN_SIZE / 2);
    void* ptr2 = _malloc(REGION_MIN_SIZE);
    void* ptr3 = _malloc(REGION_MIN_SIZE / 2);
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr3 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(ptr2);
    assert(header_from_data(ptr2)->is_free);
    _free(ptr1);
    assert(header_from_data(ptr1)->is_free);
    debug_heap(stdout, HEAP_START);
    void* ptr4 = _malloc(REGION_MIN_SIZE);
    debug_heap(stdout, HEAP_START);
    _free(ptr3);
    assert(header_from_data(ptr3)->is_free);
    _free(ptr4);
    assert(header_from_data(ptr4)->is_free);
    debug_heap(stdout, HEAP_START);
    debug("Test №3 passed!\n\n");
}

// Память закончилась, старый регион памяти не расширить из-за другого выделенного диапазона адресов,
// новый регион выделяется в другом месте.
void test_new_region_allocation() {
    debug("Test №4: New region allocation\n");
    size_t size = getpagesize() * 2 - offsetof(struct block_header, contents);
    heap_init(size);
    void* ptr1 = _malloc(size);
    int* res = mmap((void*) HEAP_START + getpagesize() * 2, getpagesize() * 2, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | 0x20 | MAP_FIXED_NOREPLACE, -1, 0);
    assert(res != MAP_FAILED);
    void* ptr2 = _malloc(REGION_MIN_SIZE);
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(ptr2);
    assert(header_from_data(ptr2)->is_free);
    _free(ptr1);
    assert(header_from_data(ptr1)->is_free);
    debug_heap(stdout, HEAP_START);
    debug_heap(stdout, HEAP_START);
    munmap((void*) HEAP_START + getpagesize() * 2, getpagesize() * 2);
    debug("Test №4 passed!\n\n");
    heap_term();
}


int main() {
    heap_init(1024);

    debug("Starting tests\n\n");
    test_successful_allocation();
    test_free_two_blocks();
    test_memory_extension();
    test_new_region_allocation();
    debug("Tests successfully completed!\n");

    heap_term();

    return 0;
}
