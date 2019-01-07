
import sys

def find_l_mers(DNA, k):
    sequence_length = len(DNA)
    print("Sequence length is " + str(sequence_length))
    print("k value is " + str(k))
    l_mers = {}
    for i in range(k, sequence_length+1):
        l_mer = DNA[i-k:i:]
        if l_mer in l_mers:
            l_mers[l_mer] = l_mers[l_mer] + 1
        else:
            l_mers[l_mer] = 1
    maxval = 0
    maxval_keys = []
    minval = sys.maxsize
    minval_keys = []
    for key in l_mers.keys():
        value = l_mers[key]
        if value >= maxval:
            if value == maxval:
                maxval_keys.append(key)
            else:
                maxval = value
                maxval_keys = [key]
        if value <= minval and value > 1:
            if value == minval:
                minval_keys.append(key)
            else:
                minval = value
                minval_keys = [key]
        print(key + " " + str(value))
    print("Maximum count is " + str(maxval))
    for key in maxval_keys:
        print(key + " " + str(maxval))
    print("Minimum count is " + str(minval))
    for key in minval_keys:
        print(key + " " + str(minval))

if __name__ == "__main__":
    k = 1
    if len(sys.argv) > 1:
        k = int(sys.argv[1])
    genome = None
    with open("Escherichia_coli_genome.fasta", "r") as file:
        genome = file.read()
    genome = genome[genome.find("\n")::]
    genome = genome.replace("\n", "")
    find_l_mers(genome, k)
