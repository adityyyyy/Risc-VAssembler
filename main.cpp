#include<bits/stdc++.h>
using namespace std;
bool error=false;
int pc = 0;
map<string, int> label;
int memory_counter=0;
vector<string>memory;
unordered_map<string, string> mp= {
    {"zero", "x0"},
    {"ra", "x1"},
    {"sp", "x2"},
    {"gp", "x3"},
    {"tp", "x4"},
    {"t0", "x5"},
    {"t1", "x6"},
    {"t2", "x7"},
    {"s0", "x8"},
    {"s1", "x9"},
    {"a0", "x10"},
    {"a1", "x11"},
    {"a2", "x12"},
    {"a3", "x13"},
    {"a4", "x14"},
    {"a5", "x15"},
    {"a6", "x16"},
    {"a7", "x17"},
    {"s2", "x18"},
    {"s3", "x19"},
    {"s4", "x20"},
    {"s5", "x21"},
    {"s6", "x22"},
    {"s7", "x23"},
    {"s8", "x24"},
    {"s9", "x25"},
    {"s10", "x26"},
    {"s11", "x27"},
    {"t3", "x28"},
    {"t4", "x29"},
    {"t5", "x30"},
    {"t6", "x31"},
};

string convert_to_hex(string a){
    bitset<32> b(a);
    unsigned long int x=b.to_ulong();
    stringstream ss;
    ss << hex << x;
    string hexString = ss.str();
    while(hexString.size() < 8){
        hexString =  "0" + hexString;
    }
    return hexString;
}
string opcode(string a){
    if(a=="R"){
        return "0110011";
    } 
    else if(a=="S"){
        return "0100011";
    } 
    else if(a=="addi" || a=="andi" || a=="ori"){
        return "0010011";
    } 
    else if(a=="lb" || a=="ld" || a=="lw" || a=="lh"){
        return "0000011";
    } 
    else if(a=="jalr"){
        return "1100111";
    } 
    else if(a=="auipc"){
        return "0010111";
    } 
    else if(a=="lui"){
        return "0110111";
    } 
    else if(a=="beq" || a=="bne" || a=="bge" || a=="blt"){
        return "1100011";
    } 
    else if(a=="jal"){
        return "1101111";
    }
    error=true;
    return "";
}

