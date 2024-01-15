#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define OPCODE_OFFSET 6
#define D_OFFSET 7
#define W_OFFSET 8 
#define MOD_OFFSET 10 
#define REG_OPERAND_OFFSET 13 
#define RM_OFFSET 16 

#define MASK_ONE_BIT 0b1
#define MASK_TWO_BIT 0b11
#define MASK_THREE_BIT 0b111

// Instructions

#define IN_MOV 0b100010

#define BUFFER_SIZE 16 + 1
#define BYTE_SIZE 8
#define REGISTER_SIZE 16

enum encodingMode {
    memNoDisp,
    mem8bitDisp,
    mem16bitDisp,
    registerNoDisp,
};

typedef struct {
    uint8_t reg_index;
    char* w0_name;
    char* w1_name;
} RegisterNames;

RegisterNames register_names[] = {
    {0, "al", "ax"},
    {1, "cl", "cx"},
    {2, "dl", "dx"},
    {3, "bl", "bx"},
    {4, "ah", "sp"},
    {5, "ch", "bp"},
    {6, "dh", "si"},
    {7, "bh", "di"},
};

struct BinaryFileInfo {
    FILE* binary_file;
    int file_size;
    unsigned char* buffer_contents;
};


char* map_register_name(uint8_t w_bit, uint8_t register_index) {
    RegisterNames *register_name = register_names + register_index;
    
    assert(register_index == register_name->reg_index && "Wrong register index");

    char* result = "";
    if (w_bit == 0) {
        result = register_name->w0_name;
    }
    else if (w_bit == 1) {
        result = register_name->w1_name;
    }
    else {
        printf("w_bit out of range");
    }
    return result;

}

int disassemble_instruction_line(char* dst, const unsigned int input) {
    RegisterNames register_name;

    int shift_offset = REGISTER_SIZE - OPCODE_OFFSET;
    //printf("shift_offset: %d\n", shift_offset);
    char instruction[50] = {}; 

    // Get the opcode
    unsigned int result = input >> (shift_offset);
    if (IN_MOV == result) {
        printf("Got MOV instruction\n");
        strlcat(instruction, "mov ", 4);
    }
    else {
        printf("Nothing\n");
    }

    // Get the direction bit
    bool isDestination = false;
    shift_offset = REGISTER_SIZE - D_OFFSET;
    result = (input >> shift_offset) & MASK_ONE_BIT;
    if (result == 0x01) {
        isDestination = true;
    }

    // Get the Word or Byte operation setting
    bool isWord = false;
    shift_offset = REGISTER_SIZE - W_OFFSET;
    result = (input >> shift_offset) & MASK_ONE_BIT;
    if (result == 0x01) {
        isWord = true;
    }

    // Get the register or memory mode setting bits
    shift_offset = REGISTER_SIZE - MOD_OFFSET;
    enum encodingMode reg_mem_mode = (input >> shift_offset) & MASK_TWO_BIT;
    if ((reg_mem_mode < memNoDisp) || (reg_mem_mode > registerNoDisp)) {
        printf("Error reading register/memory mode");
    }

    shift_offset = REGISTER_SIZE - REG_OPERAND_OFFSET;
    result = (input >> shift_offset) & MASK_THREE_BIT;
    if (isWord) {
        if ((result < 0) || (result > 7)) {
            printf("Error reading register field");
        }
    }
    char* reg_field = map_register_name(isWord, result);
    
    shift_offset = REGISTER_SIZE - RM_OFFSET;
    result = (input >> shift_offset) & MASK_THREE_BIT;
    if ((result < 0) || (result > 7)) {
        printf("Error reading register or memory setting");
    }
    char* rm_field = map_register_name(isWord, result);

    //printf("Destination: %d\n", isDestination);
    //printf("Word: %d\n", isWord);
    //printf("Reg mem mode: %d\n", reg_mem_mode);
    printf("Reg field: %s\n", reg_field);
    printf("Register or memory setting: %s\n", rm_field);

    strlcat(instruction, " ", sizeof(instruction));
    strlcat(instruction, reg_field, sizeof(instruction));
    strlcat(instruction, ", ", sizeof(instruction));
    strlcat(instruction, rm_field, sizeof(instruction));
    memcpy(dst, instruction, sizeof(instruction) + 1);
    printf("instruction: %s\n", instruction);
    printf("dst: %s\n", dst);
    return 0;
}

int write_to_assembly_file(const char* input) {
    const char* out_filename = "test.asm";
    FILE* out_file;
    out_file = fopen(out_filename, "w");
    fprintf(out_file, "%s", input);
    return 0; 
}

struct BinaryFileInfo read_binary_file() {
    FILE* in_file;
    const char* in_filename = "listing_0037_single_register_mov";
    unsigned char buffer_contents[BUFFER_SIZE] = {};

    in_file = fopen(in_filename, "rb");
    const size_t file_size = fread(buffer_contents, sizeof(unsigned char), BUFFER_SIZE, in_file);
    if (file_size == 0) {
        printf("Read nothing from file, exiting...\n");
        exit(EXIT_FAILURE);
    }
    struct BinaryFileInfo info = {in_file, file_size, buffer_contents};
    return info;
}

int main() {
    struct BinaryFileInfo in_file = read_binary_file();
    if (in_file.binary_file != NULL) {
        unsigned short decimal_value = 0;
        for (size_t i = 0; i < (in_file.file_size/sizeof(unsigned char)); i++) {
            decimal_value |= in_file.buffer_contents[i] << 8 * (in_file.file_size - 1 - i);
            //printf("0x%x ", buffer_contents[i]);
        }
        //printf("\ndecimal value: %x\n", decimal_value);
        char instruction[50];
        uint8_t result = disassemble_instruction_line(instruction, decimal_value);

        printf("\nfinal: %s", instruction);

        write_to_assembly_file(instruction);
    }
    fclose(in_file.binary_file);
    return 0;
}

