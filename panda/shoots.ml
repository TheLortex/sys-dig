(* Bibliothèque pour produire du code shoots
   2017 Malachi Voss (ENS Paris)
*)

open Format

type register =  string

let r0 = "%0"
let r1 = "%1"
let r2 = "%2"
let r3 = "%3"
let r4 = "%4"
let r5 = "%5"
let r6 = "%6"
let r7 = "%7"
let r8 = "%8"
let r9 = "%9"
let rA = "%A"
let rB = "%B"
let rC = "%C"
let rD = "%D"
let rE = "%E"
let rF = "%F"

type label = string

type 'size operand = formatter -> unit -> unit

let reg r = fun fmt () -> fprintf fmt "%s" r
let imm i = fun fmt () -> fprintf fmt "$%i" i
let ind ?(ofs=0) ?index ?(scale=1) r = fun fmt () -> match index with
  | None -> fprintf fmt "%d(%s)" ofs r
  | Some r1 -> fprintf fmt "%d(%s,%s,%d)" ofs r r1 scale
let lab (l: label) = fun fmt () -> fprintf fmt "%s" l
let ilab (l: label) = fun fmt () -> fprintf fmt "$%s" l

type 'a asm =
  | Nop
  | S of string
  | Cat of 'a asm * 'a asm

let nop = Nop
let inline s = S s
let (++) x y = Cat (x, y)

type text = [`text ] asm
type data = [`data ] asm

let buf = Buffer.create 17
let fmt = formatter_of_buffer buf
let ins x =
  Buffer.add_char buf '\t';
  kfprintf (fun fmt ->
    fprintf fmt "\n";
    pp_print_flush fmt ();
    let s = Buffer.contents buf in
    Buffer.clear buf;
    S s
  ) fmt x

let pr_list fmt pr = function
  | []      -> ()
  | [i]     -> pr fmt i
  | i :: ll -> pr fmt i; List.iter (fun i -> fprintf fmt ", %a" pr i) ll

let pr_ilist fmt l =
  pr_list fmt (fun fmt i -> fprintf fmt "%i" i) l

let pr_alist fmt l =
  pr_list fmt (fun fmt (a : label) -> fprintf fmt "%s" a) l

let mov cond a b = ins "mov %a, %a, %a" cond () a () b ()

let lea cond op r = ins "lea %a, %a, %s" cond () op () r

let add cond a b = ins "add %a, %a, %a" cond () a () b ()

let sub cond a b = ins "sub %a, %a, %a" cond () a () b ()

let mul cond a b = ins "mul %a, %a, %a" cond () a () b ()

(* DIV? REM?
let div cond a = ins "idivq %a" a ()
*)

let not cond a = ins "not %a, %a" cond () a ()

let ands cond a b = ins "and %a, %a, %a" cond () a () b ()
let ors cond a b = ins "or %a, %a, %a" cond () a () b ()
let nands cond a b = ins "nand %a, %a, %a" cond () a () b ()
let xors cond a b = ins "xor %a, %a, %a" cond () a () b ()

let jmp cond (z: label) = ins "jmp %a, %s" cond () z

(* I? O? RET?
let call (z: label) = ins "call %s" z
let call_star z = ins "call *%a" z ()
let ret = ins "ret"
*)

let cmp cond a b = ins "cmp %a, %a, %a" cond () a () b ()
let test cond a b = ins "test %a, %a, %a" cond () a () b ()

let label (s : label) = S ("Label" ^ s ^ "\n")

let comment s = S ("#" ^ s ^ "\n")

let dint l = ins ".int %a" pr_ilist l

let pushq a = ins "pushq %a" a ()
let popq r = ins "popq %s" r

type program = {
  text : [ `text ] asm;
  data : [ `data ] asm;
}

let rec pr_asm fmt = function
  | Nop          -> ()
  | S s          -> fprintf fmt "%s" s
  | Cat (a1, a2) -> pr_asm fmt a1; pr_asm fmt a2

let print_program fmt p =
  fprintf fmt ".text\n";
  pr_asm fmt p.text;
  fprintf fmt ".data\n";
  pr_asm fmt p.data;
  pp_print_flush fmt ()

let print_in_file ~file p =
  let c = open_out file in
  let fmt = formatter_of_out_channel c in
  print_program fmt p;
  close_out c