import sys
from environment import command

log_files = [sys.stdout]

@command
def setup_log(files=[], stdout=True, stderr=False):
    global log_files
    if not hasattr(files, '__iter__'): files = [files]
    log_files = [ open(f,'a') if isinstance(f,str) else f for f in files ]
    if stdout: log_files.append(sys.stdout)
    if stderr: log_files.append(sys.stderr)

    
def log(s='', without_newline=False):
    for f in log_files:
        if without_newline:
            print >>f, s,
        else:
            print >>f, s
        f.flush()
