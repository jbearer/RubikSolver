cycles = [
         [2, 4, 0, 1, 3, 5]
         [0, 2, 4, 3, 5, 1]
         [1, 2, 3, 4, 5, 0]
         [2, 3, 1, 0, 4, 5]
         [3, 0, 5, 2, 1, 4]
         [4, 0, 2, 5, 1, 3]
         [5, 3, 0, 1, 4, 2]
         [0, 5, 3, 4, 2, 1]
         [1, 5, 4, 3, 2, 0]
         [3, 1, 2, 5, 0, 4]
         [4, 1, 5, 2, 0, 3]
         [5, 4, 1, 0, 3, 2]]

def check(LoL):
    for i in range(6):
        for n in range(6):
            appears = False
            for row in LoL:
                if row[i] == n:
                    appears = True
                    break
            if appears == False:
                return False

for row in cycles:
    c = cycles.copy()
    c.remove(row)
    if (check(c)):
        print("removed " + str(row))



