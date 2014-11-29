%option nounistd

%{
#include <string>
#include <iostream>
#include <io.h>
#include "node.h"
#include "parser.hpp"
extern YYSTYPE zompilerlval;
#define SAVE_TOKEN zompilerlval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (zompilerlval.token = t)
extern "C" int yywrap() { return 1; }
int g_LineNumber = 1;

// Disable warnings
#pragma warning(disable: 4018)
#pragma warning(disable: 4996)
%}

%%

[ \t]					{ }
[0-9]+					{ SAVE_TOKEN; return TINTEGER; }

"main_loop"				{ return TOKEN(TMAIN); }
"{"						{ return TOKEN(TLBRACE); }
"}"						{ return TOKEN(TRBRACE); }
";"						{ return TOKEN(TSEMI); }
"("						{ return TOKEN(TLPAREN); }
")"						{ return TOKEN(TRPAREN); }
"["						{ return TOKEN(TLBRACKET); }
"]"						{ return TOKEN(TRBRACKET); }
"="						{ return TOKEN(TASSIGN); }

"if"					{ return TOKEN(TIF); }
"else"					{ return TOKEN(TELSE); }
"rotate"				{ return TOKEN(TROTATE); }
"forward"				{ return TOKEN(TFORWARD); }
"is_human"				{ return TOKEN(TISHUMAN); }
"is_zombie"				{ return TOKEN(TISZOMBIE); }
"is_passable"			{ return TOKEN(TISPASSABLE); }
"is_random"				{ return TOKEN(TISRANDOM); }
"ranged_attack"			{ return TOKEN(TRANGEDATTACK); }
"attack"				{ return TOKEN(TATTACK); }
"save_loc"				{ return TOKEN(TSAVELOC); }
"memory"				{ return TOKEN(TMEMORY); }
"++"					{ return TOKEN(TINC); }
"--"					{ return TOKEN(TDEC); }

\/\/.*\n				{ g_LineNumber++; }

%{
/* Do not add any of your own tokens below this line!!!! */
%}

"\n"					{ g_LineNumber++; }

[a-zA-Z_]+				{ std::cout << "ERROR: Unknown token '" << yytext << "' on line " << g_LineNumber << std::endl; yyterminate(); }

.						{ std::cout << "ERROR: Unknown token '" << yytext << "' on line " << g_LineNumber << std::endl; yyterminate(); }

%%
