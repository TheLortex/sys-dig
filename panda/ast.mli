type ident = string
type register = char
type binop = Add|Sub|Mul|Div|Rem|And|Or|Xor|Nand|Eq
type expr =
	|Var of ident
	|Imm of int
	|Reg of register
	|Binop of expr * binop * expr
	|Not of expr
type instr =
	|Label of ident
	|Goto of ident
	|Rename of register * ident
	|Set of register * expr
	|Store of register * expr
	|Load of register * expr
	|Locif of expr * (instr list) * (instr list)
	|If of expr * (instr list) * (instr list)
	|While of expr * (instr list)
type program = instr list