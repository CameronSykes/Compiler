cmake_minimum_required(VERSION 3.15)
project(Compiler C)

set(CMAKE_C_STANDARD 99)

add_executable(. fileReader.c node.h tree.c tree.h Test/treeMain.c
        scanner.c scanner.h scannerIO.c scannerIO.h Test/testScanner.c token.h
        parser.c parser.h parserIO.c parserIO.h productions.c productions.h
        semantics.c semantics.h semanticsIO.c semanticsIO.h Test/semanticsMain.c
        codeGenerator.c codeGenerator.h generatorIO.c generatorIO.h generatorMain.c)
