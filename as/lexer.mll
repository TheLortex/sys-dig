
(* Analyseur lexical pour bamboo *)

{
  open Lexing
  open Parser
  open Arm

  exception Lexing_error of string

let kwd x = (fun (y,z)-> KWD (x,y,z))
let unary x = (fun (y,z) -> UNARY (x,y,z))
let ram x = (fun (y,z) -> RAM (x,y,z))
let rot x = (fun (y,z) -> ROT x)
let branch x = (fun (y,z) -> BRANCH (x,y,z))

  let kwd_tbl = [
    "ADD",kwd ADD;
    "ADC",kwd ADC;
    "RSB",kwd RSB;
    "RSC",kwd RSC;
    "CMP",unary CMP;
    "CMN",unary CMN;
    "SUB",kwd SUB;
    "SBC",kwd SBC;
    "AND",kwd AND;
    "TST",unary TST;
    "EOR",kwd EOR;
    "TEQ",unary TEQ;
    "ORR",kwd ORR;
    "BIC",kwd BIC;
    "NOT",unary NOT;
    "MOV",unary MOV;
    "LDR",ram LDR;
    "STR",ram STR;
    "LSL",rot LSL;
    "LSR",rot LSR;
    "ASR",rot ASR;
    "ROR",rot ROR]

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

  exception Invalid

let decode ident =
  let strlen = String.length ident in
  let decode_end ident =
    match String.length ident with
    | 0 -> (AL, false)
    | 1 -> (AL,if ident.[0] <> 'S' then raise Invalid else true)
    | 2 -> (List.assoc ident cond_tbl, false)
    | 3 -> (if ident.[2] <> 'S' then raise Invalid;List.assoc (String.sub ident 0 2) cond_tbl, true)
    | _ -> raise Invalid
  in
  let size, res = (
    try
     (3,List.assoc (if strlen < 3 then raise Not_found else String.sub ident 0 3) kwd_tbl)
   with
   | Not_found ->
     if (String.sub ident 0 2) = "BL" then (2,branch BL)
     else begin
       if ident.[0] = 'B' then (1,branch B) else (raise Invalid)
     end) in
  res (decode_end (String.sub ident size (strlen - size)))

  let newline lexbuf =
    let pos = lexbuf.lex_curr_p in
    lexbuf.lex_curr_p <-
      { pos with pos_lnum = pos.pos_lnum + 1; pos_bol = pos.pos_cnum }

}
let hex = ['0'-'9' 'A'-'F']
let letter = ['A'-'Z' 'a'-'z' '_']
let digit = ['0'-'9']
let integer = ['0'-'9']+
let space = [' ' '\t']

rule token = parse
  | '\n'    { newline lexbuf; token lexbuf }
  | '@' [^'\n']* '\n' { newline lexbuf; token lexbuf }
  | 'r' (hex as r) {REG r}
  | space+  { token lexbuf }
  | '#' ("0x" hex+ as i)    {CST (int_of_string i)}
  | '#' (integer as i)    { CST (int_of_string i) }
  | letter (letter | digit)* as ident    {
    try decode ident with | _ -> LABEL ident
  }
  | ','     { COMMA }
  | ':'     { COLON }
  | '['     { O }
  | ']'     { C }
  | eof     { EOF }
  | _ as c 		{ raise (Lexing_error ("Forbidden character "^(String.make 1 c))) }
