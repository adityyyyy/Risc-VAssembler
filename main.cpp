#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

using namespace std;

unordered_map<string, string> opcode_map = {
    {"add", "0000000"},
    {"and", "0000001"},
    {"or", "0000010"},
    {"sll", "0000011"},
    {"slt", "0000100"},
    {"sra", "0000101"},
    {"srl", "0000110"},
    {"sub", "0000111"},
    {"xor", "0001000"},
    {"mul", "0001001"},
    {"div", "0001010"},
    {"rem", "0001011"},
    {"addi", "0001100"},
    {"andi", "0001101"},
    {"ori", "0001110"},
    {"lb", "0001111"},
    {"ld", "0010000"},
    {"lh", "0010001"},
    {"lw", "0010010"},
    {"jalr", "0010011"},
    {"sb", "0010100"},
    {"sw", "0010101"},
    {"sd", "0010110"},
    {"sh", "0010111"},
    {"beq", "0011000"},
    {"bne", "0011001"},
    {"bge", "0011010"},
    {"blt", "0011011"},
    {"auipc", "0011100"},
    {"lui", "0011101"},
    {"jal", "0011110"}
};

string assemble_instruction(string instruction) {
    stringstream ss(instruction);
    string opcode, rd, rs1, rs2;

    ss >> opcode;
    if (opcode == ".text" || opcode == ".data") {
        return ""; 
    }

    ss >> rd >> rs1 >> rs2; 

    string machine_code = opcode_map[opcode];
    machine_code += "000"; 
    machine_code += bitset<5>(stoi(rs1.substr(1))).to_string(); 
    machine_code += bitset<5>(stoi(rd.substr(1))).to_string(); 

    return machine_code;
}

void assemble(const string& input_file, const string& output_file) {
    ifstream infile(input_file);
    ofstream outfile(output_file);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open input file" << endl;
        return;
    }

    if (!outfile.is_open()) {
        cerr << "Error: Unable to create output file" << endl;
        return;
    }

    string line;
    int address = 0;

    while (getline(infile, line)) {
        string machine_code = assemble_instruction(line);
        if (!machine_code.empty()) {
            outfile << "0x" << hex << setw(8) << setfill('0') << address << " " << "0x" << machine_code << endl;
            address += 4; 
        }
    }

   
    outfile << "0x" << hex << setw(8) << setfill('0') << address << " " << "0xFFFFFFFF" << endl;

    infile.close();
    outfile.close();
}

int main() {
    string input_file = "input.asm";
    string output_file = "output.mc";

    assemble(input_file, output_file);

    cout << "Assembly code assembled successfully into machine code." << endl;

    return 0;
}
