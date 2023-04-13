print("Geisterspiel")
let du_bist_mutig = true;
let score = 0;

while(du_bist_mutig) {
	const geistertuer = randint(1, 3);
	print("Vor dir sind drei Tueren.")
	print("Hinter einer ist ein Geist.")
	print("Welche Tuer oeffnest du?")
	const tuer = input("1, 2 oder 3?");
	const tuer_nummer = int(tuer);
	if((tuer_nummer < geistertuer) | (tuer_nummer > geistertuer)) {
		print("Kein Geist!")
		print("Du bist ein Zimmer weiter.")
		score = score + 1
	} else {
		print("GEIST")
		du_bist_mutig = false
	}
}

print("Lauf weg!")
print("Spiel vorbei! Dein Score: " + score)