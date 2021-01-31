import subprocess
import os

code_dirs = ['src', 'hdr']

for code_dir in code_dirs:
    for root, dirs, files in os.walk(code_dir):
        for file in files:
            output = subprocess.check_output(['clang-format', '-style=file', '-i', os.path.join(root, file)])
            print(output)
