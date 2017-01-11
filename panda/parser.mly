
/* Analyseur syntaxique pour bamboo */

%{
  open Ast
  
  module Smap=Map.Make(String)
  type aliases=register Smap.t
  let aliases=ref Smap.empty
%}

%token <int> CST
%token <string> IDENT
%token <char> REG
%token IF THEN ELSE WHILE END LOCIF
%token AND OR XOR NAND NOT
%token ALIAS
%token EOF
%token LP RP ENTER
%token PLUS MINUS TIMES DIV REM
%token EQ LOAD STORE
%token LABEL GOTO
%token VAL DOT

/* Définitions des priorités et associativités des tokens */
%left AND OR XOR NAND
%nonassoc NOT
%nonassoc EQ
%left PLUS MINUS
%left TIMES DIV REM
%nonassoc uminus
%nonassoc DOT

/* Point d'entrée de la grammaire */
%start prog

/* Type des valeurs retournées par l'analyseur syntaxique */
%type < 'a Ast.program> prog

%%


prog:
| l=list(instr) EOF { l }
;

instr:
| id=IDENT ALIAS r=REG ENTER { aliases:= Smap.add id r !aliases; Rename (r, id, !aliases) }
| id=IDENT EQ e=expr ENTER { Set (Smap.find id !aliases, e, !aliases) }
| r=REG EQ e=expr ENTER { Set (r, e, !aliases) }
| id=IDENT LOAD e=expr ENTER { Load (Smap.find id !aliases, e, !aliases) }
| r=REG LOAD e=expr ENTER { Load (r, e, !aliases) }
| id=IDENT STORE e=expr ENTER { Store (Smap.find id !aliases, e, !aliases) }
| r=REG STORE e=expr ENTER { Store (r, e, !aliases) }
| IF e=expr ENTER THEN thenl=list(instr) ELSE elsel=list(instr) END ENTER { If (e, thenl, elsel, !aliases) }
| LOCIF e=expr ENTER THEN thenl=list(instr) ELSE elsel=list(instr) END ENTER { Locif (e, thenl, elsel, !aliases) }
| WHILE e=expr ENTER body=list(instr) END ENTER { While (e, body, !aliases) }
| LABEL id=IDENT ENTER { Label (id, !aliases) }
| GOTO id=IDENT ENTER { Goto (id, !aliases) }
;

expr:
| c=CST                        { Imm c }
| id=IDENT                     { Pointer id }
| VAL id=IDENT				   { Val (id, Imm 0) }
| VAL id=IDENT DOT e=expr	   { Val (id, e) }
| r=REG						   { Reg r }
| e1=expr op=binop e2=expr     { Binop (e1,op, e2) }
| MINUS e=expr %prec uminus    { Binop (Imm 0, Sub, e) }
| NOT e=expr 				   { Not e }
| LP e=expr RP                 { e }
;

%inline binop:
| PLUS  { Add }
| MINUS { Sub }
| TIMES { Mul }
| DIV   { Div }
| REM	{ Rem }
| AND	{ And }
| OR	{ Or }
| XOR	{ Xor }
| NAND	{ Nand }
| EQ	{ Eq }
;


