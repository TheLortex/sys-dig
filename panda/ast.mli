	(* AST for bamboo *)
	
	(* Comments are of form &...........\n *)
	 
	(* DO NOT:
		-> do aliases inside conditional structures
		-> stick anything besides registers/direct variables in conditional tests
		-> call ANYTHING main*)
type ident = string
type register = char
type binop = Add|Sub|Mul|Div|Rem|And|Or|Xor|Nand|Eq
type expr =
	|Val of ident * expr
	|Pointer of ident
	|Imm of int
	|Reg of register
	|Binop of expr * binop * expr
	|Not of expr
type 'a instr =
	|Label of ident * 'a
	|Goto of ident * 'a
	|Rename of register * ident * 'a
	|Set of register * expr * 'a
	|Store of register * expr * 'a
	|Load of register * expr * 'a
	|Locif of expr * ('a instr list) * ('a instr list) * 'a
	|If of expr * ('a instr list) * ('a instr list) * 'a
	|While of expr * ('a instr list) * 'a
type 'a program = 'a instr list
