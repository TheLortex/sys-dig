open Parser
open Format
open Arm

let ifile = ref ""
let ofile = ref ""
let fat   = ref false

let set_file f s = f := s
let set_flag () = fat := true

let options =
  ["-o", Arg.String (set_file ofile),
   "<file>  Pour indiquer le mom du fichier de sortie";
    "-b", Arg.Unit (set_flag),
    "Activer le mode gros"]

let output_fat_binary_int ofile i =
  let rec aux n p =
    if n == 0 then ()
    else (aux (n-1) (p/2); output_char ofile (if (p mod 2 == 0) then '0' else '1'))
  in
    aux 32 i; output_char ofile '\n'


 let () =
  (* Parsing de la ligne de commande *)
  Arg.parse options (set_file ifile) "";

  (* On vérifie que le nom du fichier source a bien été indiqué *)
  if !ifile="" then begin eprintf "Aucun fichier à compiler\n@?"; exit 1 end;

  (* Ce fichier doit avoir l'extension .exp *)
  if not (Filename.check_suffix !ifile ".arm") then begin
    eprintf "Le fichier d'entrée doit avoir l'extension .arm\n@?";
    Arg.usage options "";
    exit 1
  end;

  (* Par défaut, le fichier cible a le même nom que le fichier source,
     seule l'extension change *)
  if !ofile="" then ofile := Filename.chop_suffix !ifile ".arm" ^ ".out";

  (* Ouverture du fichier source en lecture *)
  let f = open_in !ifile in

  (* Création d'un tampon d'analyse lexicale *)
  let buf = Lexing.from_channel f in

  try
    let p = Parser.prog Lexer.token buf in
    close_in f;
    let f = open_out !ofile in
    List.iter (fun i ->
      if !fat then output_fat_binary_int f i
              else output_binary_int f i) p;
    close_out f;

  with
  	| Lexer.Lexing_error c -> print_string ("Erreur de lexing. "^c^"\n")
    | Parsing.Parse_error ->  print_string "Erreur de parsing.\n"
  	| IntegerConstantFailed i-> print_string ("Erreur d'intégration d'entier."^(string_of_int i)^"\n")
