'''
      ding Interpreter Build Program
      ``````````````````````````````
      This script compiles all source files and generates
      the executable binary, which is the ding interpreter.

      Developer & Maintainer - Rishi Tharun <vrishitharunj@gmail.com>
'''

# --COMMAND-- #
#  gcc -w -o "./ding" <source_files> -lm -iquote <include_path>

from os import system, walk, chdir, path, getcwd
from sys import exit

if path.isfile('build.py'):
  chdir('..')
else:
  pass

if path.isdir('src'):
  pass
else:
  print('/src/ folder missing !')
  print('Make sure that this script is in /tools/ folder...')
  exit(0)

if path.isdir('include'):
  pass
else:
  print('/include/ folder missing !')
  print('Make sure that this script is in /tools/ folder...')
  exit(0)

sources = ''
include_path = './include'

src_folder = list(walk('./src'))

for sub_dir in src_folder:
  dir_name = sub_dir[0]
  for c_file in sub_dir[2]:
    sources += dir_name+'/'+c_file+' '

cmd = 'gcc -o "./ding" ' +sources+ '-lm -iquote' + include_path

system(cmd)

