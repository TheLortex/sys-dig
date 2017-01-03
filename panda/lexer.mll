
(* Analyseur lexical pour bamboo *)

{
  open Lexing
  open Parser

  exception Lexing_error of string

  let kwd_tbl = ["alias",ALIAS;"and",AND;"xor",XOR;"or",OR;"nand",NAND;"not",NOT;"goto",GOTO;
  	"if",IF;"then",THEN;"else",ELSE;"end",END;"while",WHILE;"locif",LOCIF;"label",LABEL]
  let id_or_kwd s = try List.assoc s kwd_tbl with _ -> IDENT s

  let newline lexbuf =
    let pos = lexbuf.lex_curr_p in
    lexbuf.lex_curr_p <-
      { pos with pos_lnum = pos.pos_lnum + 1; pos_bol = pos.pos_cnum }

}
let reg = ['0'-'9' 'A'-'F']
let letter = ['a'-'z' 'A'-'Z']
let digit = ['0'-'9']
let ident = letter (letter | digit)*
let integer = ['0'-'9']+
let space = [' ' '\t']

rule token = parse
  | '\n'    { newline lexbuf; token lexbuf }
  | '#' [^'\n']* '\n' { newline lexbuf; token lexbuf }
  | space+  { token lexbuf }
  | ident as id { id_or_kwd id }
  | '+'     { PLUS }
  | '-'     { MINUS }
  | '*'     { TIMES }
  | '/'     { DIV }
  | '%'		{ REM }
  | 'r' (reg as r) {REG r}
  | '='     { EQ }
  | '('     { LP }
  | ')'     { RP }
  | ';'		{ ENTER }
  | '<'		{ LOAD }
  | '>'		{ STORE }
  | integer as s { if (String.length s)>5 then raise (Lexing_error "Constant is too large");
  					 let i = (int_of_string s) in if i>65536 then raise (Lexing_error "Constant is too large") else CST i }
  | eof     { EOF }
  | _ 		{ raise (Lexing_error ("Forbidden character")) }