import hashlib
from concurrent.futures import ProcessPoolExecutor
import os

def load_words(filename):
    """Load words from file."""
    try:
        with open(filename, "r", encoding="utf-8") as f:
            return [line.strip() for line in f if line.strip()]
    except FileNotFoundError:
        print(f"File {filename} not found.")
        return []

def reduction_function(hash_value, words):
    """
    Reduce an MD5 hash to a password in the format word1-word2-NUMBER
    """
    hash_bytes = bytes.fromhex(hash_value)
    combined = int.from_bytes(hash_bytes[8:16], byteorder='little')
    
    num_words = len(words)
    word1 = words[combined % num_words]
    word2 = words[(combined // num_words) % num_words]
    number = (combined // (num_words * num_words)) % 1001  # between 0 and 1000 inclusive

    return f"{word1}-{word2}-{number}"

def load_rainbow_table(file_path):
    """
    Load a rainbow table from file and return a dictionary.
    Handles cases where multiple start values are associated with the same end value.
    """
    table = {}
    with open(file_path, "r", encoding="utf-8") as f:
        for line in f:
            start, end = line.strip().split(",")
            if end not in table:
                table[end] = []
            table[end].append(start)
    
    return table

def search_chain(start_password, hash_value, words, t):
    """
    Search for a hash in a given chain, starting with start_password.
    """
    current_password = start_password
    for j in range(t):
        if hashlib.md5(current_password.encode()).hexdigest() == hash_value:
            return current_password
        current_password = reduction_function(hashlib.md5(current_password.encode()).hexdigest(), words)
    return None

def search_rainbow_table_parallel(hash_value, words, rainbow_table, t):
    """
    Search for a given hash in the rainbow table by testing all chains
    corresponding to an end_word, parallelized with ProcessPoolExecutor.
    """
    
    # Step 1: Calculate potential end_words
    hashtemp = hash_value
    potential_ends = set()
    
    for i in range(t):
        current_password = reduction_function(hashtemp, words)
        potential_ends.add(current_password)
        hashtemp = hashlib.md5(current_password.encode()).hexdigest()
    
    # Step 2: Find all matching chains
    matching_chains = {}
    
    for word in potential_ends:
        if word in rainbow_table:
            if word not in matching_chains:
                matching_chains[word] = []
            matching_chains[word].extend(rainbow_table[word])
    
    if not matching_chains:
        print("No matching chains found...")
        return None
    
    # Step 3: Gather all chains to explore
    start_passwords = []
    for end_word, starts in matching_chains.items():
        start_passwords.extend(starts)
    
    # Step 4: Parallelize the search with ProcessPoolExecutor
    with ProcessPoolExecutor() as executor:
        l = len(start_passwords)
        results = executor.map(search_chain, start_passwords, [hash_value] * l, [words] * l, [t] * l)
    
    # Return the first non-null result
    for result in results:
        if result is not None:
            return result
    
    print("No password found after exploring all chains.")
    return None

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
        hash_input = input("\nHash: ")
        
        result = search_rainbow_table_parallel(hash_input, words, rainbow_table, t)
        
        if result:
            print(f"\nPassword found: {result}")
            break
        else:
            print("Password not found in the rainbow table.")