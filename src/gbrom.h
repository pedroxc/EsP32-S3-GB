bin2h utility v1.01

Interprets any file as plain binary data and dumps to a raw C/C++ array.
usage: bin2h <in-file> <out-file> <opt-args>

valid optional arguments:
-id=<name> the C array is identified as "name". identifier is "data" if this argument is not present. bin2h does not check the identifier is valid in C/C++.
-ns=<namespace> causes the data to be wrapped in a namespace. no namespace is inserted if this argument is not used.
