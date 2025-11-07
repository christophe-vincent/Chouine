# Chouine

Jeu de carte se jouant à 2 joueurs qui est probablement issu de la Brisque, Mariage. 

L'algorithme est développé en C++ et la partie graphique grâce à Godot.


# TODO list
- Menu principal
- Algo: Prise de la carte d'atout pour completer une annonce
- Bouton pour selectionner une annonce

## bugs connus


# Algorithme

Il est popssible de tester l'algorithme grace à une app de test: chouinetest

Compilation et éxécution (sous Linux):
> mkdir build && cd build \
> cmake .. \
> make \
> ./chouinetest


# Déploiement

## Compilation de l'algorithme

L'algorithme est programmé en c++ et nécessite l'extention Godot c++.
Voir https://docs.godotengine.org/en/4.4/tutorials/scripting/gdextension/gdextension_cpp_example.html

Il sera donc nécessaire de compiler l'algorithme séparemment.

Pour cela créer un virtualenv python et installer 'scons'.

Compilation pour **html5**:
> scons platform=web target=template_release
Les fichiers sont automatiquement copiés dans le répertoire bin de godot.

Compilation pour **Windows** (pour lancer le jeu avec Godot):
> scons platform=windows

## Exporter vers HTML5

### Compilation du GDExtention pour le web
Suivre les instructions de la section **GDExtension**:
https://docs.godotengine.org/en/latest/engine_details/development/compiling/compiling_for_web.html#doc-compiling-for-web

Le fichier compilé (web_dlink_release.zip) est ensuite copié ici:
C:\Users\<USER>\AppData\Roaming\Godot\export_templates\4.5.stable

### Mise en ligne
La copie vers un bucket S3 et mise à jour du cache CloudFront s'effectue grace à ce script:

> ./deploiement/upload_site_web.py

ANDROID_HOME=/mnt/c/Users/cvincent/AppData/Local/Android/Sdk