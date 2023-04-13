# Tutorial #

fn intro() {
    print("Welcome to this interactive tutorial for CScript!")
    print("Here you will learn how to write programs with this language.")
    print("(Note: when somewhere in these docs you find ', when you try it yourself use quotation marks instead!)")
    print("To get a list of commands you can use here, simply type 'help'.")
}

fn help() {
    print("Here is a list of commands you can use here:")
    print("help     -   list of commands")
    print("var      -   explanation of variables, constants and values")
    print("obj      -   how objects work")
    print("func     -   an overview about functions")
    print("clear    -   clear the terminal")
    print("exit     -   exit the program")
}

fn var() {
    print("So variables work as follows:")
    print("")
    print("You can create variables by doing this:")
    print("  let foo;")
    print("")
    print("You can also assign a value when declaring it:")
    print("  let foo = bar;")
    print("As you may already have noticed, when declaring a variable you MUST end the statement with a semicolon!")
    print("")
    print("Constants work almost the same, only you MUST assign a value on creation like so:")
    print("  const x = 45;")
    print("")
    print("Changing the value of a variable works like this:")
    print("  foo = 100")
    print("")
    print("Types of Values:")
    print("(type)   -   (environment constants)")
    print(" null     -   null")
    print(" boolean  -   true, false")
    print(" number   -   /")
    print(" string   -   /")
    print(" objects  -   /")
}

fn obj() {
    print("Objects are a little bit complicated, but they work...")
    print("")
    print("Example:")
    print("const x = { foo: 'foo', bar: 'bar', num: 13, complex: { yeet: null } };")
    print("Here we defined a constant with the name x, the value is an object with the members: ")
    print("'foo' of type string, ")
    print("'bar' of type string, ")
    print("'num' of type number and ")
    print("'complex', which also is an object.")
    print("")
    print("But: when YOU have understood what an object does, what is the members name in object 'complex'?")
    
    while (input("To go on, type the name here: ") != "yeet") {}
    
    print("Well done!")
    print("Now (I think so) you have understood the concept of objects.")
}

fn func() {
    print("Here's an example of how to build a function:")
    print("")
    print("fn add_one(val) {")
    print("  print(val = val + 1)")
    print("}")
    print("")
    print("This function just adds one to the given value and then prints the value to the console.")
}

intro()

while (true) {
    
    const user = input("[" + time() + "]");
    
    if (user == "help") { help() } else {
    if (user == "var") { var() } else {
    if (user == "obj") { obj() } else {
    if (user == "func") { func() } else {
    if (user == "clear") { clear() } else {
    if (user == "exit") { exit() } } } } } }
}