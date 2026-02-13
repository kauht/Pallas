import os
import subprocess

for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith((".cpp", ".h", ".hpp")):
            path = os.path.join(root, file)
            subprocess.run(["clang-tidy", "-fix", "-p", "build", path])
