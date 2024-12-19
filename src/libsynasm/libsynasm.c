#include "libsynasm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Architecture-specific parsers
extern int parse_aarch64(const char *source, AssemblyIR *ir);
extern int parse_x86_64(const char *source, AssemblyIR *ir);
extern int parse_i386(const char *source, AssemblyIR *ir);
extern int parse_m68k(const char *source, AssemblyIR *ir);

static ArchType current_arch = ARCH_UNKNOWN;

// Initialize the library for a specific architecture
int libsynasm_init(ArchType arch) {
    current_arch = arch;
    return 0;
}

// Parse assembly code into IR
AssemblyIR *libsynasm_parse(const char *source) {
    if (!source) return NULL;

    AssemblyIR *ir = malloc(sizeof(AssemblyIR));
    memset(ir, 0, sizeof(AssemblyIR));

    int result = 0;
    switch (current_arch) {
        case ARCH_AARCH64:
            result = parse_aarch64(source, ir);
            break;
        case ARCH_X86_64:
            result = parse_x86_64(source, ir);
            break;
        case ARCH_I386:
            result = parse_i386(source, ir);
            break;
        case ARCH_M68K:
            result = parse_m68k(source, ir);
            break;
        default:
            fprintf(stderr, "Error: Unsupported architecture.\n");
            free(ir);
            return NULL;
    }

    if (result != 0) {
        free(ir);
        return NULL;
    }

    return ir;
}

// Encode IR into binary
int libsynasm_encode(const AssemblyIR *ir, const char *output_file) {
    if (!ir || !output_file) return -1;

    FILE *output = fopen(output_file, "wb");
    if (!output) {
        perror("Error opening output file");
        return -1;
    }

    fwrite(ir->binary, 1, ir->binary_size, output);
    fclose(output);
    return 0;
}

// Free IR
void libsynasm_free_ir(AssemblyIR *ir) {
    if (!ir) return;
    free(ir->instructions);
    free(ir->binary);
    free(ir);
}
