#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "gen_code.h"
#include "scanner.h"
#include "str.h"


#define LENGTH 50


struct str_struct code20;

void gen_code_start()
{
	str_add_const_str(&code20, "#Zacatek programu\n");
	str_add_const_str(&code20, ".IFJcode20\n");
	str_add_const_str(&code20, "DEFVAR GF@y");
	str_add_const_str(&code20, "DEFVAR GF@strA");
    str_add_const_str(&code20, "DEFVAR GF@strB");
    str_add_const_str(&code20, "DEFVAR GF@strC");
	str_add_const_str(&code20, "JUMP main\n");

}


void gen_value(struct token *token)
{
	char act_value[LENGTH];

	switch(token->type){
		case T_TYPE_INTEGER:
			str_add_const_str(&code20, "int@");
			sprintf(act_value, "%d", token->atribute.int_literal);
			str_add_const_str(&code20, act_value);
			break;
		case T_TYPE_DOUBLE:
			str_add_const_str(&code20, "float@");
			sprintf(act_value, "%g", token->atribute.double_literal);
			str_add_const_str(&code20, act_value);
			break;
		case T_TYPE_STRING:
			break;
		case T_TYPE_IDENTIFIER:
			str_add_const_str(&code20, "TF@");			
			str_add_const_str(&code20, token->atribute.string);
			break;
	}
}


void main_func()
{
	str_add_const_str(&code20, "#Hlavni funcke main\n");
	str_add_const_str(&code20, "LABEL main\n");
	str_add_const_str(&code20, "CREATEFRAME\n");		
}


void func_beg(char func_name[])
{
	str_add_const_str(&code20, "#Zacatek funkce ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "LABEL ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "PUSHFRAME\n");	
}


void func_fin(char func_name[])
{
	str_add_const_str(&code20, "#Konec funkce ");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "LABEL fin_");
	str_add_const_str(&code20, func_name);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "POPFRAME\n");

	str_add_const_str(&code20, "RETURN\n");	
}


void call_func(char func_name[])
{
	str_add_const_str(&code20, "CALL ");	
	str_add_const_str(&code20, func_name);	
	str_add_const_str(&code20, "\n");	
}


void before_call_func_params(struct token *token, int index)
{
	char tmp[LENGTH];

	str_add_const_str(&code20, "DEFVAR TF@%");
	sprintf(tmp, "%d", index);
	str_add_const_str(&code20, tmp);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "MOVE TF@%");	
	str_add_const_str(&code20, tmp);	
	str_add_const_str(&code20, " ");	
	gen_value(token);
	str_add_const_str(&code20, "\n");	
}


void after_func_beg_params(int index)
{
	char tmp[LENGTH];

	str_add_const_str(&code20, "DEFVAR LF@param");
	sprintf(tmp, "%d", index);
	str_add_const_str(&code20, tmp);	
	str_add_const_str(&code20, "\n");	

	str_add_const_str(&code20, "MOVE LF@param");
	str_add_const_str(&code20, tmp);	
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "LF@%");
	str_add_const_str(&code20, tmp);	
	str_add_const_str(&code20, "\n");	
}


void func_retval(char value[], int index)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "DEFVAR LF@%retval");
	str_add_const_str(&code20, tmp);
	str_add_const_str(&code20, "\n");

	str_add_const_str(&code20, "MOVE LF@%retval");
	str_add_const_str(&code20, tmp);
	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, value);
	str_add_const_str(&code20, "\n");	
}


void retval_to_y(int index)
{
	printf("MOVE GF@y TF@%cretval%d\n", '%', index);	
}

//loc = TF nebo LF
void make_var(char var_name[], char loc[], int depth)
{
	printf("DEFVAR %s@%s$%d\n", loc, var_name, depth);
}


void gen_label(char func_name[], int index, int depth, char ee[])
{
	str_add_const_str(&code20, "");
	printf("LABEL $%s$%d$%d$%s\n", func_name, index, depth, ee);
}


void gen_if_start()
{
	str_add_const_str(&code20, "#Tady zacina if\n");	
}


void gen_if(char func_name[], int index, int depth)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$else\n");	
}


void gen_if_else(char func_name[], int index, int depth)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$end\n");

	str_add_const_str(&code20, "#Else cast\n");
	
	gen_label(func_name, index, index, "else");
}


void gen_if_end(char func_name[], int index, int depth)
{	
	gen_label(func_name, index, depth, "if_end");
}


void gen_for_start()
{
	str_add_const_str(&code20, "#Tady zacina for\n");	
}


void gen_for(char func_name[], int index, int depth, char ee[])
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMPIFNEQ $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, ee);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "TF@RES$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, " ");
	str_add_const_str(&code20, "bool@true\n");	
}


void gen_for_end(char func_name[], int index, int depth)
{
	char tmp[LENGTH];
	sprintf(tmp, "%d", index);

	str_add_const_str(&code20, "JUMP $");
	str_add_const_str(&code20, func_name);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$");
	str_add_const_str(&code20, tmp);

	str_add_const_str(&code20, "$FOR\n");
	
	gen_label(func_name, index, depth, "for_end");	
}


void gen_arithmetic(Prec_rules symb)
{
	switch(symb){
		case E_PLUS_E:
			str_add_const_str(&code20, "ADDS\n");
			break;
		case E_MINUS_E:
			str_add_const_str(&code20, "SUBS\n");
			break;
		case E_MUL_E:
			str_add_const_str(&code20, "MULS\n");
			break;
		case E_DIV_E:
			str_add_const_str(&code20, "DIVS\n");
			break;
	}
}

void push_value(struct token *token)
{
    str_add_const_str(&code20, "PUSHS\n");
    str_add_const_str(&code20, " ");    
    gen_value(token);
    str_add_const_str(&code20, "\n");
}


void concat_strings()
{
	str_add_const_str(&code20, "POPS GF@strC\n");
	str_add_const_str(&code20, "POPS GF@strB\n");
	str_add_const_str(&code20, "CONCAT GF@strA GF@strB GF@strC\n");
	str_add_const_str(&code20, "PUSHS GF@strA\n");	
}




