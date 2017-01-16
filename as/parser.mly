

%{
	open Arm

	let pc = ref(-1)
%}

%token <int> CST
%token <char> REG
%token <Arm.instr_dec> KWD
%token <Arm.instr_dec> UNARY
%token <Arm.instr_dec> RAM
%token <Arm.instr_dec> BRANCH
%token <Arm.rot> ROT
%token <string> LABEL
%token EOF COMMA O C COLON
/* Point d'entrée de la grammaire */
%start prog

/* Type des valeurs retournées par l'analyseur syntaxique */
%type <Arm.code list> prog

%%


prog:
| l=list(instruction) EOF { l }
;

instruction:
| instr_dec=BRANCH lbl=LABEL {
	let instr,cond,set_flags = instr_dec in
	pc := !pc + 1;Branch (instr,cond,lbl,!pc)
}
| lbl=LABEL COLON {DeclLabel(lbl,!pc+1)}
| ramop_dec=RAM rd=REG COMMA O ra=REG C {pc := !pc + 1;Instr(0)}
| instr_dec=KWD rd=REG r1=registre COMMA i=CST {
	let instr,cond,set_flags = instr_dec in
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = encode_reg r1
	and cst = encode_cst i
	and s = if set_flags then 1 else 0
	in
	pc := !pc + 1;Instr((cond lsl 28) + (1 lsl 25) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + cst)
	} /*Constante immédiate dans l'opérande 2 (en appliquant la bonne rotation)*/
| instr_dec=KWD rd=REG r1=registre r2=registre shift=option(shift) {
	let instr,cond,set_flags = instr_dec in
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = encode_reg r1
	and r2 = encode_reg r2
	and shift = match shift with | Some (rot,i) -> encode_shift_cst rot i| None -> 0
	and s = if set_flags then 1 else 0
	in
	pc := !pc + 1;Instr((cond lsl 28) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + (shift lsl 4) + r2)
	} /*1 ou 2 registres selon l'opération + barrel roll éventuel*/
| instr_dec=UNARY rd=REG COMMA i=CST {
	let instr,cond,set_flags = instr_dec in
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = encode_reg rd
	and cst = encode_cst i
	and s = if set_flags then 1 else 0
	in
	pc := !pc + 1;Instr((cond lsl 28) + (1 lsl 25) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + cst)
	} /*Constante immédiate dans l'opérande 2 (en appliquant la bonne rotation)*/
| instr_dec=UNARY rd=REG r2=registre shift=option(shift) {
		let instr,cond,set_flags = instr_dec in
		let cond = encode_cond cond
		and instr = encode_instr instr
		and rd = encode_reg rd
		and r1 = encode_reg rd
		and r2 = encode_reg r2
		and shift = match shift with | Some (rot,i) -> encode_shift_cst rot i| None -> 0
		and s = if set_flags then 1 else 0
		in
		pc := !pc + 1;Instr((cond lsl 28) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + (shift lsl 4) + r2)
	}

shift:
| COMMA rot=ROT i=CST {rot,i}
| COMMA rot=ROT i=REG {rot,encode_reg i}


registre:
| COMMA r1=REG {r1}
;
