
open Format
open Shoots
open Ast

(* phase 1 : allocation des variables 

exception VarUndef of string

let (genv : (string, unit) Hashtbl.t) = Hashtbl.create 17

module Smap = Map.Make(String)

type local_env = ident Smap.t

let rec alloc_expr env next = function
  | PCst i ->
    Cst i, next

  | PVar x -> if Smap.mem x env then LVar (Smap.find x env), next
    else if Hashtbl.mem genv x then GVar x, next
    else raise(VarUndef(x))

  | PBinop (o, e1, e2)->
    let (expr1,nnext) = alloc_expr env next e1 in
    let (expr2,nnnext) = alloc_expr env nnext e2 in
    Binop (o, expr1, expr2), nnnext

  | PLetin (x, e1, e2) ->
    let (expr1,nnext) = alloc_expr env next e1 in
    let nenv = Smap.add x (nnext) env in
    let nnext = nnext + 8 in
    let (expr2,next) = alloc_expr nenv nnext e2 in
    Letin (Smap.find x nenv, expr1, expr2), next

  | PCall (f, l) ->
    let g (exprl,next) e = let (expr,next) = alloc_expr env next e in expr::exprl,next in
    let exprl,nnext = List.fold_left g ([],next) l in
    Call (f, exprl), nnext

let alloc_stmt = function
  | PSet (x, e) ->
    let (expr,next) = alloc_expr Smap.empty 0 e in
    Hashtbl.replace genv x (); Set (x, expr, next)

  | PFun (f, l, e) ->
    let env,i = List.fold_left (fun (env,i) id -> Smap.add id i env,i+8) (Smap.empty,0) l in
    let (expr,next) = alloc_expr env i e in
    Fun (f, expr, next)

  | PPrint e ->
    let e, fpmax = alloc_expr Smap.empty 0 e in
    Print (e, fpmax)

let alloc = List.map alloc_stmt

(******************************************************************************)
(* phase 2 : production de code *)

let popn n = addq (imm n) (reg rsp)
let pushn n = subq (imm n) (reg rsp)

let rec compile_expr = function
  | Cst i ->
      pushq (imm i)

  | LVar fp_x ->
      pushq (ind ~ofs:fp_x rbp)

  | GVar x ->
      pushq (lab x)

  | Binop (o, e1, e2)->
      compile_expr e1 ++
      compile_expr e2 ++
      popq rbx ++ popq rax ++
      (match o with
        | Add -> addq (reg rbx) (reg rax)
        | Sub -> subq (reg rbx) (reg rax)
        | Mul -> imulq (reg rbx) (reg rax)
        | Div -> cqto ++ idivq (reg rbx)) ++
       pushq (reg rax)

  | Letin (ofs, e1, e2) ->
      compile_expr e1 ++
      popq rax ++ movq (reg rax) (ind ~ofs rbp) ++
      compile_expr e2

  | Call (f, l) ->
      failwith "à compléter"

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
  close_out f *)