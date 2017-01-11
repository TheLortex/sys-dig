
open Format
open Shoots
open Ast

exception VarUndef of string

module Smap=Map.Make(String)

let cpt = ref 0

let rec compile_expr cc aliases  = function
 	| Imm i ->
		mov(cond cc)(imm i)(reg '2')
		
	| Reg r ->
		mov(cond cc)(reg r)(reg '2')

 	| Pointer x ->
 	 	(try let r = Smap.find x aliases in mov(cond cc)(reg r)(reg '2') 
 	 	with _-> mov(cond cc)(lab x)(reg '2'))
 	 	
 	| Val (x, e) ->
 		(try let r = Smap.find x aliases in mov(cond cc)(reg r)(reg '3')
 		with _-> mov(cond cc)(lab x)(reg '3') ) ++
 	 	compile_expr cc aliases e ++
 	 	mov(cond cc)(ind ~index:"%3" "%2")(reg '2')

 	| Binop (e1, o, e2)->
      compile_expr cc aliases e2 ++
      push(cond cc)(reg '2') ++
      compile_expr cc aliases e1 ++
      pop(cond cc)"%3" ++
      (match o with
        | Add -> add(cond cc)(reg '3') (reg '2')
        | Sub -> sub(cond cc)(reg '3') (reg '2')
        | Mul -> mul(cond cc)(reg '3') (reg '2')
        | Div -> failwith "jkkkkkkkkkeghqbveovfybioeuqrg"
        | Rem -> failwith "jkygsdjkfhgvjhdgqfouygoruihyg"
        | And -> ands(cond cc)(reg '3') (reg '2')
        | Or -> ors(cond cc)(reg '3') (reg '2')
        | Nand -> nands(cond cc)(reg '3') (reg '2')
        | Xor -> xors(cond cc)(reg '3') (reg '2') 
        | Eq -> sub(cond cc)(reg '3')(reg '2'))
        
    | Not e ->
    	compile_expr cc aliases e ++
    	not(cond cc)(reg '2')

let compile_cond aliases c = failwith "no cond yet"

let rec compile_instr cc = function
	| Label (x,_) -> label x
	| Goto (x,_) -> jmp(cond cc)x
	| Rename (r,x,_) -> nop
	| Set (r,e,aliases) -> compile_expr cc aliases e ++ mov(cond cc)(reg '2')(reg r)
	| Store (r,e,aliases) -> compile_expr cc aliases e ++
		store(cond cc)(reg r)(ind "%2")
	| Load (r,e,aliases) -> compile_expr cc aliases e ++
		load(cond cc)(ind "%2")(reg r)
	| Locif (c,thenil,elseil,aliases) -> if cc <> AL then failwith "Local if is nested!" else
		let code,thencc,elsecc = compile_cond aliases c in
		code ++ compile_block thencc thenil ++ compile_block elsecc thenil
	| If (c,thenil,elseil,aliases) -> if cc <> AL then failwith "Nested if in local if!" else
		let thencode = compile_block AL thenil and elsecode = compile_block AL elseil in
		let ccode,thenflag,elseflag = compile_cond aliases c in
		incr cpt; let s = string_of_int !cpt in
		ccode ++ jmp (cond elseflag)("else"^s) ++ thencode ++ jmp (cond AL)("end"^s) ++
		label ("else"^s) ++ elsecode ++ label("end"^s)
	| While (c,block,aliases) -> if cc <> AL then failwith "Nested while in local if!" else
		let code = compile_block AL block in
		let ccode,contflag,exitflag = compile_cond aliases c in
		incr cpt; let s = string_of_int !cpt in
		label ("while"^s) ++ ccode ++ jmp (cond exitflag)("end"^s) ++
		code ++ jmp (cond AL)("while"^s) ++ label ("end"^s)
and compile_block cc = List.fold_left (fun code instr -> code ++ compile_instr cc instr) nop

let oblig_code = nop,
	nop

let compile_program p ofile =
  let code = compile_block AL p in
  let pre,post = oblig_code in
  let p =
    { text =
        label "main" ++
        pre ++
        code ++
        post;
      data =
        nop
    }
  in
  let f = open_out ofile in
  let fmt = formatter_of_out_channel f in
  Shoots.print_program fmt p;
  fprintf fmt "@?";
  close_out f



(*
let compile_stmt (codefun, codemain) = function
  | Set (x, e, fpmax) ->
    let code = compile_expr e in
    let code =
      let pre, post = if fpmax > 0 then pushn fpmax, popn fpmax else nop, nop in
      pre ++ code ++ popq rax ++ movq (reg rax) (lab x) ++ post in
    codefun, codemain ++ code

  | Fun (f, e, fpmax) ->
    let code = compile_expr e in
    codefun ++ label f ++ code, codemain

  | Print (e, fpmax) ->
    let code = compile_expr e in
    let code =
      let pre, post = if fpmax > 0 then pushn fpmax, popn fpmax else nop, nop in
      pre ++ code ++ popq rdi ++ post ++ call "print_int"
    in
    codefun, codemain ++ code

let compile_program p ofile =
  let p = alloc p in
  let codefun, code = List.fold_left compile_stmt (nop, nop) p in
  let p =
    { text =
        glabel "main" ++
        movq (reg rsp) (reg rbp) ++
        code ++
        movq (imm 0) (reg rax) ++ (* exit *)
        ret ++
        label "print_int" ++
        movq (reg rdi) (reg rsi) ++
        movq (ilab ".Sprint_int") (reg rdi) ++
        movq (imm 0) (reg rax) ++
        call "printf" ++
        ret ++
        codefun;
      data =
        Hashtbl.fold (fun x _ l -> label x ++ dquad [1] ++ l) genv
          (label ".Sprint_int" ++ string "%d\n")
    }
  in
  let f = open_out ofile in
  let fmt = formatter_of_out_channel f in
  X86_64.print_program fmt p;
  fprintf fmt "@?";
  close_out f*)
