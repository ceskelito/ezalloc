# ezalloc

A basic C garbage collector system. Call `ez_alloc()`/`ez_calloc()` instead of `malloc()`/`calloc()` to allocate memory. Then, forget about your pointers and just call `ez_cleanup()` at the end of your program to free everything. No leaks, no stress. Have a nice day!

## Features

- **Simple API**: Drop-in replacement for malloc/calloc
- **Automatic cleanup**: One function call frees all tracked memory
- **Group support**: Organize allocations into named groups for selective cleanup
- **Error handling**: Proper error reporting with perror
- **Safe**: Integer overflow protection in calloc operations

## Basic Usage (ez_* functions)

```c
#include "ezalloc.h"

int main(void)
{
    // Allocate memory - it's automatically tracked
    int *numbers = ez_alloc(sizeof(int) * 10);
    char *text = ez_calloc(sizeof(char), 100);  // Zeroed memory
    
    // Use your memory as normal
    numbers[0] = 42;
    strcpy(text, "Hello, ezalloc!");
    
    // Add external pointers to tracking
    void *external = malloc(1024);
    ez_add(external);
    
    // Free specific pointer if needed
    ez_release(numbers);
    
    // At the end, free everything at once
    ez_cleanup();
    
    return 0;
}
```

## Group-based Usage (ezg_* functions)

Organize allocations into named groups for more control:

```c
#include "ezgalloc.h"

int main(void)
{
    // Create groups
    ezg_group_create("config");
    ezg_group_create("temp");
    
    // Allocate in specific groups
    char *config = ezg_alloc("config", 1024);
    int *temp_data = ezg_alloc("temp", sizeof(int) * 100);
    
    // Release all memory in a specific group
    ezg_group_release("temp");
    
    // Or delete the group entirely
    ezg_group_delete("config");
    
    // Cleanup all groups
    ezg_cleanup();
    
    return 0;
}
```

## API Reference

### Basic Functions (ezalloc.h)

- `void *ez_alloc(size_t size)` - Allocate and track memory
- `void *ez_calloc(size_t size, size_t count)` - Allocate zeroed memory
- `void *ez_add(void *ptr)` - Track existing pointer
- `void ez_release(void *ptr)` - Free specific pointer
- `void ez_cleanup(void)` - Free all tracked memory

### Group Functions (ezgalloc.h)

- `int ezg_group_create(char *group)` - Create a named group
- `void *ezg_alloc(char *group, size_t size)` - Allocate in group
- `void *ezg_calloc(char *group, size_t size, size_t count)` - Allocate zeroed in group
- `void *ezg_add(char *group, void *ptr)` - Add pointer to group
- `void ezg_release(char *group, void *ptr)` - Free specific pointer in group
- `void ezg_group_release(char *group)` - Free all memory in group
- `void ezg_group_delete(char *group)` - Delete group and free its memory
- `void ezg_cleanup(void)` - Free all groups

## Building

```bash
make        # Build library
make clean  # Clean build artifacts
make re     # Rebuild from scratch
```

This creates `libezalloc.a` which you can link with your project:

```bash
gcc your_program.c -L. -lezalloc -o your_program
```

## Testing

Run the included test suite:

```bash
gcc test.c -L. -lezalloc -o test
./test
```

## Example (from the original repository)

<img width="497" alt="Screen Shot 2023-02-04 at 2 14 54 PM" src="https://user-images.githubusercontent.com/95633668/216769739-afd10d33-86ff-4fad-8820-ba0f3e1110fe.png">




Output:

<img width="180" alt="Screen Shot 2023-02-04 at 1 17 51 PM" src="https://user-images.githubusercontent.com/95633668/216767370-1b619b09-5fb4-4719-85b1-f44a44c51a4a.png">


<img width="494" alt="Screen Shot 2023-02-04 at 1 18 11 PM" src="https://user-images.githubusercontent.com/95633668/216767374-17eb0d92-081e-46a1-9f9d-0d6409174c74.png">
