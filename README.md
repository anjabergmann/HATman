# HATman
_Projektarbeit 1 - Gamedevelopment with Python_


#Team
	* Tabea Halmschlager
	* Johanna Kirchmaier
	* Anja Bergmann


#Dependencies: 
	* Python 3.4 
	* Cocos2D (can be installed via 'pip install cocos2d')
	* Pyglet 1.2.4 ('pip install pyglet')
	* [numPy1.11.0 - not yet, but possibly later ('pip install numpy')]

	* Other versions might work too, but we don't guarantee ;)
	* There can be problems with the sprites on Windows 7.


#To-Do
	* Pacman eats dots
	* Ghosts (similar to Pacman)
	* Pacman-can-hunt-ghosts-mode (after pacman ate one of these bigger dots)
	* Collision detection between Pacman and Ghosts
	* Create labyrinth randomly
	* Multiplayer
	* Start-Screen
	* Score
	* Highscore-Screen
	* Database
	* Pacman-lifes
	* Hatman
		--> Special version that overall works similar to the normal version but with fancier design: 
			- Pacman wears hat
			- Labyrinth Tim-Burton-like
			- Four different ghosts with different abilities
				* Weeping Angel --> Faster than the other ghosts, but can't move if in front of another character
				* Fox --> sneaks through the lab and can only be seen (by pacman) a) every x seconds or b) when it is near pacman?
				* A ghost-ghost --> can walk through walls and doesn't care about the labrinth at all ???
				* ???
	* All the graphical stuff (draw sprites and a fancy designed labyrinth [only peaces so that they can be used to make a random created labyrinth] and so on)
	* Music (for Hatman-Version perhaps: Moonlight Sonata)
	* Mobile ...
	* Login

#Q>A
	* Do ghosts work togehter and have a common score? --> Yes.
	* What do the ghosts see? 
		--> Only a little snippet of the labyrinth (rectangular area around them)?
		--> Whole labyrinth but without Pacman? 
		--> Other ghosts? 
		--> Some sort of bonus item they can find in the labyrinth to make pacman visible for two seconds or something? (Like the bigger dots the pacman can eat to hunt the ghosts)
		--> Hatman-Ghosts different? --> Special abilities? 
	* Multiplayermode: 
		--> Can you see all open games looking for more players and join one (possibly not so good if there are a lot players playing --> they have to be very fast to join a game and we need to make sure that there are not too many players in one game accidentially) or do you just choose what character you'd like to play and then get assigned to a game that is in need of that character?
		--> Friends? 
		--> If you don't find enough players for a game within a minute (or bla) do you play against the computer? 
			- Problem: Complicated, because we would have to implement an algorithm to move the characters somehow reasonable through the lab
			- Problem2: If our algorithm is not good, it is easier to reach a higher score
	* How to get a programme mobile and onto your phone without using the play store???
	* How to connect to a database using python? 
	* How does our database look like? 
		--> Player (pk_name, pw#, loggedIn(bool))
		--> Highscore (pk_rank, fk_player, score, character)
		--> activeGames (pk_id, fk_player_pacman, fk_player_ghost1, fk_player_ghost2, fk_player_ghost3, fk_player_ghost4) --> If null --> No player chosen yet
		--> ??? 