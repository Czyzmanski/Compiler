# Compiler
Compiler reads code written in specific programming language, derivable from the following context-free grammar, 
and compiles it into code that can be interpreted by virtual machine.

    Program → Procedures MainProcedure
    MainProcedure → ProcedureDefinition
    Procedures → ε | Procedure Procedures
    Procedure → ProcedureName ProcedureDefinition
    ProcedureName → A | B | C | D | E | F | G | H | I | J | K | L | M
                  | N | O | P | Q | R | S | T | U | V | W | X | Y | Z
    ProcedureDefinition → { Commands }
    Commands → ε | Command Commands
    Command → Invocation | Writing | Branch
    Invocation → ProcedureName
    Writing → Target Bits
    Target → StackName | SpecialName
    StackName → a | b | c | d | e | f | g | h | i | j | k | l | m
              | n | o | p | q | r | s | t | u | v | w | x | y | z
    SpecialName → $
    Bits → Bit Bits | Bit
    Bit → - | +
    Branch → Source ProcedureDefinition ProcedureDefinition
    Source → StackName | SpecialName

There can also occur whitespace characters and comments from `;` character to the end of the line in the source code of
opcode program written in the programming language derivable from above grammar. They do not affect semantics of the program.
