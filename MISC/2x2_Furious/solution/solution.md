# 2x2 Furious

## Récupérer le mélange du cube (pwntools)
Vous pouvez utiliser la librairie `pwntools` en python pour parser l'output renvoyé par le challenge et récupérer le melange du cube (la couleur de chaque face).

## Résoudre le cube (programme externe ou votre propre code)
Dans un deuxième temps, il faut résoudre le cube en utilisant un solveur. Il existe une grande quantité de solveur de rubiks cube 2x2 sur github. Vous pouvez également faire votre propre code, mais faire ça durant le CTF prend trop de temps.
 
L'idée est donc d'envoyer le mélange du cube à un programme externe et de récupérer l'enchainement de mouvements à effectuer pour le résoudre.

## Envoyer les mouvements pour résoudre le cube (pwntools)
A l'aide `pwntools`, vous devez renvoyer l'enchainement des mouvements pour résoudre votre cube. Le flag s'affiche ensuite à l'écran.