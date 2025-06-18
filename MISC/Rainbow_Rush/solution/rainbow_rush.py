import hashlib
import random
import time

DICTIONARY_FILE = "dico.txt"
FLAG = "AMSI{PR070CH1P_D3CRYP710N_53QU3NC3_C0MPL373}"
TIME_LIMIT = 60  # seconds

def generate_password_and_hash(dictionary_file):
    with open(dictionary_file, "r", encoding="utf-8") as f:
        words = [line.strip() for line in f if line.strip()]

    word1, word2 = random.sample(words, 2)
    number = random.randint(0, 1000)
    password = f"{word1}-{word2}-{number}"
    hash_val = hashlib.md5(password.encode()).hexdigest()

    return password, hash_val

def main():
    print("=== Welcome to CYBERRUN 1984 ===")
    print("You’ve reached the hidden terminal behind the high score machine.")
    print("Decrypt the MD5 password to unlock the prototype chip data.")
    print(f"You have {TIME_LIMIT} seconds. Format: WORD1-WORD2-NUMBER")
    print("Example: sauced-betook-220, martin-panino-3, acture-demons-1000\n")

    password, md5_hash = generate_password_and_hash(DICTIONARY_FILE)
    print(f"MD5 Hash: {md5_hash}")

    start_time = time.time()
    user_input = input("\nEnter the password: ").strip()
    elapsed = time.time() - start_time

    if elapsed > TIME_LIMIT:
        print("\n⏱️ Time's up! You were too slow.")
    elif user_input == password:
        print("\n✅ Access granted. Unlocking prototype chip...")
        print(f"\nFLAG: {FLAG}")
    else:
        print("\n❌ Access denied. Incorrect password.")

if __name__ == "__main__":
    main()
