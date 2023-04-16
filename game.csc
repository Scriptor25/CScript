# Field Size #
const FIELD = {
    WIDTH: 15,
    HEIGHT: 15,
};

# Player Position #
const Player = {
    x: sys.int(FIELD.WIDTH  / 2),
    y: sys.int(FIELD.HEIGHT / 2),
};

# Apple Position #
const Apple = {
    x: sys.randint(0, FIELD.WIDTH  - 1),
    y: sys.randint(0, FIELD.HEIGHT - 1),
};

# Score #
let score = 0;

# FPS counter #
let fps = 0;

# Countdown #
let countdown = 10;
let prev_time = sys.time();

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
	let screen = [];
	let idx = 0;
	
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
			
			let fps_length = 9;
			if (fps < 10)  { fps_length = 7 } else {
			if (fps < 100) { fps_length = 8 } }
            
            const ws = FIELD.WIDTH * 3 -  9 -     score_length;
            const wc = FIELD.WIDTH * 3 - 13 - countdown_length;
            const wf = FIELD.WIDTH * 3 -  7 -       fps_length;
			
			screen[idx] = "- Score: "     + score     + " " + make_bar(ws)
			idx = idx + 1
            screen[idx] = "- Countdown: " + countdown + " " + make_bar(wc)
			idx = idx + 1
            screen[idx] = "- FPS: "       + fps       + " " + make_bar(wf)
		} else {
			screen[idx] = make_bar(FIELD.WIDTH * 3 + 2)
		}
		
		f = f + 1
		idx = idx + 1
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
		screen[idx] = "-" + field + "-"
		
		r = r + 1
		idx = idx + 1
	}
	
	screen[idx] = make_bar(FIELD.WIDTH * 3 + 2)
	
	# Redraw the console #
	# clear() #
	sys.cprint(screen)
}

# Uses the user input to move the player #
fn user_input () {
	const state_w = sys.key("w");
	const state_s = sys.key("s");
	const state_a = sys.key("a");
	const state_d = sys.key("d");
	
	# Multiple characters account for faster input #
	if (state_w.down) { Player.y = Player.y - 1 } # Move up    #
	if (state_s.down) { Player.y = Player.y + 1 } # Move down  #
	if (state_a.down) { Player.x = Player.x - 1 } # Move left  #
	if (state_d.down) { Player.x = Player.x + 1 } # Move right #
}

fn check_player () {
	if (Player.x < 0) { Player.x = 0 } else {
	if (Player.x > FIELD.WIDTH - 1) { Player.x = FIELD.WIDTH - 1 } }
	if (Player.y < 0) { Player.y = 0 } else {
	if (Player.y > FIELD.HEIGHT - 1) { Player.y = FIELD.HEIGHT - 1 } }
}

# Check's if the player touches the apple #
# If it does, add 1 to the score and      #
# place the apple anywhere on the field   #
fn check_apple () {
	if (Player.x == Apple.x & Player.y == Apple.y) {
		while (Player.x == Apple.x & Player.y == Apple.y) {
			Apple.x = sys.randint(0, FIELD.WIDTH  - 1)
			Apple.y = sys.randint(0, FIELD.HEIGHT - 1)
		}
		score = score + 1
        countdown = 10
	}
}

fn reset() {
	Player.x = sys.int(FIELD.WIDTH  / 2)
	Player.y = sys.int(FIELD.HEIGHT / 2)
	
	Apple.x = sys.randint(0, FIELD.WIDTH  - 1)
    Apple.y = sys.randint(0, FIELD.HEIGHT - 1)
	
	score = 0
	
	countdown = 10
	prev_time = sys.time()
}

fn update_time () {
	const now = sys.time();
    const delta = now - prev_time;
    prev_time = now
	
	fps = 1 / (delta / 1000)
	
    countdown = countdown - (delta / 1000)
	
	if (countdown <= 0) {
		sys.clear()
		sys.print("Time's over! Your Score: " + score)
		sys.input("Press enter to try again")
		reset()
	}
	
	# Limit the fps to 10 so the player doesnt go brrrrt #
	
	const sleep_time = 100 - delta;
	
	if (sleep_time > 0) { sys.sleep(sleep_time) }
}

# The main game loop       #
# draw the game            #
# wait for input           #
# check for collisions     #
# update fps and countdown #
while (true) {
	draw()
	user_input()
	check_player()
	check_apple()
    update_time()
}
