import os

path = os.path.join(os.getcwd(), "game.sql")
os.system("mysqldump -u root -padmin -d -R game > " + path)