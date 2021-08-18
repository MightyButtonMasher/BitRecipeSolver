cmake_minimum_required(VERSION 3.20)
project(BitRecipeSolver)

set(CMAKE_CXX_STANDARD 20)

add_executable(BitRecipeSolver main.cpp BitRecipe.cpp BitRecipe.h Item.cpp Item.h SubsetSolver.cpp SubsetSolver.h)
