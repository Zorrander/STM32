# Voilier

Contrôle automatique de l'écoute d'un voilier

Eléments influants sur le bordage automatique de voile :
Girouette – STM32 - Servo-moteur d'écoute - batterie

Contrôle des voiles (foc et grand-voile)
A partir de l'angle du vent par rapport à la proue, a, (allure du bateau), imposer l'angle optimal des voiles, q.
On propose la loi suivante :
A l'intervalle a Î  [45° , 180°] correspond, suivant une loi affine, l'intervalle q Î  [0° , 90°], ceci
bien sûr pour les deux sens de rotations babord et tribord.
A l'intervalle a Î  [0° , 45°] on ferme totalement l'angle des voiles, q  = 0°.

Contrôle du plateau
L'utilisateur doit pouvoir faire tourner le bateau (babord, tribord) à partir de la télécommande de modélisme.
Il doit y avoir proportionnalité entre la position du manche de commande et la vitesse de virage.

Surveillance du roulis du bateau et de la batterie
Si un angle (entre la verticale et l'axe du mât) dépasse 45°, les voiles ne doivent plus être bordées. Un
message doit être envoyé via la liaison 433MHz.
Si la tension de batterie a perdu 20% de sa valeur initiale, un message doit être envoyé via la liaison
433MHz.
