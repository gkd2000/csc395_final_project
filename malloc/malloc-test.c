#define _XOPEN_SOURCE

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#if defined(__APPLE__)

#include <malloc/malloc.h>

#define malloc_usable_size malloc_size
#define rip(c) (c->uc_mcontext->__ss.__rip)
#define rsp(c) (c->uc_mcontext->__ss.__rsp)
#define rbp(c) (c->uc_mcontext->__ss.__rbp)
#define rax(c) (c->uc_mcontext->__ss.__rax)

#elif defined(__linux__)

#include <malloc.h>

#define rip(c) (c->uc_mcontext.gregs[REG_RIP])
#define rsp(c) (c->uc_mcontext.gregs[REG_RSP])
#define rbp(c) (c->uc_mcontext.gregs[REG_RBP])
#define rax(c) (c->uc_mcontext.gregs[REG_RAX])
#else
#error "Unsupported operating system"
#endif

/****** Test cases for grading ******/

// Test for valid allocated memory
int test_valid();

// Test for allocated object sizes
int test_sizes();

// Test to see if objects are aligned to a multiple of their size
int test_alignment();

// Test for non-overlapping objects
int test_overlapping();

// Test for reallocation of freed objects
int test_reallocation();

// Test to see if allocator blocks behave appropriately
int test_blocks();

// Test for reasonable large object behavior
int test_large_objects();

/****** Utilities ******/

// Check if a given allocation is writable
bool valid_mem(void* p, size_t sz);

// Handle segfaults for checking valid allocations
void segfault_handler(int sig, siginfo_t* info, void* c);

/****** Implementation ******/

int main(int argc, char** argv) {
  // Set up the segfault handler
  struct sigaction sa = {.sa_sigaction = segfault_handler, .sa_flags = SA_SIGINFO};

  if (sigaction(SIGSEGV, &sa, NULL) != 0) {
    perror("sigaction failed");
    exit(2);
  }

  if (sigaction(SIGBUS, &sa, NULL) != 0) {
    perror("sigaction failed");
    exit(2);
  }

  printf("BiBoP Allocator Test Results:\n\n");

  // Run tests
  int total_score = 0;
  int points_possible = 0;

  total_score += test_valid();
  points_possible += 10;

  total_score += test_sizes();
  points_possible += 10;

  total_score += test_alignment();
  points_possible += 10;

  total_score += test_overlapping();
  points_possible += 10;

  total_score += test_reallocation();
  points_possible += 10;

  total_score += test_blocks();
  points_possible += 16;

  total_score += test_large_objects();
  points_possible += 10;

  printf("Total Score: %d/%d (%.1f%%)\n", total_score, points_possible,
         100 * (float)total_score / points_possible);

  return 0;
}

/****** Tests ******/

int test_valid() {
  printf("1. Is allocated memory writable?\n");

  int score = 0;

  // Allocate ten objects and make sure they are all valid
  int sizes[] = {4, 15, 18, 23, 32, 36, 150, 447, 1025, 2000};

  for (size_t i = 0; i < 10; i++) {
    void* p = malloc(sizes[i]);
    if (valid_mem(p, sizes[i])) {
      printf("  malloc(%d) returned at least %d bytes of writable memory. (+1 point)\n", sizes[i],
             sizes[i]);
      score++;
    } else {
      printf(
          "  malloc(%d) returned pointer %p. At least some part of this memory was not writable. "
          "(+0 points)\n",
          sizes[i], p);
    }
  }

  printf(" Tests Passed: %d/10\n\n", score);

  return score;
}

int test_sizes() {
  printf("2. Are allocated objects the right size?\n");

  int score = 0;
  int sizes[] = {4, 16, 18, 35, 66, 128, 200, 511, 600, 1025};
  int expected_sizes[] = {16, 16, 32, 64, 128, 128, 256, 512, 1024, 2048};

  // Allocate ten objects and make sure they are the appropriate size
  for (size_t i = 0; i < 10; i++) {
    void* p = malloc(sizes[i]);
    size_t sz = malloc_usable_size(p);
    if (sz < sizes[i]) {
      printf("  malloc(%d) returned memory that is only %lu bytes!\n", sizes[i], sz);
    } else if (sz != expected_sizes[i]) {
      printf("  malloc(%d) did not round up to %d bytes; I only received space for %lu bytes.\n",
             sizes[i], expected_sizes[i], sz);
    } else {
      printf("  malloc(%d) returned space for %d bytes. (+1 point)\n", sizes[i], expected_sizes[i]);
      score++;
    }
  }

  printf(" Tests Passed: %d/10\n\n", score);

  return score;
}

