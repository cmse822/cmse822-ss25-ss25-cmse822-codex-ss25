# -*- makefile -*-
################################################################
####
#### This makefile is part of `Parallel Programming in MPI and OpenMP'
#### by Victor Eijkhout, copyright 2013-2024
#### eijkhout@tacc.utexas.edu
####
#### Makefile for MPI course exercises in C++
####
################################################################

info ::

COURSE = mpi

info ::
	@echo "================================================================"
	@echo " make exercises for system=${MODE}, language=${LANGUAGE}"
	@echo "================================================================"
	@echo "available rules:"

EXERCISES = hello \
    commrank prime randommax jordan sumsquares onenorm \
    pingpong sendrecv stridesend stridescatter transposeblock scangather \
    rightsend \
    isendirecv isendirecvarray isendirecvcart \
    procgrid procgridnonblock bucketblock bucketpipenonblock setdiff \
    rightput randomput onesidedbuild countdown lockfetch \
    blockwrite viewwrite scatterwrite cubegather \
    ibarrierupdate rightgraph shareddata
info ::
	@echo "    [ OPTS=... ] (for instance: OPTS=\"-DN 100\")"
	@echo "    support options:"
	@echo "      bucketblock : N"

MODE = mpi
LANGUAGE = CXX
EXTRA_OBJS = tools.o
${OBJS} tools.o : tools.h
include ./Make_targets/Make.compile


include ./Make_targets/Make.clean