/*
 * This code is licensed under the GNU AGPLv3
 * Copyright (c) 2025 GokbakarE
 * Date: 28-08-2025
 */

#include "FR.hpp"
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <string>
#include <string_view>
#include <format>

static const std::unordered_map<std::string_view, std::string_view> OpcodeMap = {
    {"10100100iiiirrrr", "ADD #$I, $R"},
    {"10100101iiiirrrr", "ADD2 #-$I, $R"},
    {"10100111jjjjrrrr", "ADDC $J, $R"},
    {"10100010jjjjrrrr", "ADDN $J, $R"},
    {"10100000iiiirrrr", "ADDN #$I, $R"},
    {"10100001iiiirrrr", "ADDN2 #-$I, $R"},
    {"10101100jjjjrrrr", "SUB $J, $R"},
    {"10101101jjjjrrrr", "SUBC $J, $R"},
    {"10101110jjjjrrrr", "SUBN $J, $R"},
    {"10101010jjjjrrrr", "CMP $J, $R"},
    {"10101000iiiirrrr", "CMP #$I, $R"},
    {"10101001iiiirrrr", "CMP2 #-$I, $R"},
    {"10000010jjjjrrrr", "AND $J, $R"},
    {"10000100jjjjrrrr", "AND $J, @$R"},
    {"10000101jjjjrrrr", "ANDH $J, @$R"},
    {"10000110jjjjrrrr", "ANDB $J, @$R"},
    {"10010010jjjjrrrr", "OR $J, $R"},
    {"10010100jjjjrrrr", "OR $J, @$R"},
    {"10010101jjjjrrrr", "ORH $J, @$R"},
    {"10010110jjjjrrrr", "ORB $J, @$R"},
    {"10011010jjjjrrrr", "EOR $J, $R"},
    {"10011100jjjjrrrr", "EOR $J, @$R"},
    {"10011101jjjjrrrr", "EORH $J, @$R"},
    {"10011110jjjjrrrr", "EORB $J, @$R"},
    {"10000000iiiirrrr", "BANDL #$I, @$R"},
    {"10000001iiiirrrr", "BANDH #$I, @$R"},
    {"10010000iiiirrrr", "BORL #$I, @$R"},
    {"10010001iiiirrrr", "BORH #$I, @$R"},
    {"10011000iiiirrrr", "BEORL #$I, @$R"},
    {"10011001iiiirrrr", "BEORH #$I, @$R"},
    {"10001000iiiirrrr", "BTSTL #$I, @$R"},
    {"10001001iiiirrrr", "BTSTH #$I, @$R"},
    {"10101111jjjjrrrr", "MUL $J, $R"},
    {"10101011jjjjrrrr", "MULU $J, $R"},
    {"10111111jjjjrrrr", "MULH $J, $R"},
    {"10111011jjjjrrrr", "MULUH $J, $R"},
    {"100101110100rrrr", "DIV0S $R"},
    {"100101110101rrrr", "DIV0U $R"},
    {"100101110110rrrr", "DIV1 $R"},
    {"100101110111rrrr", "DIV2 $R"},
    {"1001111101100000", "DIV3"},
    {"1001111101110000", "DIV4S"},
    {"10110110jjjjrrrr", "LSL $J, $R"},
    {"10110100iiiirrrr", "LSL #$I, $R"},
    {"10110101iiiirrrr", "LSL2 #$I, $R"},
    {"10110010jjjjrrrr", "LSR $J, $R"},
    {"10110000iiiirrrr", "LSR #$I, $R"},
    {"10110001iiiirrrr", "LSR2 #$I, $R"},
    {"10111010jjjjrrrr", "ASR $J, $R"},
    {"10111000iiiirrrr", "ASR #$I, $R"},
    {"10111001iiiirrrr", "ASR2 #$I, $R"},
    {"100111111000rrrr", "LDI:32 #, $R"},
    {"100110119999rrrr", "LDI:20 #$9, $R"},
    {"1100oooooooorrrr", "LDI:8 #$O, $R"},
    {"00000100jjjjrrrr", "LD @$J, $R"},
    {"00000000jjjjrrrr", "LD @(R13, $J), $R"},
    {"0010ddddddddrrrr", "LD @(R14, $D), $R"},
    {"000000116666rrrr", "LD @(R15, $6), $R"},
    {"000001110000rrrr", "LD @R15+, $R"},
    {"000001111000ssss", "LD @R15+, $S"},
    {"0000011110010000", "LD @R15+, PS"},
    {"00000101jjjjrrrr", "LDUH @$J, $R"},
    {"00000001jjjjrrrr", "LDUH @(R13, $J), $R"},
    {"010099999999rrrr", "LDUH @(R14, $9), $R"},
    {"00000110jjjjrrrr", "LDUB @$J, $R"},
    {"00000010jjjjrrrr", "LDUB @(R13, $J), $R"},
    {"011088888888rrrr", "LDUB @(R14, $8), $R"},
    {"00010100jjjjrrrr", "ST $R, @$J"},
    {"00010000jjjjrrrr", "ST $R, @(R13, $J)"},
    {"0011ddddddddrrrr", "ST $R, @(R14, $D)"},
    {"000100116666rrrr", "ST $R, @(R15, $6)"},
    {"000101110000rrrr", "ST $R, @-R15"},
    {"000101111000ssss", "ST $S, @-R15"},
    {"0001011110010000", "ST PS, @-R15"},
    {"00010101jjjjrrrr", "STH $R, @$J"},
    {"00010001jjjjrrrr", "STH $R, @(R13, $J)"},
    {"010199999999rrrr", "STH $R, @(R14, $9)"},
    {"00010110jjjjrrrr", "STB $R, @$J"},
    {"00010010jjjjrrrr", "STB $R, @(R13, $J)"},
    {"011188888888rrrr", "STB $R, @(R14, $8)"},
    {"10001011jjjjrrrr", "MOV $J, $R"},
    {"10110111yyyyrrrr", "MOV $Y, $R"},
    {"000101110001rrrr", "MOV PS, $R"},
    {"10110011yyyyrrrr", "MOV $R, $Y"},
    {"000001110001rrrr", "MOV $R, PS"},
    {"100101110000rrrr", "JMP @$R"},
    {"11010lllllllllll", "CALL $L"},
    {"100101110001rrrr", "CALL @$R"},
    {"1001011100100000", "RET"},
    {"00011111uuuuuuuu", "INT #$U"},
    {"1001111100110000", "INTE"},
    {"1001011101100000", "RETI"},
    {"1110bbbbnnnnnnnn", "$B $N"},
    {"100111110000rrrr", "JMP:D @$R"},
    {"11011lllllllllll", "CALL:D $L"},
    {"100111110001rrrr", "CALL:D @$R"},
    {"1001111100100000", "RET:D"},
    {"1111bbbbnnnnnnnn", "$B:D $N"},
    {"00001000zzzzzzzz", "DMOV $Z, R13"},
    {"00011000zzzzzzzz", "DMOV R13, @$Z"},
    {"00001100zzzzzzzz", "DMOV @$Z, @R13+"},
    {"00011100zzzzzzzz", "DMOV @R13+, @$Z"},
    {"00001011zzzzzzzz", "DMOV @$Z, @-R15"},
    {"00011011zzzzzzzz", "DMOV @R15+, @$Z"},
    {"00001001xxxxxxxx", "DMOVH @$X, R13"},
    {"00011001xxxxxxxx", "DMOVH R13, @$X"},
    {"00001101xxxxxxxx", "DMOVH @$X, @R13+"},
    {"00011101xxxxxxxx", "DMOVH @R13+, @$X"},
    {"00001010cccccccc", "DMOVB @$C, R13"},
    {"00011010cccccccc", "DMOVB R13, @$C"},
    {"00001110cccccccc", "DMOVB @$C, @R13+"},
    {"00011110cccccccc", "DMOVB @R13+, @$C"},
    {"10111100iiiirrrr", "LDRES @$R+, #$I"},
    {"10111101iiiirrrr", "STRES #$I, @$R+"},
    {"1001111110100000", "NOP"},
    {"10000011uuuuuuuu", "ANDCCR #$U"},
    {"10010011uuuuuuuu", "ORCCR #$U"},
    {"10000111uuuuuuuu", "STILM #$U"},
    {"10100011ssssssss", "ADDSP #$S"},
    {"100101111000rrrr", "EXTSB $R"},
    {"100101111001rrrr", "EXTUB $R"},
    {"100101111010rrrr", "EXTSH $R"},
    {"100101111011rrrr", "EXTUH $R"},
    {"10001100eeeeeeee", "LDM0 ($E)"},
    {"10001101eeeeeeee", "LDM1 ($E)"},
    {"10001110eeeeeeee", "STM0 ($E)"},
    {"10001111eeeeeeee", "STM1 ($E)"},
    {"00001111aaaaaaaa", "ENTER #$A"},
    {"1001111110010000", "LEAVE"},
    {"10001010jjjjrrrr", "XCHB @$J, $R"},
};

