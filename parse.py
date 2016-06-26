import argparse
import random


random.seed;
user = ""
for i in range (0, 10):
	user += random.choice("abcdefghijklmnopqrstuvwxyz0123456789")

print(user)


parser = argparse.ArgumentParser(description='Parse_args_testing')


parser.add_argument('-u', '--user', type=str, required=False, default=user, help='Your username.')
parser.add_argument('-c', '--character', type=str, required=False, default="pacman", choices=["pac", "r", "o", "b", "p"], help='The character you want to play: pac(man), r(ed ghost), o(range ghost), b(lue ghost), p(ink ghost)')


args = parser.parse_args()


print("Your user name is: " + args.user)
print("You are playing: " + args.character)



