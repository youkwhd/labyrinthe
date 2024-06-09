#!/usr/bin/Rscript

library(gridExtra)
library(grid)
library(purrr)

labyrinthe.strategies <- c("best", "breadth", "depth")
labyrinthe.size <- 30

timeElapsed <- function (f) {
    start <- Sys.time()
    f()
    end <- Sys.time()
    as.numeric(end - start)
}

for (strat in labyrinthe.strategies) {
    print(strat)

    for (i in 1:10) {
        cmd <- paste0("./labyrinthe -S", " --strat=", strat, " --width=", labyrinthe.size * i, " --height=", labyrinthe.size * i)
        elapsed <- timeElapsed(function () system(cmd, ignore.stdout=TRUE))
        print(labyrinthe.size * i)
        print(elapsed)
        print("", quote=FALSE)
    }
}
