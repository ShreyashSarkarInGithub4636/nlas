#include "libsynasm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define a structure for the instruction and its operands
typedef struct {
    char *mnemonic;
    char *operands;
    uint16_t encoding;
} Instruction;

typedef struct {
    Instruction *instructions;
    size_t instruction_count;
    uint8_t *binary;
    size_t binary_size;
} AssemblyIR;

// Example function to parse m68k instructions
int parse_m68k(const char *source, AssemblyIR *ir) {
    const char *example_instruction = "move.w #1234, D0";  // Example instruction for parsing

    // Simulating parsing of the "move.w #1234, D0" instruction
    if (strstr(source, example_instruction)) {
        ir->instructions = malloc(sizeof(Instruction));
        if (ir->instructions == NULL) {
            perror("Error allocating memory for instructions");
            return -1;
        }

        ir->instructions[0].mnemonic = strdup("move.w");
        ir->instructions[0].operands = strdup("#1234, D0");
        ir->instructions[0].encoding = 0x303C;  // Example opcode for "move.w #1234, D0"
        ir->instruction_count = 1;

        // Allocate memory for binary output (Sega Genesis ROM header)
        ir->binary = malloc(512);
        if (ir->binary == NULL) {
            perror("Error allocating memory for binary output");
            return -1;
        }
        memset(ir->binary, 0, 512);  // Initialize with zero (Sega Genesis header)
        
        // Example instruction encoding
        ir->binary[0] = 0x30;      // Move instruction encoding
        ir->binary[1] = 0x3C;
        ir->binary_size = 512;      // ROM header size (for simplicity)

    } else {
        fprintf(stderr, "Error: Unknown instruction\n");
        return -1;
    }

    return 0;
}

void free_assembly_ir(AssemblyIR *ir) {
    if (ir) {
        for (size_t i = 0; i < ir->instruction_count; i++) {
            free(ir->instructions[i].mnemonic);
            free(ir->instructions[i].operands);
        }
        free(ir->instructions);
        free(ir->binary);
    }
}

int main() {
    const char *source = "move.w #1234, D0"; // Example source to be parsed
    AssemblyIR ir = {0};

    // Parse the assembly source and generate binary
    if (parse_m68k(source, &ir) == 0) {
        printf("Assembly parsed successfully!\n");
        printf("Instruction: %s %s\n", ir.instructions[0].mnemonic, ir.instructions[0].operands);
        printf("Binary size: %zu\n", ir.binary_size);
        
        // For demonstration: print first few bytes of binary output
        for (size_t i = 0; i < 10; i++) {
            printf("%02X ", ir.binary[i]);
        }
        printf("\n");

        // Free allocated memory
        free_assembly_ir(&ir);
    } else {
        fprintf(stderr, "Failed to parse assembly\n");
    }

    return 0;
}
