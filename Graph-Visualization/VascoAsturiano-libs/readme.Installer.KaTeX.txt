Voici quelques URLs fixes à utiliser pour télécharger les fichiers et les sauvegarder localement :

katex.min.js :
https://cdn.jsdelivr.net/npm/katex@0.16.25/dist/katex.min.js 
GitHub
+1

katex.min.css :
https://cdn.jsdelivr.net/npm/katex@0.16.25/dist/katex.min.css 
katex.org
+1

Extension auto‑render:
https://cdn.jsdelivr.net/npm/katex@0.16.25/dist/contrib/auto-render.min.js 
GitHub
+1

Dossier des polices :
Tous les fichiers dans https://cdn.jsdelivr.net/npm/katex@0.16.25/dist/fonts/ — on peut les télécharger:

==================================================================================
cd ~/VascoAsturiano-libs/KaTeX/fonts

wget -r -np -nH --cut-dirs=4 -R "index.html*" \
     https://cdn.jsdelivr.net/npm/katex@0.16.25/dist/fonts/