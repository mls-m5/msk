msk21
=======================

I felt for making another compiler, with some random syntax.

This is what the first test would be expected to look like.

apa.msk
```c++
export class Apa {
    let x : double = 0;
    let y : double = 0;
};

export func Apa(public double x, public double y) {
}

export func move(Apa &, double x, double y) {
    _x = x; // '_' prefix for member variables
    _y = y;
}

```

main.msk:
```c++
import apa;

func main(std::view<std::string_view> args) -> void {
    let apa = Apa{
       .x = 0,
       .y = 1,
    };
    
    apa.move(10, 20);

    print("hello there! {}, {}\n", apa.x, apa.y);

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

