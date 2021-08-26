# matchr



match(value, pat ~ body, ...)

```
<pat> ::= (<pat>)
        | quote(<expr>)
        | <id>
        | .
        | <literal>
        | <pat> && <pat>
        | <pat> || <pat>
        | !<pat>
        | any(<pat> ...)
        | all(<pat> ...)
        | none(<pat> ...)
        | sat(<expr>)
        | apply(<expr>, <pat>)
        | regex(<rx-pat>)
        | regex(<rx-pat>, <pat>)
        | vector(<patseq>)

<literal> ::= <logical-atomic>
            | <integer-atomic>
            | <character-atomic>
            | <double-atomic>

<vector> ::= <logical-vector>
           | <integer-vector>
           | <double-vector>
           | <character-vector>
           | <raw-vector>
           | <complex-vector>

<logical-vector> ::= logical(<logical-atomic>, ...)
                   | lgl(<logical-atomic>, ...)

<integer-vector> ::= integer(<integer-atomic>, ...)
                   | int(<integer-atomic>, ...)

<double-vector> ::= double(<double-atomic>, ...)
                  | dbl(<double-atomic>, ...)

<character-vector> ::= character(<character-atomic>, ...)
                     | chr(<character-atomic>, ...)

<raw-vector> ::= raw(0-255, ...)

<complex-vector> ::= complex(<double-atomic> + i <double-atomic>, ...)
                   | cplx(<double-atomic> + i <double-atomic>, ...)

<environment> ::= environment(x = )
```
