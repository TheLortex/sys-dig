Pour compiler le programme :
-lancer make all


Pour executer le programme :
-Lancer la commande "./simulateur_netlist file.net output "<spec afficheur>"" pour compiler la netlist file.net, et obtenir le résultat sous forme de fichier c++ output.cpp et executable output

<spec afficheur> est la spécification de l'affichage 7-segment du simulateur.
Pour chaque nappe de 7 fils à afficher, on ajoute un champ "|variable|" dans la specification.
Cela donne par exemple "|v1||v2|\n|v3|".

Pour lancer le simulateur :
-Lancer la commande ./output (avec ouput le nom de l'executable créé comme précédemment)
-Suivre les indications données par la console.


Pour plus d'informations, se référer au rapport situer à la racine du dossier.
