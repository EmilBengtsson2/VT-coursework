import Bio
from Bio import Entrez
import argparse
import subprocess
import time

'''
Fetches fasta files of genomes from the ncbi database
Requires wget and gunzip
'''

def parse_args():
    '''
    Handle arguments to the script
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument("email", help="The email that is given to Entrez")
    parser.add_argument("search_string", help="The string to search for in the ncbi genome database. \
        The script fetches all the full genomes it can find. (up to 10000)")
    return parser.parse_args()

def get_genome_id_list(search_string):
    '''
    Returns a list of all the genome id's that the esearch could find. (max length 10000)
    '''
    search_handle = Entrez.esearch(db="genome", term=search_string, retmax=10000)
    search_result = Entrez.read(search_handle)
    return search_result["IdList"]

def get_assembly_id_list(genome_id_list):
    '''
    Returns a list containing the assembly id's for the genomes that have them
    '''
    id_string = ""
    for genome_id in genome_id_list:
        id_string += genome_id + ","
    id_string = id_string[:-1]
    summaries = Entrez.esummary(db="genome", id=id_string)
    assembly_id_list = []
    for summary in Entrez.read(summaries):
        assembly_id = summary['AssemblyID']
        if assembly_id != '0':
            assembly_id_list.append(assembly_id)
    return assembly_id_list

def get_ftp_address_list(assembly_id_list):
    '''
    Returns a list of GenBank ftp-addresses corresponding to the assembly id's
    '''
    id_string = ""
    for assembly_id in assembly_id_list:
        id_string += assembly_id + ","
    id_string = id_string[:-1]
    summaries = Entrez.esummary(db="assembly", id=id_string)
    ftp_address_list = []
    with open("taxonomy_mapping", "w") as file:
        for summary in Entrez.read(summaries)['DocumentSummarySet']['DocumentSummary']:
            ftp_address = summary['FtpPath_GenBank']
            if ftp_address[len(ftp_address)-1] == '/':
                ftp_address = ftp_address[:-1]
            ftp_address += "/" + ftp_address[::-1][:ftp_address[::-1].index('/')][::-1] + "_protein.faa.gz"
            ftp_address_list.append(ftp_address)
            file.write("Taxid: " + str(summary['Taxid']) + ", Filename: " + ftp_address[::-1][:ftp_address[::-1].index('/')][::-1][:-3] + "\n")
    return ftp_address_list

if __name__ == "__main__":
    args = parse_args()
    Entrez.email = args.email

    genome_id_list = get_genome_id_list(args.search_string)
    assembly_id_list = get_assembly_id_list(genome_id_list)
    ftp_address_list = get_ftp_address_list(assembly_id_list)

    for ftp_address in ftp_address_list:
        wget_string = "wget -N --timeout=1 --tries=5 " + ftp_address
        subprocess.run(wget_string, shell=True)
        gunzip_string = "gunzip " + ftp_address[::-1][:ftp_address[::-1].index('/')][::-1]
        subprocess.run(gunzip_string, shell=True)
