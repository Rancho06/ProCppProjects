%{
#include "node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* g_MainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN TLBRACKET TRBRACKET
%token <token> TMAIN TROTATE TFORWARD TATTACK TSAVELOC TRANGEDATTACK
%token <token> TASSIGN TIF TELSE TMEMORY TINC TDEC
%token <token> TISZOMBIE TISHUMAN TISRANDOM TISPASSABLE
/* Statements */
%type <block> main_loop block condition
%type <statement> statement rotate forward attack save_loc ranged_attack mem_change
 
/* Expressions */
%type <numeric> numeric
%type <boolean> boolean

%%

main_loop	: TMAIN TLBRACE block TRBRACE { 
	g_MainBlock = $3;
	$3 -> SetMainBlock();
}
;

block		: statement { 
				$$ = new NBlock();
				$$ -> AddStatement($1);
			}
			| block statement {
				$$ -> AddStatement($2);
			}
;

condition	: TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE {
				$$ = new NIf($3, $6);
			}
			| TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE {
				$$ = new NIfElse($3, $6, $10);
				
			}
;

statement	: rotate TSEMI
			| forward TSEMI
			| attack TSEMI
			| ranged_attack TSEMI
			| save_loc TSEMI
			| condition
			| mem_change TSEMI
;
			
rotate		: TROTATE TLPAREN numeric TRPAREN {
				$$ = new NRotate($3);
			}
;

forward		: TFORWARD TLPAREN TRPAREN {
				$$ = new NForward();
			}
;
		
attack		: TATTACK TLPAREN TRPAREN {
				$$ = new NAttack();
			}
;

ranged_attack: TRANGEDATTACK TLPAREN TRPAREN {
				$$ = new NRangedAttack();
			}
;

mem_change	: TMEMORY TLBRACKET numeric TRBRACKET TINC {
				$$ = new NMemInc($3);
			}
			| TMEMORY TLBRACKET numeric TRBRACKET TDEC {
				$$ = new NMemDec($3);
			}
			| TMEMORY TLBRACKET numeric TRBRACKET TASSIGN numeric {
				$$ = new NMemAssign($3, $6);
			}
;

save_loc	: TSAVELOC TLPAREN TRPAREN {
				$$ = new NSaveLoc();
			}
;

boolean		: TISHUMAN TLPAREN numeric TRPAREN {
				$$ = new NIsHuman($3);
			}
			| TISZOMBIE TLPAREN numeric TRPAREN {
				$$ = new NIsZombie($3);
			}
			| TISRANDOM TLPAREN TRPAREN {
				$$ = new NIsRandom();
			}
			| TISPASSABLE TLPAREN TRPAREN {
				$$ = new NIsPassable();
			}
;


numeric		: TINTEGER {
				$$ = new NNumeric(*($1));
			}
;

%%
