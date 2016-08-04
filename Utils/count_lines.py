import os
import sys



def for_each_file(path, func):
    for folder, subs, files in os.walk(path):
            for filename in files:
                func(os.path.join(folder, filename))


def count_lines_in_file(path):
    lines = 0
    lines_wo_empty = 0
    
    with open(path, 'r') as src:
        for line in src:
            lines += 1
            if line.strip():
                lines_wo_empty += 1
    return lines, lines_wo_empty
            

class Adder:
    def __init__(s, ext = []):
        s.lines = 0
        s.lines_wo_empty = 0
        s.ext = ext

    def __call__(s, path):
        if not s.file_valid(path):
            return
        lines, lines_wo_empty = count_lines_in_file(path)
        s.lines += lines
        s.lines_wo_empty += lines_wo_empty

    def file_valid(s, path):
        if not s.ext:
            return True
        ext = os.path.splitext(path)
        return ext[1] in s.ext


#rootdir = sys.argv[1]
rootdir = "./"

def count_lines_script():
    add = Adder(['.json', '.msl'])
    for_each_file('../ProjectCrimson/Res/Script/', add)
    print 'Scripts in json:'
    print ' -- all lines: ' + str(add.lines)
    print ' -- nonempty lines: ' + str(add.lines_wo_empty)
    print ' '
    print ' '
    

def count_lines_code_game():
    add = Adder(['.cpp', '.h'])
    for_each_file('../ProjectCrimson/Game/', add)
    print 'Source in game:'
    print ' -- all lines: ' + str(add.lines)
    print ' -- nonempty lines: ' + str(add.lines_wo_empty)
    print ' '
    print ' '


def count_lines_code_mxlib():
    add = Adder(['.cpp', '.h'])
    for_each_file('../MXLib_Cinder/MXLib/', add)
    print 'Source in engine:'
    print ' -- all lines: ' + str(add.lines)
    print ' -- nonempty lines: ' + str(add.lines_wo_empty)
    print ' '
    print ' '

count_lines_script()
count_lines_code_game()
count_lines_code_mxlib()