int test_alignment() {
  printf("3. Are allocated objects size-aligned?\n");

  int score = 0;
  for (int i = 0; i < 10; i++) {
    size_t requested_size = 4 + rand() % 2044;
    void* p = malloc(requested_size);
    size_t sz = malloc_usable_size(p);
    if (sz == 0) {
      printf("  malloc(%lu) returned a pointer to zero bytes.\n", requested_size);
    } else if ((uintptr_t)p % sz != 0) {
      printf("  malloc(%lu) returned %lu bytes, but %p is not aligned to a multiple of %lu.\n",
             requested_size, sz, p, sz);
    } else {
      printf("  malloc(%lu) returned a properly-aligned pointer.\n", requested_size);
      score++;
    }
  }

  printf(" Tests Passed: %d/10\n\n", score);
  return score;
}

int test_overlapping() {
  printf("4. Do allocated objects overlap? (they shouldn't)\n");

  int overlaps = 0;

  int sz = 32;
  void* pointers[1000];

  printf("  Allocating one thousand %d byte objects...\n", sz);

  for (int i = 0; i < 1000; i++) {
    pointers[i] = malloc(sz);
    uintptr_t p1 = (uintptr_t)pointers[i];
    for (int j = 0; j < i; j++) {
      uintptr_t p2 = (uintptr_t)pointers[j];
      if ((p1 <= p2 && p1 + sz > p2) || (p2 <= p1 && p2 + sz > p1)) {
        overlaps++;
        printf("  malloc(%d) returned %p, which overlaps previous allocation at %p\n", sz,
               pointers[i], pointers[j]);
      }
    }
  }

  if (overlaps == 0) {
    printf(" No overlapping objects. (+10 points)\n\n");
    return 10;
  } else if (overlaps < 5) {
    printf(" Small number (%d) of overlapping objects. (+5 points)\n\n", overlaps);
    return 5;
  } else {
    printf(" Large number (%d) of overlapping objects. (+0 points)\n\n", overlaps);
    return 0;
  }
}

int test_reallocation() {
  printf("5. Does malloc return previously-freed objects?\n");

  int score = 0;

  // Repeat our test five times
  for (int i = 0; i < 5; i++) {
    // Choose a random object size up to 128 bytes
    size_t sz = 1 + rand() % 128;

    // How many objects should we allocate and free?
    size_t free_count = 20 + rand() % 80;

    // How many allocations should we make between these freed objects?
    size_t padding_count = rand() % 10;

    // Set up an array for allocated objects
    void* freed_pointers[free_count];

    printf("  Allocating %lu objects of %lu bytes. %lu will be freed...\n",
           free_count + padding_count * free_count, sz, free_count);

    // Allocate memory for the test
    for (int j = 0; j < free_count; j++) {
      // Allocate an object we will free
      freed_pointers[j] = malloc(sz);
      // Allocate some other objects after it
      for (int k = 0; k < padding_count; k++) {
        void* q = malloc(sz);
      }
    }

    // Free some memory
    for (int j = 0; j < free_count; j++) {
      free(freed_pointers[j]);
    }

    // Keep track of the returned memory in order
    void* returned_order[free_count];

    // How many freed objects have been returned
    size_t num_returned = 0;

    // How many mallocs have we tried?
    size_t attempts = 0;

    // Now allocate memory (up to 1000 malloc calls) to make sure we get all the freed memory back
    // eventually
    while (num_returned < free_count && attempts < 1000) {
      void* p = malloc(sz);
      for (int i = 0; i < free_count; i++) {
        if (p == freed_pointers[i]) {
          returned_order[num_returned] = p;
          num_returned++;
        }
      }

      attempts++;
    }

    if (num_returned == free_count) {
      printf("  All freed objects were returned by malloc. (+2 points)\n\n");
      score += 2;
    } else if (num_returned >= 0.9 * free_count) {
      printf("  At least 90%% of freed objects were returned from malloc. (+1 point)\n\n");
      score++;
    } else {
      printf("  Most freed objects were not returned from malloc. (+0 points)\n\n");
    }
  }

  printf(" Test Score: %d/10\n\n", score);
  return score;
}

