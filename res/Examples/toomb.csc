# Toomb - The Game #

#
(idx) | An array of symbols representing single ingame tiles
  0   | "  ": Empty
  1   | "[]": Wall segment
  2   | "? ": Skeleton
  3   | "* ": Firefly
  4   | "& ": Player
  5   | "! ": Weapon drop
  6   | "$ ": Money drop
  7   | "% ": Loot Chest
  8   | "= ": Barrier
  9   | "+ ": Heal drop
  10  | "\ ": Exit
#
const TABLE = ["  ", "[]", "? ", "* ", "& ", "! ", "$ ", "% ", "= ", "+ ", "# "];

const SCREEN = {
	WIDTH:  24,
	HEIGHT: 18,
};

const CAMERA = {
	X:  1,
	Y:  1,
	Z:  0.65,
};

const WORLD = [];

fn generate (x, y, width, height) {
	
	const chunk = {
		x: 0, 
		y: 0, 
		width: 0, 
		height: 0, 
		content: []
	};
	
	chunk.x = x
	chunk.y = y
	chunk.width = width
	chunk.height = height
	
	let r = 0;
	while (r < height) {
		let c = 0;
		while (c < width) {
			const idx = c + r * width;
			
			let sym = 0;
			
			if (r == 0 | r == height - 1 | c == 0 | c == width - 1) {
				sym = 1
				if (((r == 0 | r == height - 1) & c == sys.int(width / 2)) | ((c == 0 | c == width - 1) & r == sys.int(height / 2))) { sym = 10 }
			}
			
			chunk.content[idx] = sym
			
			c = c + 1
		}
		r = r + 1
	}
	
	WORLD[WORLD.length] = chunk
}

fn draw () {
	const screen = [];
	
	let r = 0;
	while (r < SCREEN.HEIGHT) {
		screen[r] = ""
		
		const v = r / SCREEN.HEIGHT;
		
		let c = 0;
		while (c < SCREEN.WIDTH) {
			const u = c / SCREEN.WIDTH;
			
			let x = c;
			let y = r;
			
			x = x - CAMERA.X
			y = y - CAMERA.Y
			
			x = x / CAMERA.Z
			y = y / CAMERA.Z
			
			x = sys.int(x)
			y = sys.int(y)
			
			let sym = "  ";
			let idx = 0;
			while (idx < WORLD.length) {
				const chunk = WORLD[idx];
				
				if (
				x >= chunk.x &
				x < chunk.x + chunk.width &
				y >= chunk.y &
				y < chunk.y + chunk.height) { # We're inside the chunk #
					sym = TABLE[chunk.content[(x - chunk.x) + (y - chunk.y) * chunk.width]]
					
					idx = WORLD.length
				}
				
				idx = idx + 1
			}
			
			screen[r] = screen[r] + sym
			c = c + 1
		}
		
		r = r + 1
	}
	
	sys.cprint(screen)
}

fn move () {
	const state_w = sys.key("w");
	const state_s = sys.key("s");
	const state_d = sys.key("d");
	const state_a = sys.key("a");
	const state_e = sys.key("e");
	const state_q = sys.key("q");
	
	if (state_w.down) { CAMERA.Y = CAMERA.Y + 1 }
	if (state_s.down) { CAMERA.Y = CAMERA.Y - 1 }
	if (state_d.down) { CAMERA.X = CAMERA.X - 1 }
	if (state_a.down) { CAMERA.X = CAMERA.X + 1 }
	if (state_e.down) { CAMERA.Z = CAMERA.Z * 1.1 }
	if (state_q.down) { CAMERA.Z = CAMERA.Z / 1.1 }
}

fn loop () {
	move()
	draw()
}

generate( 0,  0, 24, 24)
generate(24,  0, 24, 24)
generate( 0, 24, 24, 24)
generate(24, 24, 24, 24)

while (true) {
	loop()
}