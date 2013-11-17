import requests

s = requests.session()

loginurl = "http://localhost/view/games/login.php?go=login"
loginval = {"username":"test1", "password":"123456"}
s.post(loginurl, loginval)

code_src = open("code.cpp","r").read()

# submit code
suburl = "http://localhost/view/games/submit.php?go=submit"
subval = {"pid":"1000", "language":"0", "code":code_src}

for x in xrange(100):
	s.post(suburl, subval)