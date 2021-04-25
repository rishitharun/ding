'''
      ding pacman
      ```````````
      Packet Manual to know the naming conventions
      for packets and members

      Developer & Maintainer - Rishi Tharun <vrishitharunj@gmail.com>
'''

from os import walk, chdir, path, getcwd
from sys import argv, exit

if path.isfile('pacman.py'):
  chdir('..')
else:
  pass

if path.isdir('include'):
  pass
else:
  print('/include/ folder missing !')
  print('Make sure that this script is in /tools/ folder...')
  exit(0)

header_file = './include/codes/packet.h'

valid_args = []

help = '''
ding pacman (Packet Manual)
---------------------------
This script serves as the manual that helps the user to know about how the member variables of each packet type are addressed in ding.
A comprehensive document could have been made for this purpose. But, a script seems to be quite good in showing the user precisely, concisely, and efforlessly what he/she needs.
Also, if there is any change in the source code, such as modifying the name of any member, then the same changes need to be carefully reflected on to the documentation.
As humans, we may very well miss or forget that, which will consequently lead to an incorrect documentation.
Having this script designed in such a way that it retrieves data from the header files, any changes done internally, will also be reflected in the documentation.
If you require, you can very well open the script, find out the header file for the required packet, and find out how the member variables are named, all by yourselves. But why do so much extra work?

In order to get details about members of a particular packet type, pass the name of the packet type as the argument.
It is possible, that your and our naming conventions may not match. So to get the list of valid arguments, pass 'list-args' as argument. This list of valid arguments, specifies the packet names as per ding naming convention.

Thanks,
Team ding.
'''

def getArgs():
  f=open(header_file,'r')
  header = f.readlines()
  f.close()

  packets = set()
  for line in header:
    if '#define __' in line:
      packets.add(line.split('#define __')[1].split('_')[0].split(' ')[0])
    else:
      pass
  return packets


def construct(pkt_type):
  info = ''
  f=open(header_file,'r')
  header = f.readlines()
  f.close()

  for line in header:
    if '#define __'+pkt_type+'_' in line:
      split = '  * '+line.split('#define __'+pkt_type+'_')[1].split(' ')[0]+' --- '+line.split('//')[1]
      info+=split
    else:
      pass

  return info

if __name__ == '__main__':
  valid_args = getArgs()
  if len(argv) == 1:
    print('An argument is required...')
    print("Pass 'help' argument to get idea...")

  else:
    if argv[1] == 'list-args':
      print()
      print('List of valid arguments are as follows...')
      print(valid_args)

    elif argv[1] == 'help':
      print(help)
      
    elif argv[1] not in valid_args:
      print("'"+argv[1]+"' is not a valid argument...")
      print("Pass 'list-args' as argument to get the list of valid arguments...")

    else:
      print('\n ===============\n | ding pacman |\n ===============\n')
      print(' '+argv[1])
      print(' '+'-'*len(argv[1]))
      print(construct(argv[1]))
