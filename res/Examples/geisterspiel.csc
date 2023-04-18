sys.clear()
sys.print("Geisterspiel")
let du_bist_mutig = true;
let score = 0;

while(du_bist_mutig) {
	const geistertuer = sys.randint(1, 3);
	sys.print("Vor dir sind drei Tueren.")
	sys.print("Hinter einer ist ein Geist.")
	sys.print("Welche Tuer oeffnest du?")
	const tuer = sys.input("1, 2 oder 3?");
	const tuer_nummer = sys.int(tuer);
	if(tuer_nummer == geistertuer) {
		sys.clear()
		sys.print("GEIST")
		du_bist_mutig = false
	} else {
		sys.clear()
		sys.print("Kein Geist!")
		sys.print("Du bist ein Zimmer weiter.")
		score = score + 1
	}
}

sys.print("Lauf weg!")
sys.print("Spiel vorbei! Dein Score: " + score)
