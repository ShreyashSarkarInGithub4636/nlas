#ifndef LIBSYNASM_H
#define LIBSYNASM_H

#include <stdint.h>
#include <stddef.h>

// Supported architectures
typedef enum {
    ARCH_AARCH64,
    ARCH_X86_64,
    ARCH_I386,
    ARCH_M68K,
    ARCH_UNKNOWN
} ArchType;

// Instruction structure
typedef struct {
    char *mnemonic;
    char *operands;
    uint32_t encoding;
} Instruction;

// Intermediate representation (IR) for parsed assembly
typedef struct {
    Instruction *instructions;
    size_t instruction_count;
    uint8_t *binary;
    size_t binary_size;
} AssemblyIR;

// Library API
#ifdef __cplusplus
extern "C" {
#endif

// Initialize the library for a specific architecture
int libsynasm_init(ArchType arch);

// Parse assembly code into IR
AssemblyIR *libsynasm_parse(const char *source);

// Encode IR into binary (e.g., executable or ROM)
int libsynasm_encode(const AssemblyIR *ir, const char *output_file);

// Free IR
void libsynasm_free_ir(AssemblyIR *ir);

#ifdef __cplusplus
}
#endif

#endif // LIBSYNASM_H
