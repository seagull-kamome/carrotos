



+-----------------------------+
|      kernel                 |
+-----+-----------------------+
|     | Bord specific code    | 
|     +-----+-----------------+
| carrotlib | carrotarch      |
+-----------+-----------------+


kernel  ---> carrotlib
kernel  ---> board specific code
board specific code --> carrotlib
board specific code --> carrotarch

