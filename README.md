Merci de créer des branches qui évoquent ce sur quoi vous travaillez qu'on ne se perde pas dans les fonctionnalités.<br><br>

Pour que le jeu puisse se lancer installer:<br>

```sudo apt-get install libsdl2-image-2.0-0```<br>
```sudo apt-get install libsdl2-image-dev```<br>
```sudo apt-get install git curl libsdl2-mixer-2.0-0 libsdl2-image-2.0-0 libsdl2-2.0-0```<br>

Lancement du server :<br>
./bomberman server [port]<br>

Lancement du client :<br>
./bomberman client [adresse ip] [port]<br>

Extended quoi faire :<br>
-Changer la map<br>
-Vérifier que la connexion depuis différentes machines fonctionnent<br>
-Optimiser le code (propre, enlever le code inutile, rajouter des commentaires pour les fonctions)<br>
-Vérifier la connexion à 4 joueurs (et si le problème vient de la ram ou non)<br>
-Optimisation de la ram<br>
-Créer des launches .exe .deb<br>
-Lancement sous windows<br>
-Système de MAJ avec des nouveaux paquets<br>
<br><br>
<strong>Pragma Pack</strong>
Permet de réduire la taille des structures. Lire cette réponse pour mieux comprendre : https://stackoverflow.com/a/3318475/10760673<br>
<strong>Union</strong>
Permet à des structures d'avoir plusieurs types de données (int, float, char...) Et de les conserver dans le même endroit de la mémoire. https://www.tutorialspoint.com/cprogramming/c_unions.htm
