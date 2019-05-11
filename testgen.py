#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import sys

MAX_ARRAYS = 1000
MAX_ROW_SIZE = 1000

MIN_VAL = -500000
MAX_VAL = 500000


def progressBar(value, endvalue, bar_length=20):

        percent = float(value) / endvalue
        arrow = '-' * int(round(percent * bar_length)-1) + '>'
        spaces = ' ' * (bar_length - len(arrow))

        sys.stdout.write("\rPercent: [{0}] {1}%".format(arrow + spaces, int(round(percent * 100))))
        sys.stdout.flush()

# get number of array to generate 

size = input("How many arrays I should generate: [random] ")

if size == "":
    size = random.randint(1, MAX_ARRAYS)
else:
    size = int(size)


if size > MAX_ARRAYS:
    print("Max array count is "+ str(MAX_ARRAYS))
    size = MAX_ARRAYS


# get max array size 

max_arr_size = input("Max array size: [random] ")

if max_arr_size == "":
    max_arr_size = random.randint(1, MAX_ROW_SIZE)
else:
    max_arr_size = int(max_arr_size)


if max_arr_size > MAX_ROW_SIZE:
    print("Max array lenght is "+ str(MAX_ROW_SIZE))
    size = MAX_ROW_SIZE


# generate and save arrays

print("Generating " + str(size) + " arrays")

with open("data/test_data.txt", "w") as fd:
    i = 0
    while i < size:
        i += 1

        arr_sz = random.randint(1, max_arr_size)

        progressBar(i, size)

        j = 0
        while j < arr_sz:
            j += 1

            fd.write(str(random.randint(MIN_VAL, MAX_VAL)))
            if j != arr_sz: 
                fd.write(" ")
    
        fd.write("\n")

print("\nTest file generated")