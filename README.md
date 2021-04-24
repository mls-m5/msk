msk21
=======================

I felt for making another compiler, with some random syntax.

This is what the first test would be expected to look like.

apa.msk
```c++
class Apa {
    let _x : double = 0;
    let _y : double = 0;
};

export func move(Apa &, double x, double y) {
    _x = x;
    _y = y;
}

```

main.msk:
```c++
import apa;

func main(std::view<std::string_view> args) -> void {
    let x = 10;

    print("hello there! {}\n", x);

    return 0;
}
```

Design goals for the language:
 * Nice looking (ie like c++, because I like c++)
 * Generation of AST on a single pass, without knowledge of what types is used
 * Compilation speed
 
Future goals (will probably never be reached)
 * Templates/meta programming
 * A standard lib for printing, vectors and other fun stuff.
 * Runtime performance
