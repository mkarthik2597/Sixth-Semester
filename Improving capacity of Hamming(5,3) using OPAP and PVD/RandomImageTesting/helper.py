from import_modules import *

rand_seed = 100

def str_to_bin(string):
    """ Take a string and return a binary representation of the string based
        upon the ASCII values of each character """
    # Creates a list of elements. Here, the list consists of individual characters
    ret = list(string)

    # ord('x') converts 'x' to ASCII
    # convert to binary representation. Each element is a 7-bit ASCII representation.

    ret = ['{:07b}'.format(ord(x)) for x in ret]
    # Each element now is a list of the 7-bit ASCII representation of each character
    ret = [[bit for bit in x] for x in ret]
    # Merge all the sublists to a single list. Meanwhile, convert each string to an integer
    ret = [int(bit) for sublist in ret for bit in sublist]
    return ret

def bin_to_str(binary):
    """ take a list of binary (integer ones and zeros) and return its ascii
        string representation """
    output = []
    for i in range(int(math.floor(len(binary)/7))):
        start = i * 7
        char = binary[start:start+7]
        char = '0b' + ''.join([str(c) for c in char])
        output.append(int(char, 2))
    #print output
    st = ''.join([chr(o) for o in output if o != 0])
    return str(st)

def convert_msg(msg):
    """ Takes a list of binary numbers and groups them into triples """
    while len(msg) % 3 != 0:
        # pad the message to length
        msg.append(0)
    bin_strings = [msg[3*i:3*i+3] for i in range(int(len(msg)/3))]
    return bin_strings

def convert_lsb(lsb):
    """ Takes a list of LSBs and groups them into 5 LSBs per group """

    remainder = len(lsb) % 5
    lsb = lsb[:len(lsb)-remainder]

    bin_strings = [lsb[5*i:5*i+5] for i in range(int(len(lsb)/5))]
    return bin_strings

    return bin_strings

def toDec(binary,n):
    """ Convert an n-bit binary number to its decimal value """
    mult_arr = 2**np.arange(n)[::-1]
    ans = np.dot(binary,mult_arr)
    return int(ans)

def toBinary(num,bits):
    str = []

    for i in range(bits):
        str.append(num%2)
        num = int(math.floor(num/2))

    return str[::-1]

