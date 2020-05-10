import shlex
import subprocess
import getopt
import os
import sys

# Repo Name
PROJECT_NAME = "Assignment_1"

# Directory Relavent Variables
SOURCE_DIR = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = SOURCE_DIR + "/.build/" + PROJECT_NAME

# TODO: Add Windows support

def create_directory_if_not_exist(dirName):
     if not os.path.exists(dirName):
          os.makedirs(dirName)  

def build_linux(toolchain):
     create_directory_if_not_exist(BUILD_DIR)
     
     # Right now we only have one target but use subprocess to future
     # proof our build process in a possible future cases where although
     # the target is the same, we might have different 'CARDS' of which
     # our executable will be ran on

     # Setup arguments we'll be passing into CMake
     cmake_arguments  = "-G Ninja"
     cmake_arguments += " -S " + str(SOURCE_DIR) # Specify Source Tree
     cmake_arguments += " -B " + str(BUILD_DIR)  # Specify Buiild Tree

     subprocess.run(["cmake " + cmake_arguments], shell=True, cwd=BUILD_DIR)
     subprocess.run(["ninja"], shell=True, cwd=BUILD_DIR)


def build_project(build_arguments):
     try:
          opts, args = getopt.getopt(build_arguments,"h")
     except getopt.GetoptError:
               print(__file__ + ': Failed to parse arguments')
               sys.exit(2)

     for opt, arg in opts:
          if opt == '-h':
               print("-h : Prints out a help menu")
               sys.exit()

     # Here we would want to add different toolchains and integrate them into
     # the build process for cross compilation.
     #
     # For now we just assume our native platform (Linux-GNU) will be our target
     # as well.
     #
     # DummyToolchain serves to just hit this point home.. doesn't do anything
     build_linux("DummyToolchain.cmake")

if __name__ == "__main__":
     build_project(sys.argv[1:])
