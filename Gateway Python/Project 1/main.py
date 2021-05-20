# -*- coding: utf-8 -*-
codons = {"UUU":"Phe", "UUC":"Phe", "UUA":"Leu", "UUG":"Leu",
          "UCU":"Ser", "UCC":"Ser", "UCA":"Ser", "UCG":"Ser",
          "UAU":"Tyr", "UAC":"Tyr", "UAA":"STOP", "UAG":"STOP",
          "UGU":"Cys", "UGC":"Cys", "UGA":"STOP", "UGG":"Trp",
          "CUU":"Leu", "CUC":"Leu", "CUA":"Leu", "CUG":"Leu",
          "CCU":"Pro", "CCC":"Pro", "CCA":"Pro", "CCG":"Pro",
          "CAU":"His", "CAC":"His", "CAA":"Gln", "CAG":"Gln",
          "CGU":"Arg", "CGC":"Arg", "CGA":"Arg", "CGG":"Arg",
          "AUU":"Ile", "AUC":"Ile", "AUA":"Ile", "AUG":"Met",
          "ACU":"Thr", "ACC":"Thr", "ACA":"Thr", "ACG":"Thr",
          "AAU":"Asn", "AAC":"Asn", "AAA":"Lys", "AAG":"Lys",
          "AGU":"Ser", "AGC":"Ser", "AGA":"Arg", "AGG":"Arg",
          "GUU":"Val", "GUC":"Val", "GUA":"Val", "GUG":"Val",
          "GCU":"Ala", "GCC":"Ala", "GCA":"Ala", "GCG":"Ala",
          "GAU":"Asp", "GAC":"Asp", "GAA":"Glu", "GAG":"Glu",
          "GGU":"Gly", "GGC":"Gly", "GGA":"Gly", "GGG":"Gly"}
        
def main():
    """ Write your code here """
    dna = readFile('apoe.txt')
    
    
    writeFile('replication.txt',replication(dna))  #Writes the replicaiton.txt file using the transcription method 
                                                                    #and readFile method to read in the gene and write it using writeFile
    if allele(dna) == 4:
        print('This allele indicates a higher risk of Alzheimer’s disease.')
    else:
        print('This allele indicates a lower risk of Alzheimer’s disease.')
    
    print('This GC-content of this gene us %.2f%%' % gcContent(dna))
    
    
    
    pass

def readFile(fileName):
    """
    input: fileName to read
    output: dna sequence as string
    
    """
    with open(fileName,'r') as dnaFile:
        bases = dnaFile.readlines()
        
    return ''.join(bases)
    
    
def writeFile(fileName,dna):
    """
    input: fileName to write
           dna as a string
    """
    with open(fileName,'w') as textFile:
        textFile.write(dna)

    
def replication(dna):
    """ Write your code here """
    dnarep = ''                   #Create empty string to begin to build replacement sequence
    for i in range(0,len(dna)):   #For loop that goes through the string one character at a time to begin to replace individuals letters
        if dna[i] == 'A':
            dnarep = dnarep + 'T'  ##Series of If statements that check what letter is currently being examined and adding it to
        elif dna[i] == 'G':        #the replacement sequence with its compliment
            dnarep = dnarep + 'C'
        elif dna[i] == 'T':
            dnarep = dnarep + 'A' 
        elif dna[i] == 'C':
            dnarep = dnarep + 'G'
    return dnarep                 # Return the replacement string that was built in the for loop
    
    pass    

def transcription(dna):
    """ Write your code here """
    mRNA = ''
    for i in range(0, len(dna)): #For loop that goes through the string one character at a time to begin to replace individuals letters
        if dna[i] == 'T':        #If statement checks if the current character is a T, if it is it replaces it with a U
            mRNA = mRNA + 'U'
        else:                    #Otherwise the other letter is added and not replaced by another character
            mRNA = mRNA + dna[i]
    return mRNA                  #Return the transcribed DNA sequence
    
    
    
    
    
    pass    

def translation(mrna):
    """ Write your code here """
    translation = ''         #Empty stirng to begin to build translation
    i = 0                    #Create iterator variable for while loop
    while i < len(mrna):     #While loop runs until i exceeds or is equal too the length of the input mrna string
        if i == 0:           #Checks to make sure that it is not the first translation added to avoid putting a space in front
            translation = translation + codons[mrna[i:i+3]] #Adds translation to 
        else:
            translation = translation + " "+codons[mrna[i:i+3]]
        i = i+3
    return translation
    pass

def allele(dna):
    """ Write your code here """
    index = dna.find('GGCCGCCTGGTGCAGTACCGCGGC') - 3
    index1 = dna.find('CTGGCAGTGTACCAGGCC') - 3
    
    firstGene = dna[index: index+3]
    secondGene = dna[index1: index1+3]
    
    firstGene = transcription(firstGene)
    secondGene = transcription(secondGene)
    
    YYY = codons[firstGene]
    ZZZ = codons[secondGene]
    
    if YYY == 'Cys' and ZZZ == 'Cys':
        return 2
    elif YYY == 'Arg' and ZZZ == 'Arg':
        return 4
    elif YYY == 'Cys' and ZZZ == 'Arg':
        return 3
    else:
        return -1
    
    
    
    pass

def gcContent(dna):
    """ Write your code here """
    GCcount = 0
    for i in range(0,len(dna)):
        if dna[i] == 'G' or dna[i] == 'C':
            GCcount += 1
    return GCcount/len(dna) * 100
    pass
def hamming(dna1, dna2):
    """ Write your code here """
    if len(dna1) != len(dna2):
        return -1
    differences = 0
    for i in range(0,len(dna1)):
        if dna1[i] != dna2[i]:
            differences += 1
    return differences
    
    
    pass

if __name__ == "__main__": main()