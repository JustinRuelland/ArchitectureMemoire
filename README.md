# ArchitectureMemoire
Explications et détails sur le fonctionnement et l’utilité du projet pour de potentiels utilisateurs :
I. Comment compiler et exécuter le programme :
L’intérêt de ce projet est de simuler un ordinateur composé d’une mémoire dont la capacité de stockage est de 64 kilo-octets, c’est-à-dire de 65536 octets, ainsi que d’un micro-processeur.
Afin d’exécuter sans problème le programme, vous aurez besoin au préalable d’écrire sur un fichier texte correspondant à un algorithme en langage assembleur.
Il faut compiler le programme principal afin de pouvoir l’exécuter. Pour cela, vous devez extraire le dossier RuellandMalak.zip, puis ouvrir le dossier. Ensuite, il vous faut ouvrir votre terminal, accéder à l’adresse où se trouve le contenu du fichier extrait, puis écrire dans la ligne de commande :
➔ gcc -Wall *.c -o nom_executable -lm
Où nom_executable est le nom que vous donnerez à votre fichier exécutable. Attention : il est important de savoir que ‘-lm’ est à mettre obligatoirement si vous travaillez sur un noyau Linux hors macOS du fait de l’importation de la bibliothèque <math.h> et de l’utilisation de la fonction ‘pow()’.
Ensuite, vous pouvez exécuter le programme en mettant sur la ligne de commande de votre terminal :
→./nom_executable pgm.txt
Où pgm.txt est le fichier texte contenant votre programme en langage assembleur (faites attention à respecter l’extension .txt). Veuillez ne pas nommer ce fichier fichierTemp.txt, hexa.txt, ni binaire.txt, tous trois crées et utilisés par et dans le programme
Vous avez maintenant affiché à votre écran le résultat de votre programme. En cas d’affichage d’une erreur dans votre programme, veuillez-vous référer à la seconde section de ce formulaire portant sur la syntaxe à respecter.
II. La syntaxe à respecter pour le fichier d’entrée :
Comme énoncé précédemment, il vous faut écrire l’algorithme que vous voulez exécuter, avec l’aide de notre programme, en langage assembleur. Pour cela, les instructions suivantes vous sont disponibles : (pour plus d’information, voir le sujet du projet)
➢ add rd, rn, S ➢ sub rd, rn, S
➢ mul rd, rn, S
➢ div rd, rn, S
➢ or rd, rn, S
➢ xor rd, rn, S
➢ and rd, rn, S
➢ shl rd, rn, S
➢ ldb rd, (rn)S
➢ ldw rd, (rn)S
➢ stb (rd)S, rn
➢ stw (rd)S, rn
➢ jmp S→Retourne à l’adresse S
➢ jzs S→Retourne à l’adresse de S si le résultat de l’instruction précédente
est 0
➢ jzc S→Retourne à l’adresse de S si le résultat de l’instruction précédente
n’est pas 0
➢ jcs S→Retourne à l’adresse de S si le résultat de l’instruction précédente a
créé une retenue
➢ jcc S→Retourne à l’adresse de S si le résultat de l’instruction précédente
n’a pas créé une retenue
➢ jns S → Retourne à l’adresse de S si le bit de poids fort de l’instruction
précédente est à 1
➢ jnc S → Retourne à l’adresse de S si le bit de poids fort de l’instruction
précédente est à 0
➢ in rd→Demande à l’utilisateur de saisir une valeur au clavier et la met dans
rd
➢ out rd→Affiche la valeur de rd
➢ rnd rd, rn, S → Met dans rd un nombre aléatoire entre la valeur de rn et S
➢ hlt→Arrête le programme
Où S correspond soit à un registre général, soit à une valeur immédiate (la valeur est alors précédée de ‘#’.
Il faut cependant respecter une certaine syntaxe dans ce fichier d’entrée à la fois vis- à-vis du format, mais aussi des valeurs des instructions :
✓ Pas de saut ligne autorisé, excepté en fin de code où un seul saut est autorisé
✓ Les étiquettes dans votre code ne doivent pas dépasser 45 caractères
✓ Lorsque vous appelez des instructions de sauts, tout ce qui est écrit après l’adresse sera ignoré, il n’y aura pas d’erreur renvoyée pour des raisons de sécurités de saut. N’importe quel autre ajout d’un élément nuisible à
l’intégrité du code renverra une erreur
✓ Les valeurs immédiates peuvent être saisies en hexadécimale, la syntaxe
sera alors ‘#hS’, où S sera la valeur en hexadécimal

✓ Des valeurs supérieures à 32767 où inférieures à -32768 seront tronquées à ces mêmes valeurs
✓ Si le programme se termine en affichant « Segmentation Fault », cela implique que l’utilisateur a voulu accéder à une case mémoire non autorisée, ou bien que cette case mémoire est trop grande, ou encore que celle-ci n’est pas disponible
✓ L’adresse de l’instruction « stw » ne doit être ni la dernière case mémoire, soit 32767, ni la première, soit -32768
✓ Les instructions « stb » et « stw » ne sont pas disponibles pour les cases mémoires où sont stockées les instructions de votre fichier, c’est-à-dire, les ‘nombres de lignes de votre code’ x 4 + 1 premières cases mémoires
✓ Si votre code ne comporte pas une ligne d’instruction « hlt », un « segmentation fault » vous sera retourné
III. Analyse des résultats :
Si après l’exécution de votre programme, un « segmentation fault » vous est affiché, cela veut dire que votre code est erroné. S’il affiche une erreur de syntaxe, celle-ci est accompagnée de la ligne où se situe l’erreur, et vous aide donc à revoir et corriger votre programme.
Sinon, l’affichage obtenue est celui correspondant au résultat de votre algorithme.
Si cependant vous n’avez aucune information à l’écran, cela implique que vous avez omis de mettre des instructions « out » permettant d’afficher les valeurs des registres en question.
