type instr = LDR|STR|ADD|ADC|RSB|RSC|CMP|B|BL|CMN|SUB|SBC|AND|TST|EOR|TEQ|ORR|BIC|NOT|MOV
type cond = EQ|NQ|CS|CC|MI|PL|VS|VC|HI|LS|GE|LT|GT|LE|AL
type instr_dec = instr*cond*bool
type rot = LSL|ASL|LSR|ASR|ROR|RRX
exception IntegerConstantFailed of int


type code = Instr of int | Branch of (instr*cond*string*int) | DeclLabel of (string*int)

let encode_cst i =
  let n = ref(0) and c = ref(i) in
  if (!c mod (1 lsl 24) < 16)
  then
    begin
      (4 lsl 8) + (!c lsl 4) + (!c lsr 28)
    end
  else
    begin
    	while (!c >= 256) do
    		if !c mod 4 = 0 then
    			(c := !c / 4; incr n)
    		else
    			raise (IntegerConstantFailed i)
    	done;
     (if (!n = 0) then 0 else (16-(!n))) lsl 8 + !c
    end


let encode_cond cond =
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
	| ORR -> 10
	| BIC -> 11
	| NOT -> 12
	| MOV -> 13
	| EOR -> 14
	| TEQ -> 15

let encode_reg reg =
	match reg with
	| '0' -> 0
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

let encode_shift shift =
	match shift with
	| LSL | ASL -> 0
	| ROR -> 1
	| LSR -> 2
	| ASR -> 3
	| RRX -> 3

let encode_shift_cst shift cst =
	if cst > 31 || cst < 0 then raise(IntegerConstantFailed cst);
	let shift_code = encode_shift shift in
	if shift = RRX then
		(shift_code lsl 1)
	else
		(cst lsl 3) + (shift_code lsl 1)

let encode_shift_reg shift reg =
	let shift_code = encode_shift shift
	and reg_code = encode_reg reg in
	(reg_code lsl 4) + (shift_code lsl 1) + 1
