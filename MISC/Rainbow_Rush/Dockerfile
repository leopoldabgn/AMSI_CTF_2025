FROM debian:bookworm
RUN echo 'deb http://deb.debian.org/debian bookworm-backports main' > /etc/apt/sources.list.d/backports.list

# Configuration du timezone
ENV TIMEZONE=Europe/Paris

# le rm-rf permet de rendre plus léger le container
# RUN apt-get update && apt-get install -y ncat gcc python3 && ln -s /usr/bin/python3 /bin/python && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y socat python3 && ln -s /usr/bin/python3 /bin/python && rm -rf /var/lib/apt/lists/*

# Copie du code source
COPY ./ /app
WORKDIR /app

# Installation des dépendances Python si requirements.txt existe
RUN if [ -f "requirements.txt" ]; then pip3 install -r requirements.txt --break-system-packages; fi

# Création d'un utilisateur non-privilégié pour la sécurité
RUN useradd --home=/rainbow -U rainbow

# Attribution des permissions
RUN chown -R rainbow:rainbow /app
RUN chmod 755 /app/rainbow_rush.py

# Changement vers l'utilisateur non-privilégié
USER rainbow

# Exposition du port
EXPOSE 4001

# Commande pour lancer le service avec netcat
# CMD ["sh", "-c", "ncat -e /app/rainbow_rush.py -l 4007 -k"]

CMD socat TCP-LISTEN:4001,reuseaddr,fork EXEC:"python3 /app/rainbow_rush.py",pty,stderr
