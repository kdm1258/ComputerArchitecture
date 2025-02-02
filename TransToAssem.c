#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//label저장 구조체
typedef struct{
	char label[32];
	int address;
}  LabelTable;
LabelTable labels[101010];
//Instruct 저장 구조체
typedef struct{
	char oper[50];
	char operand1[50];
	char operand2[50];
	char operand3[50];
	//int address;
} InstTable;

char output_filename[256];
InstTable insts[101010];
int insts_cnt = 250;	//pc = 0일때의 명령어를 0번째 인덱스에 저장한다고 가정 , pc = 1000번째명령어는 250에 저장
int label_cnt = 0;
int pc = 1000;
int reg[32]; 
int Mem[101010];
FILE *output;
void R_typeInst(int,int,int,int,int,int);
void Add(int, int, int);
void Sub(int, int, int);
void And(int, int, int);
void Or(int, int, int);
void Xor(int, int, int);
void Sll(int, int, int);
void Srl(int, int, int);
void Sra(int, int, int);
void I_typeInst(int,int,int,int,int);
void Addi(int, int, int);
void Andi(int, int, int);
void Ori(int, int, int);
void Xori(int, int, int);
void Slli(int, int, int);
void Srli(int, int, int);
void Srai(int, int, int);
void Lw(int, int, int);
void Jalr(int, int, int);
void S_typeInst(int, int, int, int, int);
void Sw(int, int, int);
void SB_typeInst(int, int, int, int, int);
void Beq(int, int, int);
void Bne(int, int, int);
void Bge(int, int, int);
void Blt(int, int, int);
void UJ_typeInst(int, int, int);
void Jal(int, int);
//label Table 추가 함수
void add_label(const char *label, int address){
	strcpy(labels[label_cnt].label, label);
	labels[label_cnt].address = address;
	label_cnt++;
}
//label Table 검색 함수
int find_label(const char *label){
	char label1[50] = {0};
	for(int i = 0 ; label[i] ; i++){
		label1[i] = tolower(label[i]);
	}
	for(int i = 0 ; i < label_cnt ; i++){
		if(strcmp(labels[i].label, label1) == 0) return labels[i].address;
	}
	return -1;
}
int isNumeric(const char* str) {
    if (*str == '\0') {
        return 0; 
    }

    // 첫 번째 문자가 '-'인 경우 처리
    if (*str == '-') {
        str++; // 부호를 스킵
    }

    if (*str == '\0') {
        return 0;
    }

    // 나머지 문자가 모두 숫자인지 확인
    while (*str) {
        if (!isdigit(*str)) {
            return 0; // 숫자가 아닌 문자를 발견하면 false 반환
        }
        str++;
    }

    return 1; 
}
//instTable 추가함수
void add_inst(char oper[50], char operand1[50], char operand2[50], char operand3[50]){
	strcpy(insts[insts_cnt].oper, oper);
	strcpy(insts[insts_cnt].operand1, operand1);
	strcpy(insts[insts_cnt].operand2, operand2);
	strcpy(insts[insts_cnt].operand3, operand3);
	
	if (!(strcmp(oper,"add"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);	
		Add(rd,rs1,rs2);
	}
	else if(!(strcmp(oper,"sub"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Sub(rd, rs1, rs2);
	}
	else if(!(strcmp(oper,"and"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		And(rd, rs1, rs2);
	}else if(!(strcmp(oper,"or"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Or(rd, rs1, rs2);
	}
	else if(!(strcmp(oper,"xor"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Xor(rd, rs1, rs2);
	}
	else if(!(strcmp(oper,"sll"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Sll(rd, rs1, rs2);
	}
	else if(!(strcmp(oper,"srl"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Srl(rd, rs1, rs2);
	}
	else if(!(strcmp(oper,"sra"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int rs2 = atoi(&operand3[1]);
		Sra(rd, rs1, rs2);
	}
	//I-type
	else if(!(strcmp(oper,"addi"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int imm12 = atoi(&operand3[0]);
		Addi(rd,rs1,imm12);
	}
	else if(!(strcmp(oper,"andi"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int imm12 = atoi(&operand3[0]);
		Andi(rd,rs1,imm12);
	}
	else if(!(strcmp(oper,"ori"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int imm12 = atoi(&operand3[0]);
		Ori(rd,rs1,imm12);
	}
	else if(!(strcmp(oper,"xori"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int imm12 = atoi(&operand3[0]);
		Xori(rd,rs1,imm12);
	}
	else if(!(strcmp(oper,"slli"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int shamt = atoi(&operand3[0]);
		Slli(rd,rs1,shamt);
	}
	else if(!(strcmp(oper,"srli"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int shamt = atoi(&operand3[0]);
		Srli(rd,rs1,shamt);
	}
	else if(!(strcmp(oper,"srai"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand2[1]);
		int shamt = atoi(&operand3[0]);
		Srai(rd,rs1,shamt);
	}
	else if(!(strcmp(oper,"lw"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand3[1]);
		int imm12 = atoi(&operand2[0]);
		Lw(rd,rs1,imm12);
	}
	else if(!(strcmp(oper,"jalr"))){
		int rd = atoi(&operand1[1]);
		int rs1 = atoi(&operand3[1]);
		int imm12 = atoi(&operand2[0]);
		Jalr(rd,rs1,imm12);
	}
	//S-type
	else if(!(strcmp(oper,"sw"))){
		int rs2 = atoi(&operand1[1]);
		int rs1 = atoi(&operand3[1]);
		int imm12 = atoi(&operand2[0]);
		Sw(rs1, rs2, imm12);
	}
	//SB-type
	else if(!(strcmp(oper,"beq"))){
		int rs1 = atoi(&operand1[1]);
		int rs2 = atoi(&operand2[1]);
		int imm12 = find_label(operand3) - pc;
		Beq(rs1, rs2, imm12);
	}
	else if(!(strcmp(oper,"bne"))){
		int rs1 = atoi(&operand1[1]);
		int rs2 = atoi(&operand2[1]);
		int imm12 = find_label(operand3) - pc;
		Bne(rs1, rs2, imm12);
	}
	else if(!(strcmp(oper,"bge"))){
		int rs1 = atoi(&operand1[1]);
		int rs2 = atoi(&operand2[1]);
		int imm12 = find_label(operand3) - pc;
		Bge(rs1, rs2, imm12);
	}
	else if(!(strcmp(oper,"blt"))){
		int rs1 = atoi(&operand1[1]);
		int rs2 = atoi(&operand2[1]);
		int imm12 = find_label(operand3) - pc;
		Blt(rs1, rs2, imm12);
	}
	//UJ-type
	else if(!(strcmp(oper,"jal"))){
		int rd = atoi(&operand1[1]);
		int imm20 = find_label(operand2) - pc;
		Jal(rd, imm20);
	}
	//Exit
	else if(!(strcmp(oper,"exit"))){
		fprintf(output, "11111111111111111111111111111111\n");
	}
	insts_cnt++;
	return;
}
void Make_Binary(int val, FILE *output) { // 파일 포인터를 인자로 받음
    for (int i = 31; i >= 0; i--) {
        fprintf(output, "%d", (val >> i) & 1); // 출력 파일로 이진수 기록
    }
    fprintf(output, "\n");
}
//file 파싱, Label list 추가
int parse_line(const char *line) {
    int parcnt = 0;  // 괄호 개수
    char tmp[256] = {0};
    char oper[50] = {0};
    char operand1[50] = {0};
    char operand2[50] = {0};
    char operand3[50] = {0};
    int operand_count = 0;
    int has_extra_comma = 0;

    strcpy(tmp, line);

    // 공백 제거
    char *token = strtok(tmp, " \t\n");
    if (!token) return 1;

    // 레이블일 경우 무시
    if (token[strlen(token) - 1] == ':') {
        return 1;
    }

    // 연산자 추출
    for (int i = 0; token[i]; i++) {
        oper[i] = tolower(token[i]);
    }
    
    // "exit" 명령어 처리
    if (!strcmp(oper, "exit")) {
        add_inst(oper, operand1, operand2, operand3);
        pc += 4;
        return 1;
    }

    // 나머지 오퍼랜드 처리
    char *line_ptr = token + strlen(token) + 1;
    int i = 0, length = strlen(line_ptr), operand_start = 1;
    while (i < length) {
        char ch = line_ptr[i];
        if (ch == ',') {
            if (!operand_start) {  // 쉼표 뒤에 유효한 오퍼랜드가 없으면 에러
                has_extra_comma = 1;
                break;
            }
            operand_start = 0;
            operand_count++;
        } else if (ch == '(') {  // 여는 괄호 처리
            parcnt++;
            operand_start = 1;  // 괄호 안에서도 유효한 오퍼랜드가 올 수 있음
            operand_count++;    // 괄호 안의 값은 operand3로 처리
        } else if (ch == ')') {  // 닫는 괄호 처리
            parcnt--;
        } else if (!isspace(ch)) {
            operand_start = 1;
            if (operand_count == 0) strncat(operand1, &ch, 1);
            else if (operand_count == 1) strncat(operand2, &ch, 1);
            else if (operand_count == 2) strncat(operand3, &ch, 1);
        }
        i++;
    }

    // 쉼표 개수 및 괄호 짝 확인
    if (parcnt != 0) {
        return 0;
    }
    if (has_extra_comma || operand_count > 2) {
        return 0;
    }

    // 오퍼랜드 개수 확인
    if (operand1[0] == '\0' || operand2[0] == '\0') {
        return 0;
    }

    // 명령어 추가
    add_inst(oper, operand1, operand2, operand3);
    pc += 4;
    return 1;
}

   
void parse_label(const char *line){
	char tmp[256] = {0};
	char label[50] = {0};	//label
	strcpy(tmp, line);

	//공백제거
	char *token = strtok(tmp," \t\n");
	if (!token) return;
	
	//token의 마지막 문자가 :일 경우 label로 인식
	if(token[strlen(token)-1]==':'){
		strncpy(label, token, strlen(token)-1);
		for(int i = 0 ; label[i] ; i++){ 
			label[i] = tolower(label[i]);
		}
		add_label(label, pc);
		return;
	}	
	pc+=4;	//label저장시 address얻기 위함 
	return;
}
//연산부
int inst_calc(const InstTable instTable){
	char oper[50]= {0}; strcpy(oper,instTable.oper);
	char operand1[50] = {0}; strcpy(operand1, instTable.operand1);
	char operand2[50] = {0}; strcpy(operand2, instTable.operand2);
	char operand3[50] = {0}; strcpy(operand3, instTable.operand3);
	//연산자별 기계어 변환
	//R-type
	if (!(strcmp(oper,"add"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) && isNumeric(&operand3[1])  
		){	
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);	
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			reg[rd] = reg[rs1] + reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"sub"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			reg[rd] = reg[rs1] - reg[rs2];
			pc+=4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"and"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			reg[rd] = reg[rs1] & reg[rs2];
			pc+=4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"or"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			reg[rd] = reg[rs1] | reg[rs2];
			pc+=4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"xor"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			reg[rd] = reg[rs1] ^ reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"sll"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			if (reg[rs2]>31){ return 1;}
			reg[rd] = reg[rs1] << reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"srl"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			if (reg[rs2]>31){ return 1;}
			reg[rd] = (unsigned)reg[rs1] >> reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"sra"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int rs2 = atoi(&operand3[1]);
			if(rd>31 || rs1 >31 || rs2 >31) return 1;
			if (reg[rs2]>31){ return 1;}
			reg[rd] = reg[rs1] >> reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	//I-type
	else if(!(strcmp(oper,"addi"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int imm12 = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			reg[rd] = reg[rs1] + imm12;
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"andi"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int imm12 = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			reg[rd] = reg[rs1] & imm12;
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"ori"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int imm12 = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			reg[rd] = reg[rs1] | imm12;
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"xori"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int imm12 = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			reg[rd] = reg[rs1] ^ imm12;
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"slli"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int shamt = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || shamt>31 || shamt<0) return 1;
			reg[rd] = reg[rs1] << shamt;
			pc += 4;	
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"srli"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int shamt = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || shamt>31 || shamt<0) return 1;
			reg[rd] = (unsigned)reg[rs1] >> shamt;
			if (shamt>31){ return 1;}
			pc+=4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"srai"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]) &&isNumeric(&operand3[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand2[1]);
			int shamt = atoi(&operand3[0]);
			if(rd>31 || rs1 >31 || shamt>31 ||shamt<0) return 1;
			reg[rd] = reg[rs1] >> shamt;
			if (shamt>31){ return 1;}
			pc += 4;
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"lw"))){
		if(operand1[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[0]) &&isNumeric(&operand3[1])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand3[1]);
			int imm12 = atoi(&operand2[0]);
			if(rd>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			reg[rd] = Mem[reg[rs1] + imm12];
			pc += 4;
			return 0;
		}
		return 1;	
	}
	else if(!(strcmp(oper,"jalr"))){
		if(operand1[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand3[1]) &&isNumeric(&operand2[0])  
		){
			int rd = atoi(&operand1[1]);
			int rs1 = atoi(&operand3[1]);
			int imm12 = atoi(&operand2[0]);
			if(rd>31 || rs1 >31 || imm12>2047) return 1;
			if(rd!=0){reg[rd] = pc + 4 ;}
			pc = (reg[rs1] + imm12) & (~1);	
			return 0;
		}
		return 1;
	}
	//S-type
	else if(!(strcmp(oper,"sw"))){
		if(operand1[0] == 'x' && operand3[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand3[1]) &&isNumeric(&operand2[0])  
		){
			int rs2 = atoi(&operand1[1]);
			int rs1 = atoi(&operand3[1]);
			int imm12 = atoi(&operand2[0]);
			if(rs2>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			Mem[reg[rs1] + imm12] = reg[rs2];
			pc += 4;
			return 0;
		}
		return 1;
	}
	//SB-type
	else if(!(strcmp(oper,"beq"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]))  
		{
			int rs1 = atoi(&operand1[1]);
			int rs2 = atoi(&operand2[1]);
			int label_address = find_label(operand3);
			int imm12 = label_address - pc;
			if(rs2>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			if (label_address == -1) return 1;	// 라벨이 없으면 에러 처리
			if(reg[rs1] == reg[rs2]){pc += imm12;}
			else {pc += 4;}
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"bne"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]))  
		{
			int rs1 = atoi(&operand1[1]);
			int rs2 = atoi(&operand2[1]);
			int label_address = find_label(operand3);
			int imm12 = label_address - pc;
			if(rs2>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			if (label_address == -1) return 1;	// 라벨이 없으면 에러 처리
			if(reg[rs1] != reg[rs2]){pc += imm12;}
			else {pc += 4;}
			return 0;
		}
		return 1;
			
	}
	else if(!(strcmp(oper,"bge"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]))  
		{
			int rs1 = atoi(&operand1[1]);
			int rs2 = atoi(&operand2[1]);
			int label_address = find_label(operand3);
			int imm12 = label_address - pc;
			if(rs2>31 || rs1 >31 || imm12>2047 || imm12<-2048) return 1;
			if (label_address == -1) return 1;	// 라벨이 없으면 에러 처리
			if(reg[rs1] >= reg[rs2]){pc += imm12;}
			else {pc += 4;}
			return 0;
		}
		return 1;
	}
	else if(!(strcmp(oper,"blt"))){
		if(operand1[0] == 'x' && operand2[0] == 'x' &&
		isNumeric(&operand1[1]) && isNumeric(&operand2[1]))  
		{
			int rs1 = atoi(&operand1[1]);
			int rs2 = atoi(&operand2[1]);
			int label_address = find_label(operand3);
			int imm12 = label_address - pc;
			if(rs2>31 || rs1 >31 || imm12>2047) return 1;
			if (label_address == -1) return 1;	// 라벨이 없으면 에러 처리
			if(reg[rs1] < reg[rs2]){pc += imm12;}
			else {pc += 4;}
			return 0;
			}
		return 1;
	}
	//UJ-type
	else if(!(strcmp(oper,"jal"))){
		if(operand1[0] == 'x' && isNumeric(&operand1[1]))  
		{
			int rd = atoi(&operand1[1]);
			int label_address = find_label(operand2);
			if(label_address == -1){return 1;}
			int imm20 = label_address - pc;
			if(rd >31 || imm20>524287 || imm20<-524288) return 1;
			if(rd!=0){reg[rd] = pc + 4 ;} //rd = x0일때는 리턴주소저장x
			pc = pc + imm20;
			return 0;
		}
		return 1;
	}
	//Exit
	else if(!(strcmp(oper,"exit"))){
		return -1;
	}
	return 1;	//해당하는 명령어 없을 시 에러 code = 1

}
///////////////////////////////////////////////////////// R-type Inst//////////////////////////////////////////////////////////////////
void R_typeInst(int rd, int rs1, int rs2, int opcode, int func3, int func7){
	unsigned int outinst = (func7)<<25 | (rs2<<20) | (rs1<<15) | (func3<<12) | (rd<<7) | opcode;
	Make_Binary(outinst, output);
}
void Add(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b000;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);

}
void Sub(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b000;
	const int func7 = 0b0100000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void And(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b111;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void Or(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b110;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void Xor(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b100;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void Sll(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b001;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void Srl(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b101;
	const int func7 = 0b0000000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
void Sra(int rd, int rs1, int rs2){
	const int opcode = 0b0110011;
	const int func3 = 0b101;
	const int func7 = 0b0100000;
	R_typeInst(rd, rs1, rs2, opcode, func3, func7);
}
///////////////////////////////////////////////////////// I-type Inst//////////////////////////////////////////////////////////////////
void I_typeInst(int rd, int rs1, int opcode, int func3, int imm12){
	unsigned int outinst = (imm12<<20) | (rs1<<15) | (func3<<12) | (rd<<7) | opcode;
	Make_Binary(outinst, output);
}
void Addi(int rd, int rs1, int imm12){
	const int opcode = 0b0010011;
	const int func3 = 0b000;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
void Andi(int rd, int rs1, int imm12){
	const int opcode = 0b0010011;
	const int func3 = 0b111;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
void Ori(int rd, int rs1, int imm12){
	const int opcode = 0b0010011;
	const int func3 = 0b110;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
void Xori(int rd, int rs1, int imm12){
	const int opcode = 0b0010011;
	const int func3 = 0b100;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
void Slli(int rd, int rs1, int shamt){
	const int opcode = 0b0010011;
	const int func3 = 0b001;
	I_typeInst(rd, rs1, opcode, func3, shamt);
}
void Srli(int rd, int rs1, int shamt){
	const int opcode = 0b0010011;
	const int func3 = 0b101;
	I_typeInst(rd, rs1, opcode, func3, shamt);
}
void Srai(int rd, int rs1, int shamt){
	const int opcode = 0b0010011;
	const int func3 = 0b101;
	shamt = shamt | 0b010000000000;
	I_typeInst(rd, rs1, opcode, func3, shamt);
}
void Lw(int rd, int rs1, int imm12){
	const int opcode = 0b0000011;
	const int func3 = 0b010;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
void Jalr(int rd, int rs1, int imm12){
	const int opcode = 0b1100111;
	const int func3 = 0b000;
	I_typeInst(rd, rs1, opcode, func3, imm12);
}
///////////////////////////////////////////////////////// S-type Inst//////////////////////////////////////////////////////////////////
void S_typeInst(int rs1, int rs2, int opcode, int func3, int imm12){
	int imm11_5 = imm12>>5;
	int imm4_0 = imm12 & 0b11111;
	unsigned int outinst = (imm11_5<<25) | (rs2<<20) | (rs1<<15) | (func3<<12) | (imm4_0<<7) | opcode;
	Make_Binary(outinst, output);
}
void Sw(int rs1, int rs2, int imm12){
	const int opcode = 0b0100011;
	const int func3 = 0b010;
	S_typeInst(rs1, rs2, opcode, func3, imm12);
}
///////////////////////////////////////////////////////// SB-type Inst//////////////////////////////////////////////////////////////////
void SB_typeInst(int rs1, int rs2, int opcode, int func3, int imm12){
	imm12 = imm12>>1;
	int imm_12 = imm12 >> 11;
	int imm_11 = imm12 & 0b10000000000; imm_11 = imm_11 >> 10;
	int imm10_5 = imm12 & 0b01111110000; imm10_5 = imm10_5 >> 4;
	int imm4_1 = imm12 & 0b00000001111;
	unsigned int outinst = (imm_12<<31) | (imm10_5<<25) | (rs2<<20) | (rs1<<15) | (func3<<12) | (imm4_1<<8) | (imm_11<<7) | opcode;
	Make_Binary(outinst, output);
}
void Beq(int rs1, int rs2, int imm12){
	const int opcode = 0b1100011;
	const int func3 = 0b000;
	SB_typeInst(rs1, rs2, opcode, func3, imm12);
}
void Bne(int rs1, int rs2,  int imm12){
	const int opcode = 0b1100011;
	const int func3 = 0b001;
	SB_typeInst(rs1, rs2, opcode, func3, imm12);
}
void Bge(int rs1, int rs2, int imm12){
	const int opcode = 0b1100011;
	const int func3 = 0b101;
	SB_typeInst(rs1, rs2, opcode, func3, imm12);
}
void Blt(int rs1, int rs2,  int imm12){
	const int opcode = 0b1100011;
	const int func3 = 0b100;
	SB_typeInst(rs1, rs2, opcode, func3, imm12);
}
///////////////////////////////////////////////////////// UJ-type Inst//////////////////////////////////////////////////////////////////
void UJ_typeInst(int rd, int opcode, int imm20){
	imm20 = imm20>>1;
	int imm_20 = imm20 >> 19;
	int imm_11 = imm20 & 0b10000000000; imm_11 = imm_11 >> 10;
	int imm19_12 = imm20 & 0b1111111100000000000; imm19_12 >> 11;
	int imm10_1 = imm20 & 0b1111111111;
	unsigned int outinst = (imm_20<<31) | (imm10_1<<21) | (imm_11<<20) | (imm19_12<<12) | (rd<<7) | opcode;
	Make_Binary(outinst, output);
}
void Jal(int rd, int imm20){
	const int opcode = 0b1101111;
	UJ_typeInst(rd, opcode, imm20);
}


int main(void){
	FILE *input;
    	FILE *trace_file; // .trace 파일 포인터
	char input_filename[256];
	char trace_filename[256];
	char line[1024];

	//*.s file 한줄씩 불러오기
	while(1){
		int success = 1;
		printf("Enter Input File Name: ");
		scanf("%s", input_filename);
		if(!strcmp(input_filename, "terminate")){
			return 0;
		}
		input = fopen(input_filename, "r");

		if(input == NULL){
			printf("Input file does not exist!!\n");
			continue;
		}

		memset(labels, 0, sizeof(labels));
		memset(insts, 0, sizeof(insts));
		memset(reg, 0, sizeof(reg));
		memset(Mem, 0, sizeof(Mem));
		insts_cnt = 250;
		label_cnt = 0;
		pc = 1000;
		for(int i = 1 ; i<=6 ; i++) reg[i] = i; //register x1~x6 초기화
		
		// .o 파일 생성
		strncpy(output_filename,input_filename, strlen(input_filename)-2);
		output_filename[strlen(input_filename)-2] = '\0';
		strcat(output_filename, ".o");
		output = fopen(output_filename, "w");
		
		// .trace 파일 생성
		strncpy(trace_filename, input_filename, strlen(input_filename) - 2);
		trace_filename[strlen(input_filename) - 2] = '\0';
		strcat(trace_filename, ".trace");
		trace_file = fopen(trace_filename, "w");
		

		while (fgets(line, sizeof(line),input) != NULL){	//label획득
			parse_label(line);
		}
		pc = 1000;
		rewind(input);
		while (fgets(line, sizeof(line),input) != NULL){	//insts 획득
			success = parse_line(line);
			if(success == 0) break;
		}
		//실행
		
		pc = 1000;
		while(pc/4 < insts_cnt){
			reg[0] = 0; //x0는 항상 0
			if(pc<1000 || pc/4 >= insts_cnt){
				success=0;
				break;
			}
			// PC 이동 기록
           		fprintf(trace_file, "%d\n", pc);
           		int flag = inst_calc(insts[pc/4]);
			if(flag==1){ //1반환시 에러로 인식하고 반복문 탈출
				success = 0;
				break;
			}
			else if(flag == -1){	//exit처리
				break;
			}	
		}
		if(success){
			fclose(output);
			fclose(trace_file);
		}
		else{
			printf("Syntax Error!\n");
        		fclose(output); // 기존 파일 저장 중단
			fclose(trace_file);
		   	remove(output_filename); // .o 파일 삭제
		   	remove(trace_filename); // .trace 파일 삭제
		}
		fclose(input);
	}
}
