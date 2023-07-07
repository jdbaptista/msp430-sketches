/*
 * disassembler.c
 *
 *  Created on: Jul 5, 2023
 *      Author: Jaden Baptista
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "include/disassembler.h"

/**
 * Does not append a space!
 */
void appendOperand(char** result, char* reg, addressingMode mode, char* offset) {
    switch (mode) {
        case REGISTER:
            strcat(*result, reg); // result: "R15"
            break;
        case INDEXED:
            if (offset == NULL) {
                strcat(*result, "ERROR");
            } else {
                strcat(*result, offset);
                strcat(*result, "(");
                strcat(*result, reg);
                strcat(*result, ")"); // result: "FFFFh(R15)"
            }
            break;
        case INDIRECT:
            strcpy(*result, "@");
            strcpy(*result, reg); // result: "@R15"
            break;
        case AUTOINCREMENT:
            strcpy(*result, "@");
            strcpy(*result, reg);
            strcpy(*result, "+"); // result: "@R15+"
            break;
        default:
            strcpy(*result, "ERROR"); // ADDRESSINGERROR
            break;
    }
}

void getAsm(char** result, uint16_t byteCode, opCode* opCode, char* srcOffset, char* destOffset) {
    if (opCode->format == FORMATERROR) {
        strcpy(*result, opCode->repr);
        return;
    }
    strcpy(*result, opCode->repr); // result: "MOV"
    if (isByteOperation(byteCode)) {
        strcat(*result, ".B "); // result: "MOV.B "
    } else {
        strcat(*result, " "); // result: "MOV "
    }
    switch(opCode->format) {
        case DOUBLE: {
            addressingMode destMode = getDestRegisterMode(byteCode, DOUBLE);
            addressingMode srcMode = getSourceRegisterMode(byteCode, DOUBLE);
            char* dest = parseRegisterNum(getDestRegister(byteCode, DOUBLE));
            char* src = parseRegisterNum(getSourceRegister(byteCode, DOUBLE));
            appendOperand(result, src, srcMode, srcOffset);
            strcat(*result, " ");
            appendOperand(result, dest, destMode, destOffset);
            break;
        }
        case SINGLE: {
            addressingMode destMode = getDestRegisterMode(byteCode, SINGLE);
            char* dest = parseRegisterNum(getDestRegister(byteCode, SINGLE));
            appendOperand(result, dest, destMode, destOffset);
            break;
        }
        case JUMP: {
            char* pcOffset = getJumpOffset(byteCode);
            strcat(*result, pcOffset);
            break;
        }
        default:
            strcpy(*result, "ERROR");
            break;
    }
}




// NOTE: order matters here! some masks are submasks of others!
opCode getOpCode(uint16_t byteCode) {
    // Note: Order of opcodes is set in disassembler.h
    //       and is made so submasks are checked late.
    unsigned int i = 0;
    opCode currCode;
    while (true) {  // danger is my middle name
        currCode = CODES[i];
        bool doneIterating = currCode.format == FORMATERROR;
        bool isCorrectCode = (currCode.mask & byteCode) == currCode.mask;
        if (doneIterating || isCorrectCode) {
            return currCode; // only exit
        }
        i++;
    }
}

addressingMode getDestRegisterMode(uint16_t byteCode, formatType type) {
    if (type != DOUBLE) {
        return ADDRESSINGERROR;
    }
    switch (byteCode & 0x0080) {
        case 0x0080:
            return INDEXED;
        case 0x0000:
            return REGISTER;
        default:
            return ADDRESSINGERROR;
    }
}

addressingMode getSourceRegisterMode(uint16_t byteCode, formatType type) {
    if (type != DOUBLE && type != SINGLE) {
        return ADDRESSINGERROR;
    }
    switch (byteCode & 0x0030) {
        case 0x0030:
            return AUTOINCREMENT;
        case 0x0020:
            return INDIRECT;
        case 0x0010:
            return INDEXED;
        case 0x0000:
            return REGISTER;
        default:
            return ADDRESSINGERROR;
    }
}

uint16_t getDestRegister(uint16_t byteCode, formatType type) {
    if (type != DOUBLE) {
        return 0;
    }
    return (byteCode & 0x000F);
}

uint16_t getSourceRegister(uint16_t byteCode, formatType type) {
    switch (type) {
    case DOUBLE:
        return (byteCode & 0x0F00);
    case SINGLE:
        return (byteCode & 0x000F);
    default:
        return 0;
    }
}

bool isByteOperation(uint16_t byteCode) {
    return (byteCode & 0x0040);
}

char* getJumpOffset(uint16_t byteCode) {
    int16_t signedOffset = (int16_t) (byteCode & 0x03FF);
    // TODO: Implement function (store string on heap)
    return NULL;
}

char* byteCodeToOffset(uint16_t byteCode) {
    // TODO: Implement function (store string on heap)
    int16_t signedOffset = (int16_t) byteCode;
    return NULL;
}

char* parseRegisterNum(uint16_t regNum) {
    // TODO: Implement function (store string on heap)
    return NULL;
}
