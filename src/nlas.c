#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "libsynasm.h"

#define ROM_HEADER_SIZE 512

typedef enum {
    ARCH_AARCH64,
    ARCH_X86_64,
    ARCH_I386,
    ARCH_SGS_M68K,
    ARCH_UNKNOWN
} ArchType;

// Helper function: Get opcode using libsynasm
uint32_t get_opcode(const char *mnemonic, const char *operand, ArchType arch) {
    return libsynasm_get_opcode(mnemonic, operand, arch);
}

// Assembler logic for each architecture
void assemble(const char *input_file, const char *output_file, ArchType arch, const char *include_file) {
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "wb");
    if (!input || !output) {
        perror("Error opening file");
        exit(1);
    }

    // Write ROM header for m68k
    if (arch == ARCH_SGS_M68K) {
        uint8_t header[ROM_HEADER_SIZE] = {0};
        fwrite(header, sizeof(uint8_t), ROM_HEADER_SIZE, output);
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        char mnemonic[16], operand[16];
        if (sscanf(line, "%s %s", mnemonic, operand) == 2) {
            uint32_t opcode = get_opcode(mnemonic, operand, arch);
            if (opcode == 0xFFFFFFFF) {
                fprintf(stderr, "Invalid instruction: %s\n", mnemonic);
                exit(1);
            }

            fwrite(&opcode, sizeof(uint32_t), 1, output);
            uint32_t imm = atoi(operand);
            fwrite(&imm, sizeof(uint32_t), 1, output);
        }
    }

    // Handle .incbin directive if an include file is specified
    if (include_file) {
        FILE *include = fopen(include_file, "rb");
        if (!include) {
            perror("Error opening include file");
            exit(1);
        }
        uint8_t buffer[1024];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), include)) > 0) {
            fwrite(buffer, 1, bytes_read, output);
        }
        fclose(include);
    }

    fclose(input);
    fclose(output);
    printf("Assembled file to %s\n", output_file);
}

// Detect architecture from string
ArchType detect_arch(const char *arch) {
    if (strcmp(arch, "aarch64") == 0) return ARCH_AARCH64;
    if (strcmp(arch, "x86_64") == 0) return ARCH_X86_64;
    if (strcmp(arch, "i386") == 0) return ARCH_I386;
    if (strcmp(arch, "sgs-m68k") == 0) return ARCH_SGS_M68K;
    return ARCH_UNKNOWN;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: nlas <input> -o <output> --arch <arch> [-I <include>]\n");
        return 1;
    }

    const char *input_file = NULL;
    const char *output_file = NULL;
    const char *arch_name = NULL;
    const char *include_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--arch") == 0 && i + 1 < argc) {
            arch_name = argv[++i];
        } else if (strcmp(argv[i], "-I") == 0 && i + 1 < argc) {
            include_file = argv[++i];
        } else {
            input_file = argv[i];
        }
    }

    if (!input_file || !output_file || !arch_name) {
        fprintf(stderr, "Error: Missing required arguments\n");
        return 1;
    }

    ArchType arch = detect_arch(arch_name);
    if (arch == ARCH_UNKNOWN) {
        fprintf(stderr, "Error: Unknown architecture '%s'\n", arch_name);
        return 1;
    }

    assemble(input_file, output_file, arch, include_file);

    return 0;
}
