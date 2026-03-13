# Cult Game

## Description
Jeu en C avec interface terminale utilisant ncurses.  
Le joueur gère une secte : recrute des membres, assigne des activités et augmente la légitimité.

---

## Prérequis

### Linux
Installer ncurses :

```bash
sudo apt install libncurses5-dev libncursesw5-dev  # Debian/Ubuntu
sudo dnf install ncurses-devel                      # Fedora
```

Conversion des fichiers en format Linux (LF)

Sous Windows, il est fréquent que les fichiers texte aient des retours chariot CRLF. Ce qui peux poser problèmes
Pour les convertir au format Linux (LF) :

Avec dos2unix (disponible dans MSYS2 ou Cygwin) :
```
dos2unix [nom du fichier]
```
