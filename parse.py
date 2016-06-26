import argparse
import random


def parseArgs():

	random.seed;
	user = ""
	for i in range (0, 10):
		user += random.choice("abcdefghijklmnopqrstuvwxyz0123456789")


	parser = argparse.ArgumentParser(description='Parse_args_testing')


	parser.add_argument('-u', '--user', type=str, required=False, default=user, help='Your username.')
	parser.add_argument('-c', '--character', type=str, required=False, default="pacman", choices=["pac", "r", "o", "b", "p"], help='The character you want to play: pac(man), r(ed ghost), o(range ghost), b(lue ghost), p(ink ghost)')
	parser.add_argument('-p', '--port', type=int, required=True, help='Port of hatman server.')
	parser.add_argument('--host', type=str, required=False, default="127.0.0.1", help='IPv4 address of hatman server. Default is localhost.')
	
	args = parser.parse_args()


	print("Your user name is: " + args.user)
	print("You are playing: " + args.character)


	return args;