================================================================================
========================= PROJET SYSTÈME DIGITAL ===============================
================================================================================

Afin de compiler le projet il faut mener à bien la compilation dans l'ordre des
différentes sous parties du projet.

* minijazz: une version modifiée de minijazz a été utilisée. Elle autorise les
opérations binaires sur des nappes de fils.
 -> Compilation en tapant "ocamlbuild -use-menhir mjc.native" dans le dossier.

* mjblocks: contient la description en langage minijazz du processeur.

* as: assembleur transformant le jeu d'instructions en code binaire.
-> Compilation en tapant "make" dans le dossier.

* simulateur_netlist: prend en entrée une netlist issue de minijazz et la
compile afin de la simuler. (pour plus d'informations voir le readme du
simulateur.)

* panda: compilateur d'un langage haut niveau dérivé du BASIC vers l'assembleur.
-> se compile avec ocamlbuild (non nécessaire pour la compilation de l'horloge)

* clock: contient le programme de l'horloge ainsi que le code de l'afficheur
digital à base de 7-segments.
-> Il faut au préalable compiler le processeur grâce à au script "build_proc.sh"
Important: l'afficheur utilise la bibliothèque SDL2 qui peut-être installée via
un gestionnaire de paquets.
(distributions Debian: sudo apt-get install libsdl2-dev)

Deux programmes sont disponibles:
 - clock_realtime.arm qui exécute l'horloge en temps réel synchronisé.
 - clock_quick.arm qui exécute l'horloge à la vitesse maximale possible.
Ils sont directement exécutables grâces aux scripts "execute.sh" et
"execute_quick.sh"

====
==== Pour compiler ses propres programmes ARM
====

La spécification du processeur a été modifiée pour avoir ses sorties dans
l'afficheur 7-segments. Cependant on peut modifier les paramètres de la
spécification Minijazz pour convenir à un usage plus général.

- Les valeurs en sortie texte peuvent être ajoutées comme des valeurs de sorties
standard en Minijazz et permettent d'afficher flags, registres..
- L'utilisation d'un 7-segment se fait en calculant une nappe de 7 bits et en
la spécifiant au simulateur. En effet le 3ème paramètre de compilateur_netlist
est la spécification de l'affichage, de la forme
"|variable1||variable2|\n|variable3|". Quand les modifications nécessaires sont
apportées à la spécification du processeur, on peut le compiler en exécutant
 -> cpp -P mjblocks/proco.mmj > clock/proc.mj
 -> minijazz/mjc.native clock/proc.mj
 -> simulateur_netlist/compilateur_netlist clock/proc.mj clock/proc.out "|x||y|"

Une fois le processeur compilé, il reste à lui donner un programme à exécuter:
- as/as.native est un programme qui convertis l'assembleur ARM en une série
d'instructions binaires. Attention: il faut utiliser le paramètre -b qui crée
le fichier dans le bon format pour le simulateur.
- une fois ce fichier obtenu, le placer dans le dossier du processeur.
- exécuter le tout en lançant ""./proc". 3 entrées sont ensuite demandées:
  -> le nom du programme compilé par l'assembleur
  -> le nom d'un fichier de ram (pour initialiser à la ram vide mettre "~")
  -> le nombre d'instructions à exécuter (-1 pour ne jamais s'arrêter)
