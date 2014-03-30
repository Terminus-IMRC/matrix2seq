#!/usr/bin/env python3
while True:
	try:
		s=input()
	except EOFError:
		break
	
	if s[0]!='-':
		print(s, end=' ')
	else:
		print()
