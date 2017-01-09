type instr = ADD|ADC|RSB|RSC|CMP|CMN|SUB|SBC|AND|TST|EOR|TEQ|ORR|BIC|NOT|MOV
type ramop = LDR|STR 
type cond = EQ|NQ|CS|CC|MI|PL|VS|VC|HI|LS|GE|LT|GT|LE|AL 
type rot = LSL|LSR|ASR|ROR

exception IntegerConstantFailed of int 

let encode_cst i = 
	let n = ref(0) and c = ref(i) in 
	while (!c >= 256) do 
		if !c mod 4 == 0 then
			(c := !c / 4; incr n)
		else 
			raise (IntegerConstantFailed i)
	done; 
	!n lsl 8 + !c 


let encode_cond cond = 
	match cond with 
	| Some cond -> begin
		match cond with 
		| EQ -> 0
		| NQ -> 1
		| CS -> 2
		| CC -> 3
		| MI -> 4
		| PL -> 5
		| VS -> 6
		| VC -> 7
		| HI -> 8
		| LS -> 9
		| GE -> 10
		| LT -> 11
		| GT -> 12
		| LE -> 13
		| AL -> 15
	end
	| None -> 15


let encode_instr instr = 
	match instr with 
	| ADD -> 0
	| ADC -> 1
	| RSB -> 2
	| RSC -> 3
	| CMP -> 4
	| CMN -> 5
	| SUB -> 6
	| SBC -> 7
	| AND -> 8
	| TST -> 9
	| EOR -> 10
	| TEQ -> 11 
	| ORR -> 12
	| BIC -> 13
	| NOT -> 14
	| MOV -> 15

let encode_reg reg = 
	match reg with 
	| '1' -> 1
	| '2' -> 2
	| '3' -> 3
	| '4' -> 4
	| '5' -> 5
	| '6' -> 6
	| '7' -> 7
	| '8' -> 8
	| '9' -> 9
	| 'A' -> 10
	| 'B' -> 11
	| 'C' -> 12
	| 'D' -> 13
	| 'E' -> 14
	| 'F' -> 15

let encode_shift shift cst = 0 

