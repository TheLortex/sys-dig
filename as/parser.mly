

%{
	open Arm
%}

%token <int> CST
%token <char> REG
%token <Arm.instr> KWD
%token <Arm.instr> UNARY
%token <Arm.rot> ROT
%token <Arm.cond> COND
%token <Arm.ramop> RAM
%token EOF COMMA S O C
/* Point d'entrée de la grammaire */
%start prog

/* Type des valeurs retournées par l'analyseur syntaxique */
%type <int list> prog

%%


prog:
| l=list(instruction) EOF { l }
;

instruction:
| ramop=RAM cond=option(conditional) rd=REG COMMA O ra=REG C {0}
| instr=KWD cond=option(conditional) s=option(setflags) rd=REG r1=registre COMMA i=CST {
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = encode_reg r1
	and cst = encode_cst i
	and s = match s with | Some _ -> 1 | None -> 0
	in
	(cond lsl 28) + (1 lsl 25) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + cst
	} /*Constante immédiate dans l'opérande 2 (en appliquant la bonne rotation)*/
| instr=KWD cond=option(conditional) s=option(setflags) rd=REG r1=registre r2=registre shift=option(shift) {
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = encode_reg r1
	and r2 = encode_reg r2
	and shift = match shift with | Some (rot,i) -> encode_shift_cst rot i| None -> 0
	and s = match s with | Some _ -> 1 | None -> 0
	in
	(cond lsl 28) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + (shift lsl 4) + r2
	} /*1 ou 2 registres selon l'opération + barrel roll éventuel*/
| instr=UNARY cond=option(conditional) s=option(setflags) rd=REG COMMA i=CST {
	let cond = encode_cond cond
	and instr = encode_instr instr
	and rd = encode_reg rd
	and r1 = 0
	and cst = encode_cst i
	and s = match s with | Some _ -> 1 | None -> 0
	in
	(cond lsl 28) + (1 lsl 25) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + cst
	} /*Constante immédiate dans l'opérande 2 (en appliquant la bonne rotation)*/
| instr=UNARY cond=option(conditional) s=option(setflags) rd=REG r2=registre shift=option(shift) {
		let cond = encode_cond cond
		and instr = encode_instr instr
		and rd = encode_reg rd
		and r1 = 0
		and r2 = encode_reg r2
		and shift = match shift with | Some (rot,i) -> encode_shift_cst rot i| None -> 0
		and s = match s with | Some _ -> 1 | None -> 0
		in
		(cond lsl 28) + (1 lsl 25) + (instr lsl 21) + (s lsl 20) + (rd lsl 16) + (r1 lsl 12) + (shift lsl 4) + r2
	}

shift:
| COMMA rot=ROT i=CST {rot,i}
| COMMA rot=ROT i=REG {rot,encode_reg i}

conditional:
| c=COND {c}

setflags:
| S {1}

registre:
| COMMA r1=REG {r1}
;
