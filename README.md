# TP – Analyse de logs réseau

## Description

Ce projet consiste à développer un mini outil d’analyse de logs réseau simulant le travail d’un centre de supervision (NOC).

Le programme lit un fichier `network_log.txt` contenant des traces réseau au format :

DATE;HEURE;IP_SOURCE;PORT;PROTOCOLE;STATUT

Exemple :
2026-02-10;08:45:12;192.168.1.10;22;TCP;ECHEC

Le champ STATUT peut être :
- SUCCES
- ECHEC

Deux versions du programme sont fournies :
- Version C
- Version Python

Les deux versions produisent les mêmes résultats et génèrent un fichier 'rapport_analyse.txt'.

Exemple:

===== RESUME DES STATISTIQUES =====
Total connexions : 84
Succes : 59
Echecs : 25
Port le plus utilise : 22
IP la plus active : 192.168.1.10

===== LA LISTE DES IPs SUSPECTES =====
IP suspecte : 192.168.1.10 (Port 22)
IP suspecte : 192.168.1.20 (Port 21)
IP suspecte : 192.168.1.40 (Port 22)

---
# Pour la compilation du programme en "Python"

python3 analyse_logs.py

# Pour la compilation du programme en "C"

Dans le dossier du projet, exécuter :

```bash
gcc analyse_logs.c -o analyse_logs