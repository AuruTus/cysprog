# Lab1 mcc compiler assignment

**Table of Content**
- [Exercise 4](#exercise-4)
- [Exercise 5](#exercise-5)

## Exercise 4

**Q:** What does the compiler do when the parameters "-o", "-S", "-c", "-m" and "-v" are added or not, and how can these parameters be used in combination?

**A:** As we can found the cli args in `main.c`. and here are that part snippets,

```c
static struct argp_option options[] = {
    {" ", 'c', 0, 0, "Produce .o file"},
    {" ", 'S', 0, 0, "Produce .s file"},
    {"output", 'o', "OUTPUT", 0, "Produce a custom output name"},
    {"machine", 'm', "arch", 0, "Select an arch, support X64 and RISCV, default X64"},
    {"verbose", 'v', 0, 0, "Show detailed information during compilation"},
    {0}
};

```
  these are all the options this compiler will use. And in the following pattern match code, we can know how the `mcc` (as a state machine) will do with this options.

```c
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'c':
            arguments->mode = ASSEMBLY;
            break;
        case 'S':            
            arguments->mode = COMPILE;
            break;
        case 'o':
            arguments->output_name = arg;
            break;
        case 'm':
            arguments->arch = arg;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case ARGP_KEY_ARG:
            if (!arguments->input_file) {
                arguments->input_file = arg;
            } else {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
```

As it suggests, the `c` and `S` cannot exist at the same time, for it will affect the output format. But actually, because the interface isn't designed to fit for process multiple options once, we can still overwrite the former option like this `./mcc -cS input.c` and it will output `input.s`. And we can even produce such an output file `input.sss` with `./mcc -cSo input.sss input.c`, just because the CLI doesn't check those input options and deal with the incompatibility. 

Luckily, this state machine is too simple to produce uncontrollable and disastrous code base. We can still accept it as an educational tool, however, I belive it could be better with a modern CLI interface like `cobra` in `Golang`, while I'm not familliar with `C` tools and repos, I cannot give some concret advices here.

## Exercise 5

**Q:** How does mcc implement an assignment statement like a = a + 5. What about an output statement print(a)?

**A:** These tool sets look like `yacc`, for which we provide a set of symbol system of the syntax, and finally we can get a set of lexer and parser front end in our project language with it.

The lexical symbol are defined in `scanner.l`, and when using `flex scanner.l` we get the lexer `lex.yy.c`. And similarly we can get the parser `paser.tab.*` with `bison`. With these tools, we can get an `AST`.
And in our `main.c`, we traverse the `AST` starting from `root` and generate temprary assembly IR code, and finnaly assemble it to our `ELF` file in `linux x86` by default (, if no `-S` provided).

For assignment statement (, let's talk about `x86`, because `Risc-V` version is incomplete for `print` and in fact the implement principles are the same), there's a pattern arm for it, the `case STM_ASSIGN:`. It will search the `identifier` in the symbol table (, or set in this program) and the continue to compile the right expression with `compile_exp(s->exp);` and finally emit a `movq` statement as a eventual assignment.

And for print statement, it's similar, but no symbol search this time. Evaluate the right expression and then prepare args for syscall (, add a final `\n` for this output in `leaq mcc_format(%rip), %rdi`) and call `printf` finally.
