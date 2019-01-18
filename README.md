# Bomberman_extended
ETNA bomberman Extended

Pour que le jeu puisse se lancer installer:<br>

```sudo apt-get install libsdl2-image-2.0-0```<br>
```sudo apt-get install libsdl2-image-dev```<br>
```sudo apt-get install git curl libsdl2-mixer-2.0-0 libsdl2-image-2.0-0 libsdl2-2.0-0```<br>

Lancement du server :<br>
./bomberman server [port]<br>

Lancement du client :<br>
./bomberman client [adresse ip] [port]<br>

Topo du call<br>
-Modifier le server pour qu'il soit en réseau (actuellement on est toujours en 127.0.0.1)<br>
-Faire les audits avant/après pour les optimisations (RAM pour nous, et donc les photos postées plus haut)<br>
-Portabilité windows & linux (Nada est sur windows, faut se mettre aussi sur le .deb ou .rpm) Pas de cygwin pour windows<br>
-Launchers windows & linux<br>


<strong>Pragma Pack</strong>
Permet de réduire la taille des structures. Lire cette réponse pour mieux comprendre : https://stackoverflow.com/a/3318475/10760673<br>