static const std::unordered_map<std::string_view, std::string_view> registerMap = {
    {"0000", "R0"}, {"0001", "R1"}, {"0010", "R2"}, {"0011", "R3"},
    {"0100", "R4"}, {"0101", "R5"}, {"0110", "R6"}, {"0111", "R7"},
    {"1000", "R8"}, {"1001", "R9"}, {"1010", "R10"}, {"1011", "R11"},
    {"1100", "R12"}, {"1101", "R13"}, {"1110", "R14"}, {"1111", "R15"}
};

static const std::unordered_map<std::string_view, std::string_view> specialRegisterMap = {
    {"0000", "TBR"}, {"0001", "RP"}, {"0010", "SSP"}, {"0011", "USP"},
    {"0100", "MDH"}, {"0101", "MDL"}
};

static const std::unordered_map<std::string_view, std::string_view> branchMap = {
    {"0000", "BRA"}, {"0001", "BNO"}, {"0010", "BEQ"}, {"0011", "BNE"},
    {"0100", "BC"}, {"0101", "BNC"}, {"0110", "BN"}, {"0111", "BP"},
    {"1000", "BV"}, {"1001", "BNV"}, {"1010", "BLT"}, {"1011", "BGE"},
    {"1100", "BLE"}, {"1101", "BGT"}, {"1110", "BLS"}, {"1111", "BHI"}
};