string func3(string a){
    if(a=="add" || a=="sub" || a=="mul" || a=="sb"){
        return "000";
    }
    else if(a=="xor" || a=="div"){
        return "100";
    }
    else if(a=="srl"){
        return "101";
    }
    else if(a=="sra"){
        return "101";
    }
    else if(a=="sll" || a=="sh"){
        return "001";
    }
    else if(a=="slt" || a=="sw"){
        return "010";
    }
    else if(a=="or" || a=="rem"){
        return "110";
    }
    else if(a=="and"){
        return "111";
    }
    else if(a=="sd"){
        return "011";
    }
    else if(a=="lb" || a=="jalr" || a=="addi"){
        return "000";
    }
    else if(a=="lh"){
        return "001";
    }
    else if(a=="lw"){
        return "010";
    }
    else if(a=="ld"){
        return "011";
    }
    else if(a=="ori"){
        return "110";
    }
    else if(a=="andi"){
        return "111";
    }
    else if(a=="beq"){
        return "000";
    }
    else if(a=="bne"){
        return "001";
    }
    else if(a=="blt"){
        return "100";
    }
    else if(a=="bge"){
        return "101";
    }
    error=true;
    return "";
}
string func7(string a){
    if(a=="add" || a=="sll" || a=="slt" || a=="xor" || a=="srl" || a=="or" || a=="and"){
        return "0000000";
    }
    else if(a=="mul" || a=="div" || a=="rem"){
        return "0000001";
    }
    else if(a=="sub" || a=="sra"){
        return "0100000";
    }
    error=true;
    return "";
}
string register_num(string a){
    if(mp.count(a)>0){
        a=mp[a];
    }
    string reg_string;
    for(int i=1;i<a.size();i++){
        reg_string+= a[i];
    }
    int reg_no= stoi(reg_string);
    if(reg_no>=0 && reg_no<32){
        string ans;
        for(int i=0;i<5;i++){
            ans+= ('0'+reg_no%2);
            reg_no/=2;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }
    error=true;
    return "";
}
string immediate(string a){
    int imm= stoi(a);
    if(imm>=-4096 && imm<4096){
        string ans="";
        bitset<12> x(imm);
        ans=x.to_string();
        // reverse(ans.begin(),ans.end());
        return ans;
        
    }
    error=true;
    return "";
}
string immediate_SB(string a){
    int imm= stoi(a);
    if(imm>=-4096*2 && imm<4096*2){
        string ans="";
        bitset<13> x(imm);
        ans=x.to_string();
        // reverse(ans.begin(),ans.end());
        return ans;
        
    }
    error=true;
    return "";
}
string immediate_U(string a){
    int imm= stoi(a);
    if(imm>=0 && imm<1048576){
        string ans;
        for(int i=0;i<20;i++){
            ans=(char)('0' + imm%2)+ans;
            imm/=2;
        }
        reverse(ans.begin(),ans.end());
        return ans;
    }
    error=true;
    return "";
}
string immediate_UJ(string a){
    int imm= stoi(a);
    if(imm>=-1048576 && imm<1048576){
        string ans;
        bitset<21> x(imm);
        ans=x.to_string();
        // reverse(ans.begin(),ans.end());
        return ans;
    }
    error=true;
    return "";
}

string U_format(string operation,string rd,string imm){
    string ans="";
    imm=immediate_U(imm);
    reverse(imm.begin(),imm.end());
    ans+=imm;
    ans+=register_num(rd);
    ans+=opcode(operation);
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}

string R_format(string operation,string rd,string rs1,string rs2){
    // Can't find mul, div
    // mul,div,add,rem-> same opcode,func7 mul,div-> different func3
    // add-> 0x003100B3
    // mul-> 0x023100B3 -> func3=000
    // div-> 0x023140B3 -> func3=100
    // rem-> 0x023160B3 -> func3=110
    string ans="";
    ans+=func7(operation);
    ans+=register_num(rs2);
    ans+=register_num(rs1);
    ans+=func3(operation);
    ans+=register_num(rd);
    ans+=opcode("R");
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}
string S_format(string operation,string rs2,string rs1,string imm){
    // I have considered sd as S format -> DOUBTFULL
    string ans="";
    imm=immediate(imm);
    // reverse(imm.begin(),imm.end());
    // 11,10,.. 0
    string imm1=imm.substr(0,7),imm2=imm.substr(7);
    ans+=imm1;
    ans+=register_num(rs2);
    ans+=register_num(rs1);
    ans+=func3(operation);
    ans+=imm2;
    ans+=opcode("S");
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}

string I_format(string operation,string rd,string rs1, string imm){
    string ans="";    
    ans=immediate(imm);
    ans+=register_num(rs1);
    ans+=func3(operation);
    ans+=register_num(rd);
    ans+=opcode(operation);
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}

string SB_format(string operation,string rs2, string rs1, string imm){
    string ans="";
    imm=immediate_SB(imm);
    ans+=imm[0];            //12
    ans+=imm.substr(2,6);   //10:5
    ans+=register_num(rs2);
    ans+=register_num(rs1);
    ans+=func3(operation);
    ans+=imm.substr(8,4);   //4:1
    ans+=imm[1];            //11
    ans+=opcode(operation);
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}

string UJ_format(string operation,string rd,string imm){
    string ans="";
    imm=immediate_UJ(imm);
    ans+=imm[0];            //20
    ans+=imm.substr(10,10); //10:5
    ans+=imm[9];            //11
    ans+=imm.substr(1,8);   //19:12
    ans+=register_num(rd);
    ans+=opcode(operation);
    if(error){
        return "";
    }
    ans=convert_to_hex(ans);
    ans="0x"+ans;
    return ans;
}

bool check_hex(string s){
    int flag=0;
    for(int i=0;i<s.size();i++){
        if(!((s[i]>='0' && s[i]<='9') || (s[i]>='A' && s[i]<='F') || (s[i]>='a' && s[i]<='f'))){
            return false;
        }
        else if(flag==0 && s[i]!= '0'){
            if(s.size()-i>8){
                return false;
            }
            else{
                flag=1;
            }
        }
    }
    return true;
}

bool check_decimal(string s){
    if(s=="-")return false;
    if(!((s[0]<='9' && s[0]>='0') || s[0]=='-')){
        return false;
    }
    for(int i=1;i<s.size();i++){
        if(!(s[i]<='9' && s[i]>='0')){
            return false;
        }
    }
    long long num=0;
    int l=0;
    if(s[0]=='-')l++;
    int count=0;
    for(int i=s.size()-1;i>=l;i--){
        num+= (s[i]-'0')*powl(10,count);
        if((l==0 && num>=powl(2,31)) || (l==1 && num>powl(2,31))){
            return false;
        }
        count++;
    }
    return true;
}

bool check_hex_byte(string s){
    int flag=0;
    for(int i=0;i<s.size();i++){
        if(!((s[i]>='0' && s[i]<='9') || (s[i]>='A' && s[i]<='F') || (s[i]>='a' && s[i]<='f'))){
            return false;
        }
        else if(flag==0 && s[i]!= '0'){
            if(s.size()-i>2){
                return false;
            }
            else{
                flag=1;
            }
        }
    }
    return true;
}

bool check_byte(string s){
    if(s.size()!=3 || s[0]!='\'' || s[2]!='\''){
        return false;
    }
    return true;
}

bool check_decimal_byte(string s){
    if(s=="-")return false;
    if(!((s[0]<='9' && s[0]>='0') || s[0]=='-')){
        return false;
    }
    for(int i=1;i<s.size();i++){
        if(!(s[i]<='9' && s[i]>='0')){
            return false;
        }
    }
    long long num=0;
    int l=0;
    if(s[0]=='-')l++;
    int count=0;
    for(int i=s.size()-1;i>=l;i--){
        num+= (s[i]-'0')*powl(10,count);
        if((l==0 && num>=powl(2,7)) || (l==1 && num>powl(2,7))){
            return false;
        }
        count++;
    }
    return true;
}

bool check_asciiz(string s){
    if(s[0]!= '\"' || s[s.size()-1]!= '\"'){
        return false;
    }
    return true;
}

bool check_hex_half(string s){
    int flag=0;
    for(int i=0;i<s.size();i++){
        if(!((s[i]>='0' && s[i]<='9') || (s[i]>='A' && s[i]<='F') || (s[i]>='a' && s[i]<='f'))){
            return false;
        }
        else if(flag==0 && s[i]!= '0'){
            if(s.size()-i>4){
                return false;
            }
            else{
                flag=1;
            }
        }
    }
    return true;
}

bool check_decimal_half(string s){
    if(s=="-")return false;
    if(!((s[0]<='9' && s[0]>='0') || s[0]=='-')){
        return false;
    }
    for(int i=1;i<s.size();i++){
        if(!(s[i]<='9' && s[i]>='0')){
            return false;
        }
    }
    long long num=0;
    int l=0;
    if(s[0]=='-')l++;
    int count=0;
    for(int i=s.size()-1;i>=l;i--){
        num+= (s[i]-'0')*powl(10,count);
        if((l==0 && num>=powl(2,15)) || (l==1 && num>powl(2,15))){
            return false;
        }
        count++;
    }
    return true;
}

bool check_hex_double(string s){
    int flag=0;
    for(int i=0;i<s.size();i++){
        if(!((s[i]>='0' && s[i]<='9') || (s[i]>='A' && s[i]<='F') || (s[i]>='a' && s[i]<='f'))){
            return false;
        }
        else if(flag==0 && s[i]!= '0'){
            if(s.size()-i>16){
                return false;
            }
            else{
                flag=1;
            }
        }
    }
    return true;
}

bool check_decimal_double(string s){
    if(s=="-")return false;
    if(!((s[0]<='9' && s[0]>='0') || s[0]=='-')){
        return false;
    }
    for(int i=1;i<s.size();i++){
        if(!(s[i]<='9' && s[i]>='0')){
            return false;
        }
    }
    long long num=0;
    int l=0;
    if(s[0]=='-')l++;
    int count=0;
    for(int i=s.size()-1;i>=l;i--){
        num+= (s[i]-'0')*powl(10,count);
        if((l==0 && num>=powl(2,63)) || (l==1 && num>powl(2,63))){
            return false;
        }
        count++;
    }
    return true;
}

int main(){
    string ifilename = "test.asm";
    string ofilename = "test.mc";

    ifstream ifile(ifilename);
    ofstream ofile(ofilename);

    map<string, int> label;
    map<string, int> label2;
    int memory_counter=0;
    vector<string>memory;
    vector<vector<string>>instructions;
    string line;
    int flag=0;
    ofile<<"Code Segment\n";
    while (getline(ifile, line)) {
        if(line.size() == 0) continue;
        vector<string> lineVec;
        string seg = "";
        for(int i=0;i<line.size();i++){
            if(line[i]==' ' || line[i]==',' || line[i]=='\t' || line[i] == '\n'){
                if(seg!=""){
                    lineVec.push_back(seg);
                    seg="";
                }
                continue;
            }
            else if(line[i]==':'){
                if(seg!=""){
                    seg+=':';
                    lineVec.push_back(seg);
                    seg="";
                }
                continue;
            }
            seg+=line[i];
            if(i == line.size()-1) lineVec.push_back(seg);
        }
        if(line==".data"){
            while (getline(ifile, line)) {
                if(line==".text"){
                    flag=1;
                    break;
                }
                vector<string> lineVec;
                string seg = "";
                for(int i=0;i<line.size();i++){
                    if(line[i]==' ' || line[i]==',' || line[i]=='\t' || line[i] == '\n' || line[i]==':'){
                        if(seg!=""){
                            lineVec.push_back(seg);
                            seg="";
                        }
                        continue;
                    }
                    seg+=line[i];
                    if(i == line.size()-1) lineVec.push_back(seg);
                }
                label[lineVec[0]]=memory_counter;
                if(lineVec[1]==".word"){
                    int j=2;
                    while(j<lineVec.size()){
                        long long decimal;
                        string temp= lineVec[j];
                        if(lineVec[j][0]=='0' && lineVec[j][1]=='x'){
                            if(!check_hex(lineVec[j].substr(2))){
                                ofile<< "Error\n";
                                exit(0);
                            }
                            int append_zero= 10 -lineVec[j].size();
                            string temp;
                            if(lineVec[j].size()>10){
                                for(int k= lineVec[j].size()-8;k<lineVec[j].size();k++){
                                    temp+= lineVec[j][k];
                                }
                            }
                            else{
                                for(int i=0;i<append_zero;i++){
                                    temp+="0";
                                }
                                for(int i=2;i<lineVec[j].size();i++){
                                    temp+= lineVec[j][i];
                                }
                            }
                            memory.push_back(temp.substr(6,2));
                            memory_counter++;
                            memory.push_back(temp.substr(4,2));
                            memory_counter++;
                            memory.push_back(temp.substr(2,2));
                            memory_counter++;
                            memory.push_back(temp.substr(0,2));
                            memory_counter++;
                            j++;
                            continue;
                        }
                        else{
                            if(!(check_decimal(lineVec[j]))){
                                exit(0);
                            }
                            decimal = stoi(lineVec[j]);
                            if(decimal < 0){
                                decimal= powl(2,32) + decimal;
                            }
                        }
                        for(int i=0;i<4;i++){
                            stringstream ss;
                            string hexString="";
                            if((decimal %256) <16){
                                hexString+= "0";
                            }
                            ss << hex << decimal%256;
                            hexString += ss.str();
                            memory.push_back(hexString);
                            hexString="";
                            memory_counter++;
                            decimal/=256;
                        }
                        j++;
                    }
                }
                else if(lineVec[1]==".byte"){
                    int j=2;
                    while(j<lineVec.size()){
                        int decimal;
                        string temp = lineVec[j];
                        if(lineVec[j][0]=='0' && lineVec[j][1]=='x'){
                            if(!check_hex_byte(lineVec[j].substr(2))){
                                ofile<< "Error\n";
                                exit(0);
                            }
                            int append_zero= 4 -lineVec[j].size();
                            string temp;
                            if(lineVec[j].size()>4){
                                for(int k= lineVec[j].size()-2;k<lineVec[j].size();k++){
                                    temp+= lineVec[j][k];
                                }
                            }
                            else{
                                for(int i=0;i<append_zero;i++){
                                    temp+="0";
                                }
                                for(int i=2;i<lineVec[j].size();i++){
                                    temp+= lineVec[j][i];
                                }
                            }
                            memory.push_back(temp.substr(0,2));
                            memory_counter++;
                            j++;
                            continue;
                        }
                        else if(lineVec[j][0]=='\''){
                            if(!check_byte(lineVec[j])) exit(0);
                            decimal = (int)lineVec[j][1];
                        }
                        else{
                            if(!check_decimal_byte(lineVec[j])) exit(0);
                            decimal= stoi(lineVec[j]);
                            if(decimal < 0){
                                decimal= powl(2,8) + decimal;
                            }
                        }
                        stringstream ss;
                        string hexString="";
                        if((decimal) <16){
                            hexString+= "0";
                        }
                        ss << hex << decimal;
                        hexString += ss.str();
                        memory.push_back(hexString);
                        memory_counter++;
                        j++;
                    }
                }
                else if(lineVec[1]==".asciz"){
                    int j=2;
                    if(lineVec.size()>3)exit(0);
                    if(!check_asciiz(lineVec[2]))exit(0);
                    while(j<lineVec.size()){
                        int decimal;
                        if(lineVec[j][0]=='\"'){
                            for(int k=1;lineVec[j][k]!='\"';k++){
                                decimal = (int)lineVec[j][k];
                                stringstream ss;
                                string hexString="";
                                if((decimal) <16){
                                    hexString+= "0";
                                }
                                ss << hex << decimal;
                                hexString += ss.str();
                                memory.push_back(hexString);
                                memory_counter++;
                            }
                        }
                        j++;
                    }
                }
                else if(lineVec[1]==".half"){
                    int j=2;
                    while(j<lineVec.size()){
                        long long decimal;
                        string temp= lineVec[j];
                        if(lineVec[j][0]=='0' && lineVec[j][1]=='x'){
                            if(!check_hex_half(lineVec[j].substr(2))){
                                ofile<< "Error\n";
                                exit(0);
                            }
                            int append_zero= 6 -lineVec[j].size();
                            string temp;
                            if(lineVec[j].size()>6){
                                for(int k= lineVec[j].size()-4;k<lineVec[j].size();k++){
                                    temp+= lineVec[j][k];
                                }
                            }
                            else{
                                for(int i=0;i<append_zero;i++){
                                    temp+="0";
                                }
                                for(int i=2;i<lineVec[j].size();i++){
                                    temp+= lineVec[j][i];
                                }
                            }
                            memory.push_back(temp.substr(2,2));
                            memory_counter++;
                            memory.push_back(temp.substr(0,2));
                            memory_counter++;
                            j++;
                            continue;
                        }
                        else{
                            if(!(check_decimal_half(lineVec[j]))){
                                exit(0);
                            }
                            decimal = stoi(lineVec[j]);
                            if(decimal < 0){
                                decimal= powl(2,16) + decimal;
                            }
                        }
                        for(int i=0;i<2;i++){
                            stringstream ss;
                            string hexString="";
                            if((decimal %256) <16){
                                hexString+= "0";
                            }
                            ss << hex << decimal%256;
                            hexString += ss.str();
                            memory.push_back(hexString);
                            hexString="";
                            memory_counter++;
                            decimal/=256;
                        }
                        j++;
                    }
                }
                else if(lineVec[1]==".dword"){
                    int j=2;
                    while(j<lineVec.size()){
                        long long decimal;
                        string temp= lineVec[j];
                        if(lineVec[j][0]=='0' && lineVec[j][1]=='x'){
                            if(!check_hex_double(lineVec[j].substr(2))){
                                ofile<< "Error\n";
                                exit(0);
                            }
                            int append_zero= 18 -lineVec[j].size();
                            string temp;
                            if(lineVec[j].size()>18){
                                for(int k= lineVec[j].size()-16;k<lineVec[j].size();k++){
                                    temp+= lineVec[j][k];
                                }
                            }
                            else{
                                for(int i=0;i<append_zero;i++){
                                    temp+="0";
                                }
                                for(int i=2;i<lineVec[j].size();i++){
                                    temp+= lineVec[j][i];
                                }
                            }
                            memory.push_back(temp.substr(14,2));
                            memory_counter++;
                            memory.push_back(temp.substr(12,2));
                            memory_counter++;
                            memory.push_back(temp.substr(10,2));
                            memory_counter++;
                            memory.push_back(temp.substr(8,2));
                            memory_counter++;
                            memory.push_back(temp.substr(6,2));
                            memory_counter++;
                            memory.push_back(temp.substr(4,2));
                            memory_counter++;
                            memory.push_back(temp.substr(2,2));
                            memory_counter++;
                            memory.push_back(temp.substr(0,2));
                            memory_counter++;
                            j++;
                            continue;
                        }
                        else{
                            if(!(check_decimal_double(lineVec[j]))){
                                exit(0);
                            }
                            decimal = stoi(lineVec[j]);
                            if(decimal < 0){
                                decimal= powl(2,64) + decimal;
                            }
                        }
                        for(int i=0;i<8;i++){
                            stringstream ss;
                            string hexString="";
                            if((decimal %256) <16){
                                hexString+= "0";
                            }
                            ss << hex << decimal%256;
                            hexString += ss.str();
                            memory.push_back(hexString);
                            hexString="";
                            memory_counter++;
                            decimal/=256;
                        }
                        j++;
                    }
                }
            }
        }
        if(flag==1){
            flag=0;
            continue;
        }
        int start=0;
        for(int k=0;k<lineVec.size();k++){
            if(lineVec[k][lineVec[k].size()-1]==':'){
                for(int i=0;i<k;i++){
                    if(label2.find(lineVec[i])==label2.end() && label.find(lineVec[i])==label.end()){
                        label2[lineVec[i]]=pc;
                    }
                    else{
                        ofile<<"Error\n";
                        exit(0);
                    }
                }
                if(lineVec[k].size()>1 && label2.find(lineVec[k].substr(0,lineVec[k].size()-1))==label2.end() && label.find(lineVec[k].substr(0,lineVec[k].size()-1))==label.end()){
                    label2[lineVec[k].substr(0,lineVec[k].size()-1)]=pc;
                }
                else{
                    ofile<<"Error\n";
                    exit(0);
                }
                start=k+1;
                break;
            }
        }
        if(lineVec[0][start]=='#'){
            continue;
        }
        if(start>= lineVec.size())continue;
        vector<string>v;
        for(int i=start;i<lineVec.size();i++){
            v.push_back(lineVec[i]);
        }
        instructions.push_back(v);
        if(flag==0){
            pc += 4;
        }
        else flag=0;
    }
    for(int i=0;i<instructions.size();i++){
        string operation = instructions[i][0]; 
        string rd = "";
        string rs1 = "";
        string rs2 = "";
        string imm = "";

        string ans = "";

        if(operation=="add" || operation=="sub" || operation=="xor" || operation=="mul" || operation=="div" || operation=="rem" || operation=="srl" || operation=="sll" || operation=="slt" || operation=="or" || operation=="and" || operation=="sra"){
            rd = instructions[i][1];
            rs1 = instructions[i][2];
            rs2 = instructions[i][3];
            ans = R_format(operation, rd, rs1, rs2);
        } else if(operation=="addi" || operation=="andi" || operation=="ori" ||  operation=="jalr"){
            rd = instructions[i][1];
            rs1 = instructions[i][2];
            if(label2.find(instructions[i][3])!=label2.end()) instructions[i][3]= to_string(label2[instructions[i][3]]);
            imm = instructions[i][3];
            ans = I_format(operation, rd, rs1, imm);
        } 
        else if(operation=="lb" ||  operation=="ld" ||  operation=="lh" ||  operation=="lw"){
            rd = instructions[i][1];
            // rs1 = lineVec[2];
            int j=0,n1=instructions[i][2].size();
            while(j<n1 && instructions[i][2][j]!='('){
                imm+=instructions[i][2][j];
                j++;
            }
            j++;
            while(j<n1 && instructions[i][2][j]!=')'){
                rs1+=instructions[i][2][j];
                j++;
            }
            ans = I_format(operation, rd, rs1, imm);
        }
        else if(operation=="sb" || operation=="sw" || operation=="sd" || operation=="sh"){
            rs2 = instructions[i][1];
            // rs1 = lineVec[2];
            int j=0,n1=instructions[i][2].size();
            while(j<n1 && instructions[i][2][j]!='('){
                imm+=instructions[i][2][j];
                j++;
            }
            j++;
            while(j<n1 && instructions[i][2][j]!=')'){
                rs1+=instructions[i][2][j];
                j++;
            }
            ans = S_format(operation, rs2, rs1, imm);
        } else if(operation=="beq" || operation=="bne" || operation=="bge" || operation=="blt"){
            rs1 = instructions[i][1];
            rs2 = instructions[i][2];
            if(label2.find(instructions[i][3])!=label2.end()) instructions[i][3]= to_string(label2[instructions[i][3]]-i*4);
            imm = instructions[i][3];
            ans = SB_format(operation, rs2, rs1, imm);
        } else if(operation=="jal"){
            rd = instructions[i][1];
            if(label2.find(instructions[i][2])!=label2.end()) instructions[i][2]= to_string(label2[instructions[i][2]]-i*4);
            imm = instructions[i][2];
            ans = UJ_format(operation, rd, imm);
        } else if(operation=="auipc" || operation=="lui"){
            rd = instructions[i][1];
            imm = instructions[i][2];
            ans = U_format(operation, rd, imm);
        }

        if(error){
            ofile<<"Error Occured!"<<endl;
            break;
        }
            stringstream ss;
            ss << hex << i*4;
            ofile << "0x"<<ss.str() << " ";
            ofile << ans << "\n";
    }
    ofile<< "Data Segment\n";
    for(int i=0;i<memory.size();i++){
        stringstream ss;
        ss << hex << 268435456+i;
        ofile << "0x" << ss.str() << " " << "0x" <<memory[i]<<"\n";
    }
    ofile<<"Labels\n";
    for(auto it= label2.begin();it!=label2.end();it++){
        ofile << it->first <<" " << it->second <<"\n";
    }
    ifile.close();
    ofile.close();
    return 0;
}
