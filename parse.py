import argparse
import random


random.seed;
user = ""
for i in range (0, 10):
	user += random.choice("abcdefghijklmnopqrstuvwxyz0123456789")

print(user)


parser = argparse.ArgumentParser(description='Process some names')
parser.add_argument('--user', action='store_true', default=user, help='user flag')
parser.add_argument('string', action='store')

args = parser.parse_args()



if args.user:
	print("~ Your user name is: " + str(args.string))
else:
	print("~ You did not enter a user name after '--user'")