std::string FR(const std::string_view binaryCode) {
    for (const auto& [pattern, assembly] : OpcodeMap) {
        bool match = true;
        for (size_t i = 0; i < pattern.size(); ++i) {
            if (std::isalpha(pattern[i])) continue;
            if (pattern[i] != binaryCode[i]) {
                match = false;
                break;
            }
        }
        if (!match) continue;

        auto substituteRegister = [&](size_t pos, size_t len) -> std::string {
            std::string regBits = std::string(binaryCode.substr(pos, len));
            return std::string(registerMap.at(regBits));
        };

        auto substituteSpecialRegister = [&](size_t pos, size_t len) -> std::string {
            std::string regBits = std::string(binaryCode.substr(pos, len));
            return std::string(specialRegisterMap.at(regBits));
        };

        auto substituteBranch = [&](size_t pos, size_t len) -> std::string {
            std::string branchBits = std::string(binaryCode.substr(pos, len));
            return std::string(branchMap.at(branchBits));
        };

        auto substituteImmediate = [&](size_t pos, size_t len) -> std::string {
            std::string bits = std::string(binaryCode.substr(pos, len));
            return std::to_string(std::bitset<16>(bits).to_ulong());
        };

        auto substituteHex = [&](size_t pos, size_t len, int multiplier = 1) -> std::string {
            std::string bits = std::string(binaryCode.substr(pos, len));
            unsigned long val = std::bitset<16>(bits).to_ulong() * multiplier;
            return std::format("{:X}", val);
        };

        std::string result(assembly);
        size_t pos;

        
        while ((pos = result.find("$R")) != std::string::npos) {
            result.replace(pos, 2, substituteRegister(12, 4));
        }
        while ((pos = result.find("$J")) != std::string::npos) {
            result.replace(pos, 2, substituteRegister(8, 4));
        }
        while ((pos = result.find("$Y")) != std::string::npos) {
            result.replace(pos, 2, substituteSpecialRegister(8, 4));
        }
        while ((pos = result.find("$S")) != std::string::npos) {
            result.replace(pos, 2, substituteSpecialRegister(12, 4));
        }
        while ((pos = result.find("$B")) != std::string::npos) {
            result.replace(pos, 2, substituteBranch(4, 4));
        }

        
        while ((pos = result.find("$I")) != std::string::npos) {
            result.replace(pos, 2, substituteImmediate(8, 4));
        }
        while ((pos = result.find("$O")) != std::string::npos) {
            result.replace(pos, 2, substituteImmediate(4, 8));
        }
        while ((pos = result.find("$U")) != std::string::npos) {
            result.replace(pos, 2, substituteImmediate(8, 8));
        }
        while ((pos = result.find("$A")) != std::string::npos) {
            unsigned long val = std::bitset<8>(std::string(binaryCode.substr(8, 8))).to_ulong() * 4;
            result.replace(pos, 2, std::to_string(val));
        }

        while ((pos = result.find("$9")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(4, 8, 2));
        }
        while ((pos = result.find("$8")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(4, 8, 1));
        }
        while ((pos = result.find("$D")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(4, 8, 4));
        }
        while ((pos = result.find("$Z")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(8, 8, 4));
        }
        while ((pos = result.find("$X")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(8, 8, 2));
        }
        while ((pos = result.find("$C")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(8, 8, 1));
        }
        while ((pos = result.find("$L")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(4, 11, 2));
        }
        while ((pos = result.find("$N")) != std::string::npos) {
            result.replace(pos, 2, substituteHex(8, 8, 2));
        }

        
        while ((pos = result.find("$E")) != std::string::npos) {
            result.replace(pos, 2, std::string(binaryCode.substr(8, 8)));
        }
        while ((pos = result.find("$6")) != std::string::npos) {
            unsigned long val = std::bitset<4>(std::string(binaryCode.substr(8, 4))).to_ulong() * 4;
            result.replace(pos, 2, std::to_string(val));
        }

        return result;
    }
    return std::string("word") + std::string(binaryCode);
}