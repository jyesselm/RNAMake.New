
def get_lines_from_file(path):
    f = open(path)
    lines = f.readlines()
    f.close()
    return lines