int test_blocks() {
  printf("6. Does the allocator expand blocks appropriately?\n");

  int score = 0;

  int sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048};

  // Allocate objects of each size until we start a new page for each
  for (int i = 0; i < 8; i++) {
    // Allocate an object
    void* p = malloc(sizes[i]);

    // Find the beginning of that object's page
    uintptr_t page = (uintptr_t)p & ~0xFFF;
    uintptr_t newpage = (uintptr_t)p & ~0xFFF;

    // Allocate until we get a new page. Stop early if we allocate at least 4096 bytes and dont'
    // move to a new page.
    int allocated = 0;
    while (allocated < 4096 && page == newpage) {
      p = malloc(sizes[i]);
      allocated += sizes[i];

      // Find the page for the new allocation
      newpage = (uintptr_t)p & ~0xFFF;
    }

    // Did we fail to get a new page?
    if (page == newpage) {
      printf("  malloc(%d) failed to expand to a new page.\n", sizes[i]);
      continue;
    } else {
      // Allocate until we get yet another page, keeping track of the number of allocated bytes
      page = newpage;
      allocated = sizes[i];
      while (allocated < 4096 && page == newpage) {
        p = malloc(sizes[i]);
        allocated += sizes[i];

        // Find the page for the new allocation
        newpage = (uintptr_t)p & ~0xFFF;
      }

      // Did we fail to move to a new page?
      if (page == newpage) {
        printf("  malloc(%d) failed to expand to a new page.\n", sizes[i]);
        continue;
      } else if (allocated < 4096 - sizes[i] - 100) {
        // Ballpark a large page header at 100 bytes
        printf("  malloc(%d) moved to a new page, but wasted a full object. (+1 point)\n",
               sizes[i]);
        score++;
      } else {
        printf("  malloc(%d) moved to a new page and did not waste too much space. (+2 points)\n",
               sizes[i]);
        score += 2;
      }
    }
  }

  printf(" Test Score: %d/16\n\n", score);

  return score;
}

int test_large_objects() {
  printf("7. Does malloc work for large objects?\n");

  int score = 0;

  void* pointers[10];
  int sizes[10];

  for (int i = 0; i < 10; i++) {
    int size = 2049 + rand() % 8192;
    void* p = malloc(size);
    pointers[i] = p;
    sizes[i] = size;

    if (!valid_mem(p, size)) {
      printf("  malloc(%d) returned memory that was not writable.\n", size);
    } else {
      uintptr_t p1 = (uintptr_t)pointers[i];
      int sz1 = sizes[i];

      // Does the new allocation overlap a previous one?
      bool overlap = false;
      for (int j = 0; j < i; j++) {
        uintptr_t p2 = (uintptr_t)pointers[j];
        int sz2 = sizes[j];
        if ((p2 >= p1 && p2 < p1 + sz1) || (p1 >= p2 && p1 < p2 + sz2)) {
          overlap = true;
        }
      }

      if (overlap) {
        printf("  malloc(%d) returned memory that overlaps a previous allocation.\n", size);
      } else {
        printf("  malloc(%d) returned writable, non-overlapping memory. (+1 point)\n", size);
        score++;
      }
    }
  }

  printf(" Test Score: %d/10\n\n", score);
  return score;
}

/****** Utilities ******/

bool valid_mem(void* p, size_t sz) {
  uint8_t* ptr = (uint8_t*)p;

  // Write each byte of the memory. If we make it through, it's valid.
  for (size_t i = 0; i < sz; i++) {
    *ptr = 0xcc;
    ptr++;
  }

  // If a segfault occurs we will not actually reach this point
  return true;
}

void segfault_handler(int sig, siginfo_t* info, void* c) {
  printf("  UH OH! There was a segfault when running this test.\n\n");

  ucontext_t* context = (ucontext_t*)c;

  // Access the ucontext_t to get the pre-signal register state
  void** frame_pointer = (void**)rbp(context);

  // Set up a false return value
  rax(context) = 0;

  // Do the return
  rip(context) = (uint64_t)frame_pointer[1];
  rsp(context) = (uint64_t)(frame_pointer + 2);
  rbp(context) = (uint64_t)frame_pointer[0];
}
