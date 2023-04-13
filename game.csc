# Field Size #
const WIDTH  = 21;
const HEIGHT = 21;

# Player Position #
let x = int(WIDTH  / 2);
let y = int(HEIGHT / 2);

# Apple Position #
let apple_x = randint(0, WIDTH  - 1);
let apple_y = randint(0, HEIGHT - 1);

# Score #
let score = 0;

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
			
			print("- Score: " + score + " " + make_bar(WIDTH * 3 - 10 - score_length + 1))
		} else {
			print(make_bar(WIDTH * 3 + 2))
		}
		f = f + 1
	}
	
	# Iterate through the field and        #
	# if at player position, draw player,  #
	# if at apple position, draw apple and #
	# else draw the field                  #
	let r = 0;
	while (r < HEIGHT) {
		let field = "";
		let c = 0;
		while (c < WIDTH) {
			if (c == x & r == y) { field = field + " * " } else {
			if (c == apple_x & r == apple_y) { field = field + " 0 " }
			else { field = field + "   " } }
			
			c = c + 1
		}
		print("-" + field + "-")
		r = r + 1
	}
	
	print(make_bar(WIDTH * 3 + 2))
}

# Uses the user input to move the player #
fn user_input () {
	let user = input("");
	
	# Multiple characters account for faster input #
	if (user == "w" | user == "ww" | user == "www") { y = y - 1 } else { # Move up    #
	if (user == "s" | user == "ss" | user == "sss") { y = y + 1 } else { # Move down  #
	if (user == "a" | user == "aa" | user == "aaa") { x = x - 1 } else { # Move left  #
	if (user == "d" | user == "dd" | user == "ddd") { x = x + 1 } } } }  # Move right #
}

fn check_player () {
	if (x < 0) { x = 0 } else {
	if (x > WIDTH - 1) { x = WIDTH - 1 } }
	if (y < 0) { y = 0 } else {
	if (y > HEIGHT - 1) { y = HEIGHT - 1 } }
}

# Check's if the player touches the apple #
# If it does, add 1 to the score and      #
# place the apple anywhere on the field   #
fn check_apple () {
	if (x == apple_x & y == apple_y) {
		while (x == apple_x & y == apple_y) {
			apple_x = randint(0, WIDTH  - 1)
			apple_y = randint(0, HEIGHT - 1)
		}
		score = score + 1
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
}
