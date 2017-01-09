
(* Analyseur lexical pour bamboo *)

{
  open Lexing
  open Parser
  open Arm

  exception Lexing_error of string

  let kwd_tbl = [
    "ADD",KWD ADD;
    "ADC",KWD ADC;
    "RSB",KWD RSB;
    "RSC",KWD RSC;
    "CMP",KWD CMP;
    "CMN",KWD CMN;
    "SUB",KWD SUB;
    "SBC",KWD SBC;
    "AND",KWD AND;
    "TST",KWD TST;
    "EOR",KWD EOR;
    "TEQ",KWD TEQ;
    "ORR",KWD ORR;
    "BIC",KWD BIC;
    "NOT",UNARY NOT;
    "MOV",UNARY MOV;
    "LDR",RAM LDR;
    "STR",RAM STR;
    "LSL",ROT LSL;
    "LSR",ROT LSR;
    "ASR",ROT ASR;
    "ROR",ROT ROR]
  let cond_tbl = [
    "EQ",EQ;
    "NQ",NQ;
    "CS",CS;
    "CC",CC;
    "MI",MI;
    "PL",PL;
    "VS",VS;
    "VC",VC;
    "HI",HI;
    "LS",LS;
    "GE",GE;
    "LT",LT;
    "GT",GT;
    "LE",LE;
    "AL",AL;
  ]


  let newline lexbuf =
    let pos = lexbuf.lex_curr_p in
    lexbuf.lex_curr_p <-
      { pos with pos_lnum = pos.pos_lnum + 1; pos_bol = pos.pos_cnum }

}
let reg = ['0'-'9' 'A'-'F']
let letter = ['A'-'Z']
let digit = ['0'-'9']
let integer = ['0'-'9']+
let space = [' ' '\t']

rule token = parse
  | '\n'    { newline lexbuf; token lexbuf }
  | '@' [^'\n']* '\n' { newline lexbuf; token lexbuf }
  | space+  { token lexbuf }
  | (letter letter letter) as kwd    {try List.assoc kwd kwd_tbl with _ -> raise (Lexing_error ("Instruction keyword not recognized:"^kwd))}
  | 'r' (reg as r) {REG r}
  | (letter letter) as cond   {try COND (List.assoc cond cond_tbl) with _ -> raise (Lexing_error ("Conditional keyword not recognized"^cond))}
  | 's'     { S }
  | ','     { COMMA }
  | '['     { O }
  | ']'     { C }
  | '#' (integer as i)    { CST (int_of_string i) }
  | eof     { EOF }
  | _ 		{ raise (Lexing_error ("Forbidden character")) }