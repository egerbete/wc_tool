# wc_tool

### word, line, character, and byte count

A simple version of the Unix command line tool `wc` to practice C programming.

Inspired by CodingChallenges: https://codingchallenges.fyi/challenges/challenge-wc

### Usage

```
wc [-clmwh] [file]
```

The wc utility displays the number of lines, words, and bytes contained in a single input file,
or standard input (if no file is specified) to the standard output. A line is defined as a string
of characters delimited by a `newline` character.

A word is defined as a string of characters delimited by white space characters.
White space characters are the set of characters for which the `iswspace()` function returns true.

```
-c    The number of bytes in the input file or standard input.
-l    The number of lines in the input file or standard input.
-m    The number of characters in the input file or standard input.
      If the current locale does not support multibyte characters, this is equivalent to the -c option.
-w    The number of words in the input file or standard input.
-h    Print this usage message.
```

The default action is equivalent to specifying the `-c`, `-l` and `-w` options.

If no file is specified, the standard input is used and no file name is displayed.
The prompt will accept input until receiving `EOF`, or `[^D]` in most environments.