# Field Size #
const FIELD = {
    WIDTH: 21,
    HEIGHT: 21,
};

# Player Position #
const Player = {
    x: int(FIELD.WIDTH  / 2),
    y: int(FIELD.HEIGHT / 2),
};

# Apple Position #
const Apple = {
    x: randint(0, FIELD.WIDTH  - 1),
    y: randint(0, FIELD.HEIGHT - 1),
};

# Score #
let score = 0;

# Countdown #
let countdown = 10;
let prev_time = time();

fn make_bar (width) {
	let n = 0;
	let bar = "";
	while (n < width) {
		bar = bar + "-"
		n = n + 1
	}
	
	bar
}

# Draws the game world #
fn draw () {
	# Clear the console #
	clear()
	
	# Print the score #
	let f = 0;
	while (f < 3) {
		if (f == 1) {
			let score_length = 9;
			if (score < 10)  { score_length = 7 } else {
			if (score < 100) { score_length = 8 } }
            
            let countdown_length = 9;
			if (countdown < 10)  { countdown_length = 7 } else {
			if (countdown < 100) { countdown_length = 8 } }
            
            const ws = FIELD.WIDTH * 3 -  9 - score_length;
            const wc = FIELD.WIDTH * 3 - 13 - countdown_length;
			
			print("- Score: "     + score     + " " + make_bar(ws))
            print("- Countdown: " + countdown + " " + make_bar(wc))
		} else {
			print(make_bar(FIELD.WIDTH * 3 + 2))
		}
		f = f + 1
	}
	
	# Iterate through the field and        #
	# if at player position, draw player,  #
	# if at apple position, draw apple and #
	# else draw the field                  #
	let r = 0;
	while (r < FIELD.HEIGHT) {
		let field = "";
		let c = 0;
		while (c < FIELD.WIDTH) {
			if (c == Player.x & r == Player.y) { field = field + " * " } else {
			if (c == Apple.x & r == Apple.y) { field = field + " 0 " }
			else { field = field + "   " } }
			
			c = c + 1
		}
		print("-" + field + "-")
		r = r + 1
	}
	
	print(make_bar(FIELD.WIDTH * 3 + 2))
}

# Uses the user input to move the player #
fn user_input () {
	let user = input("");
	
	# Multiple characters account for faster input #
	if (user == "w" | user == "ww" | user == "www") { Player.y = Player.y - 1 } else { # Move up    #
	if (user == "s" | user == "ss" | user == "sss") { Player.y = Player.y + 1 } else { # Move down  #
	if (user == "a" | user == "aa" | user == "aaa") { Player.x = Player.x - 1 } else { # Move left  #
	if (user == "d" | user == "dd" | user == "ddd") { Player.x = Player.x + 1 } } } }  # Move right #
}

fn check_player () {
	if (Player.x < 0) { Player.x = 0 } else {
	if (Player.x > FIELD.WIDTH - 1) { Player.x = FIELD.WIDTH - 1 } }
	if (Player.y < 0) { y = 0 } else {
	if (Player.y > FIELD.HEIGHT - 1) { Player.y = FIELD.HEIGHT - 1 } }
}

# Check's if the player touches the apple #
# If it does, add 1 to the score and      #
# place the apple anywhere on the field   #
fn check_apple () {
	if (Player.x == Apple.x & Player.y == Apple.y) {
		while (Player.x == Apple.x & Player.y == Apple.y) {
			Apple.x = randint(0, FIELD.WIDTH  - 1)
			Apple.y = randint(0, FIELD.HEIGHT - 1)
		}
		score = score + 1
        countdown = 10
	}
}

fn reset() {
	Player.x = int(FIELD.WIDTH  / 2)
	Player.y = int(FIELD.HEIGHT / 2)
	
	Apple.x = randint(0, FIELD.WIDTH  - 1)
    Apple.y = randint(0, FIELD.HEIGHT - 1)
	
	score = 0
	
	countdown = 10
	prev_time = time()
}

fn advance_countdown () {
    const delta = time() - prev_time;
    prev_time = time()
    countdown = countdown - delta / 1000
	
	if (countdown <= 0) {
		clear()
		print("Time's over! Your Score: " + score)
		input("Press enter to try again")
		reset()
	}
}

# The main game loop   #
# draw the game        #
# wait for input       #
# check for collisions #
while (true) {
	draw()
	user_input()
	check_player()
	check_apple()
    advance_countdown()
}
