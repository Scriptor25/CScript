# Tutorial #

fn intro() {
	sys.clear()
    sys.print("Welcome to this interactive tutorial for CScript!")
    sys.print("Here you will learn how to write programs with this language.")
    sys.print("(Note: when somewhere in these docs you find ', when you try it yourself use quotation marks instead!)")
	sys.print("(Note: this documentation is far from being finished, for examples or references take a look at the contents of this file or the other examples on https://github.com/Scriptor25/CScript)")
    sys.print("To get a list of commands you can use here, simply type 'help'.")
}

fn help() {
	sys.clear()
    sys.print("Here is a list of commands you can use here:")
    sys.print("help     -   list of commands")
    sys.print("var      -   explanation of variables, constants and values")
    sys.print("obj      -   how objects work")
    sys.print("func     -   an overview about functions")
	sys.print("builtin  -   a list of builtin functions and constants")
    sys.print("clear    -   clear the terminal")
    sys.print("exit     -   exit the program")
}

fn var() {
	sys.clear()
    sys.print("So variables work as follows:")
    sys.print("")
    sys.print("You can create variables by doing this:")
    sys.print("  let foo;")
    sys.print("")
    sys.print("You can also assign a value when declaring it:")
    sys.print("  let foo = bar;")
    sys.print("As you may already have noticed, when declaring a variable you MUST end the statement with a semicolon!")
    sys.print("")
    sys.print("Constants work almost the same, only you MUST assign a value on creation like so:")
    sys.print("  const x = 45;")
    sys.print("")
    sys.print("Changing the value of a variable works like this:")
    sys.print("  foo = 100")
    sys.print("")
    sys.print("Types of Values:")
    sys.print("(type)   -   (environment constants)")
    sys.print(" null     -   null")
    sys.print(" boolean  -   true, false")
    sys.print(" number   -   /")
    sys.print(" string   -   /")
    sys.print(" objects  -   /")
}

fn obj() {
	sys.clear()
    sys.print("Objects are a little bit complicated, but they work...")
    sys.print("")
    sys.print("Example:")
    sys.print("const x = { foo: 'foo', bar: 'bar', num: 13, complex: { yeet: null } };")
    sys.print("Here we defined a constant with the name x, the value is an object with the members: ")
    sys.print("'foo' of type string, ")
    sys.print("'bar' of type string, ")
    sys.print("'num' of type number and ")
    sys.print("'complex', which also is an object.")
    sys.print("")
    sys.print("But: when YOU have understood what an object does, what is the members name in object 'complex'?")
    
    while (sys.input("To go on, type the name here: ") != "yeet") {}
    
    sys.print("Well done!")
    sys.print("Now (I think so) you have understood the concept of objects.")
}

fn func() {
	sys.clear()
    sys.print("Here's an example of how to build a function:")
    sys.print("")
    sys.print("fn add_one(val) {")
    sys.print("  print(val = val + 1)")
    sys.print("}")
    sys.print("")
    sys.print("This function just adds one to the given value and then prints the value to the console.")
}

fn builtin() {
	sys.clear()
	sys.print("Builtin constants:")
	sys.print("true              - the true boolean value")
	sys.print("false             - the false boolean value")
	sys.print("null              - the null value")
	sys.print("")
	sys.print("Builtin functions (these are accessible by calling them on namespace sys, e.g.: 'sys.clear()'):")
	sys.print("print(any...)     - prints the given arguments out, beginning a new line each")
	sys.print("cprint(any...)    - clears the screen and then does the same as print(any...)")
	sys.print("time()            - returns the current time in milliseconds")
	sys.print("rand(min, max)    - returns a random double floating point number value between <min> and <max>")
	sys.print("randint(min, max) - returns a random integer value between <min> and <max>")
	sys.print("input(msg)        - prints out <msg>, waits for a user to enter something and returns the user provided input")
	sys.print("key(keyname)      - returns the state of the requested key as an object with the boolean fields 'down' and 'toggle'")
	sys.print("int(val)          - returns <val> as an integer value")
	sys.print("clear()           - clears the console output")
	sys.print("sleep(millis)     - pauses the program for <millis> milliseconds")
	sys.print("exit()            - aborts the program")
}

intro()

while (true) {
    
    const user = sys.input("[" + sys.time() + "]");
    
    if (user == "help") { help() } else {
    if (user == "var") { var() } else {
    if (user == "obj") { obj() } else {
    if (user == "func") { func() } else {
	if (user == "builtin") { builtin() } else {
    if (user == "clear") { sys.clear() } else {
    if (user == "exit") { sys.clear() sys.exit() } } } } } } }
}