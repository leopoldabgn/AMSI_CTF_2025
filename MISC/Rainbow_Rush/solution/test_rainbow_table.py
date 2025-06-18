import hashlib
from concurrent.futures import ProcessPoolExecutor
import os
import random

from solve import *

def generate_string_and_hash(dictionary_file):
    """
    Opens the dictionary file, randomly selects 2 words, joins them with a hyphen,
    appends a random number between 0 and 1000, computes the MD5 hash,
    and returns the tuple (generated_string, hash).
    """
    with open(dictionary_file, "r", encoding="utf-8") as f:
        words = [line.strip() for line in f if line.strip()]
    
    word_part = "-".join(random.sample(words, 2))
    number = random.randint(0, 1000)
    generated_string = f"{word_part}-{number}"
    hash_val = hashlib.md5(generated_string.encode()).hexdigest()
    
    return generated_string, hash_val

if __name__ == "__main__":
    # Load words
    filename = "dico.txt"
    words = load_words(filename)

    if not words:
        print("Word list not found. Check your file.")
        exit(1)

    # Parameters
    m = 50_000_000  # Number of chains
    t = 1000        # Chain length
    output_file = "rainbow_table.txt"

    print(f"Total operations: {m * t}")

    # Check if rainbow table exists
    if os.path.exists(output_file):
        print("Rainbow table found. Loading...")
        rainbow_table = load_rainbow_table(output_file)
    else:
        print("Rainbow table not found.")
        exit(1)

    # Search for hashes
    print("Online search...")

    while True:
        password, hash_input = generate_string_and_hash("dico.txt")
        print("\nHash: ", hash_input)

        result = search_rainbow_table_parallel(hash_input, words, rainbow_table, t)
        
        if result:
            print(f"\nPassword found: {result}")
            break
        else:
            print("Password not found in the rainbow table.")